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
#include <boost/python.hpp>
#include <ClanLib/display.h>
#include <ClanLib/gui.h>
#include <iostream>
#include "glumolgame.h"

extern CGlumolGame *CurrentGame;
CL_GUIManager *Gui = NULL;

using namespace boost::python;

struct CL_GUIManager_wrap : public CL_GUIManager
{
	CL_GUIManager_wrap(PyObject* self_, CL_StyleManager* style)
		: CL_GUIManager(style), self(self_)
	{
		//slot_paint = sig_paint().connect(this, &CL_GUIManager_wrap::on_paint);
		if(CurrentGame) {
                        for(int i = 0; i < 100; i++)
                            std::cout << "add_gui" << std::endl;
			CurrentGame->add_gui(this);
		}
	}

	~CL_GUIManager_wrap()
	{
		if(CurrentGame) {
			CurrentGame->remove_gui(this);
		}
	}

	CL_GUIManager_wrap(CL_StyleManager* style)
		: CL_GUIManager(style)
	{
		//Gui = this;
		//slot_paint = sig_paint().connect(this, &CL_GUIManager_wrap::on_paint);
	}

	void on_paint()
	{
		//CL_Display::clear(CL_Color(150, 150, 150));
	}

	CL_Slot slot_paint;
	PyObject* self;
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	run_overloads, run, 0, 1)
        
void export_gui_manager()
{
	class_<CL_GUIManager, CL_GUIManager_wrap, bases<CL_Component>, boost::noncopyable>("GUIManager", init<CL_StyleManager*>())
		.def("run", &CL_GUIManager::run, run_overloads())
		.def("quit", &CL_GUIManager::quit)
	;
}
