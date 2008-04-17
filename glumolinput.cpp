/*
Glumol - An adventure game creator
Copyright (C) 1998-2008  Sylvain Baubeau & Alexis Contour

This file is part of Glumol.

Glumol is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Glumol is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Glumol.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"
#include "glumolinput.h"
#include "glumolgame.h"

CGlumolKeyDown::CGlumolKeyDown(const CL_InputEvent& key, bool m)
{
	start = 0;
	kill = false;
	mouse = m;
	id = key.id;
	type = key.type;
}

void CGlumolKeyDown::Start()
{
	start = CL_System::get_time();
}
	
DWORD CGlumolKeyDown::Now()
{
	return CL_System::get_time() - start;
}

void CGlumolKeyDown::Kill()
{
	kill = true;
}

int CGlumolKeyDown::GetId()
{ 
	return id;
}

bool CGlumolKeyDown::IsMouse()
{ 
	return mouse;
}

bool CGlumolKeyDown::IsDead()
{
	return kill;
}

CGlumolInputDispatcher::CGlumolInputDispatcher(CGlumolGame* app)
{
	game = app; 
	mousedown = CL_Mouse::sig_key_down().connect(this, &CGlumolInputDispatcher::OnMouseButtonDown);
	mouseup = CL_Mouse::sig_key_up().connect(this, &CGlumolInputDispatcher::OnMouseButtonUp);
	mousemove = CL_Mouse::sig_move().connect(this, &CGlumolInputDispatcher::OnMouseMove);	
	keydown = CL_Keyboard::sig_key_down().connect(this, &CGlumolInputDispatcher::OnKeyDown);	
	keyup = CL_Keyboard::sig_key_up().connect(this, &CGlumolInputDispatcher::OnKeyUp);	
}

CGlumolInputDispatcher::~CGlumolInputDispatcher()
{
}

void CGlumolInputDispatcher::DispatchRepeat()
{
	for(std::list<CGlumolKeyDown*>::iterator i = downlist.begin();
		i != downlist.end(); i++) {
		if(is_dead<CGlumolKeyDown*>()(*i)) {
			continue;
		}
		if((*i)->IsMouse()) {
			switch((*i)->GetId()) {
				case 0:
                    game->on_left_button_down_repeat((*i)->Now()); 
					return; 
				case 1: 
					game->on_right_button_down_repeat((*i)->Now());
					return; 
				case 2:
					game->on_middle_button_down_repeat((*i)->Now());
					return; 
				default:
					return;
			}
		}
		else {
			game->on_key_down_repeat((*i)->GetId(), (*i)->Now()); 
		}
	}
	/*
	for(std::list<CGlumolKeyDown*>::iterator i = downlist.begin();
		i != downlist.end(); i++) {
		if(is_dead<CGlumolKeyDown*>()(*i)) {
			i = downlist.erase(i);
		}
	}
	*/
}

void CGlumolInputDispatcher::OnMouseMove(const CL_InputEvent &key)
{
	// game->UpdateInputs(); 
	CL_Pointf ftmp;
	ftmp.x = (float) key.mouse_pos.x; 
	ftmp.y = (float) key.mouse_pos.y; 
	game->on_mouse_move(game->cursor_pos, ftmp, key.axis_pos);
	game->cursor_pos = ftmp; 
}

void CGlumolInputDispatcher::OnKeyDown(const CL_InputEvent& key)
{
	for(std::list<CGlumolKeyDown*>::iterator i = downlist.begin(); i != downlist.end(); i++)
		if (!(*i)->IsMouse() && key.id == (*i)->GetId())
			return;
	CGlumolKeyDown* d = new CGlumolKeyDown(key,false); 
	downlist.push_back(d); 
	d->Start();
	game->on_key_down(key.id); 
}

void CGlumolInputDispatcher::OnMouseButtonDown(const CL_InputEvent &key)
{
	CGlumolKeyDown* d = new CGlumolKeyDown(key,true); 
	downlist.push_back(d); 
	d->Start();
	game->on_mouse_button_down(key.id); 
}

void CGlumolInputDispatcher::OnMouseButtonUp(const CL_InputEvent &key)
{
	Kill(key); 
	game->on_mouse_button_up(key.id);
}

void CGlumolInputDispatcher::OnKeyUp(const CL_InputEvent &key)
{
	Kill(key); 
	game->on_key_up(key.id);
}

void CGlumolInputDispatcher::Kill(const CL_InputEvent& key)
{	
	for(std::list<CGlumolKeyDown*>::iterator i = downlist.begin(); i != downlist.end(); i++) {
		if (key.id == (*i)->GetId()) {
			delete (*i);
			downlist.erase(i);
			return;
			//(*i)->Kill(); 
		}
	}
	// throw "Oulala";
}
