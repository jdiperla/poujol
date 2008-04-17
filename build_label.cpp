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

void export_label()
{
	void (CL_Label::*fx1)(const std::string&) = &CL_Label::set_text;
	void (CL_Label::*fx2)(int) = &CL_Label::set_text;
	void (CL_Label::*fx3)(double) = &CL_Label::set_text;

	class_<CL_Label, bases<CL_Component>, boost::noncopyable> CL_Label_wrapper("Label", init<CL_Component*, CL_StyleManager*>());
	CL_Label_wrapper
		.def(init<const char*, CL_Component*, CL_StyleManager*>())
		.def(init<const CL_Point&, const char*, CL_Component*, CL_StyleManager*>())
		.def(init<const CL_Rect&, const char*, CL_Component*, CL_StyleManager*>())

		.def("get_text", &CL_Label::get_text, return_value_policy<copy_const_reference>())
		.def("set_text", fx1)
		.def("set_text", fx2)
		.def("set_text", fx3)
		.def("clear", &CL_Label::clear)
		.add_property("alignment", &CL_Label::get_alignment, &CL_Label::set_alignment)
	;
}
