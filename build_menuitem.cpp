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
#include <ClanLib/gui.h>
#include <ClanLib/core.h>

using namespace boost::python;

void export_menu_item()
{
	void (CL_MenuItem::*fx1)(const std::string&) = &CL_MenuItem::set_text;
	void (CL_MenuItem::*fx2)(int) = &CL_MenuItem::set_text;
	void (CL_MenuItem::*fx3)(double) = &CL_MenuItem::set_text;
	
	class_<CL_MenuItem, bases<CL_Component>, boost::noncopyable> CL_MenuItem_wrapper("MenuItem", init<const char*, CL_Component*, optional<CL_StyleManager*> >());
	CL_MenuItem_wrapper
		.add_property("selected", &CL_MenuItem::is_selected, &CL_MenuItem::set_selected)
		.add_property("toggling", &CL_MenuItem::is_toggling, &CL_MenuItem::set_toggling)
		.add_property("use_icon", &CL_MenuItem::use_icon, &CL_MenuItem::set_use_icon)
		.def("get_text", &CL_MenuItem::get_text, return_value_policy<copy_const_reference>())
		.def("set_text", fx1)
		.def("set_text", fx2)
		.def("set_text", fx3)
		.def("clear", &CL_MenuItem::clear)
	;
}
