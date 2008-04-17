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

void export_input_dialog()
{
	class_<CL_InputDialog, bases<CL_Window>, boost::noncopyable> ("InputDialog",
		init<const char*, const char*, const char*, const char*,
			 CL_Component*, CL_StyleManager*>())
		.def("add_input_box", &CL_InputDialog::add_input_box, return_value_policy<reference_existing_object>())
		.def("add_check_box", &CL_InputDialog::add_check_box, return_value_policy<reference_existing_object>())
		.add_property("result_button", &CL_InputDialog::get_result_button)
	;
}
