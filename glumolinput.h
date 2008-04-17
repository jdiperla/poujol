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


#ifndef __GLUMOLINPUT_H__
#define __GLUMOLINPUT_H__

#include "time.h"
#include <ClanLib/display.h> 
#include <ClanLib/signals.h> 
#include "misc_classes.h"
#include "glumolsprite.h"

class CGlumolGame;

class CGlumolKeyDown
{
public: 	
	DWORD start;
	bool kill;
	bool mouse; 
	int id; 
	CL_InputEvent::Type type;

	CGlumolKeyDown(const CL_InputEvent& key, bool m);
	void Start();
	DWORD Now();
	void Kill();
	int GetId();
	CL_InputEvent::Type GetType() { return type; }
	bool IsMouse();
	bool IsDead();
}; 

template <class T > class is_dead  : public std::unary_function<T , bool>
{
public:
	bool operator( ) ( T& key ){ return (key->IsDead() ); }
};

class CGlumolInputDispatcher
{
public:
	CGlumolGame* game ;
	CL_Slot mouseup;
	CL_Slot mousedown;
	CL_Slot mousemove; 
	CL_Slot keydown; 
	CL_Slot keyup; 
	std::list<CGlumolKeyDown*> downlist;

	CGlumolInputDispatcher(CGlumolGame* app);
	virtual ~CGlumolInputDispatcher();
	
	void DispatchRepeat(); 

	void OnMouseButtonDown(const CL_InputEvent &key); 
	void OnMouseButtonUp(const CL_InputEvent &key); 
	void OnMouseMove(const CL_InputEvent &key); 
	void OnKeyDown(const CL_InputEvent &key); 
	void OnKeyUp(const CL_InputEvent &key); 
	void SetCursor(CGlumolSprite& cursor);
	void SetMousePos(int x,int y); 
	void Kill(const CL_InputEvent& key); 
};

#endif
