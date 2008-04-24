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

using namespace boost::python;

struct CL_CheckBox_wrap : public CL_CheckBox
{
	CL_CheckBox_wrap(PyObject* self_, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_CheckBox(parent, style),
		self(self_)
	{
		// init_slots();
	}

	PyObject* self;
};

void export_checkbox()
{
	class_<CL_CheckBox, bases<CL_Button>, CL_CheckBox_wrap, boost::noncopyable>("CheckBox", init< CL_Component*, optional<CL_StyleManager*> >())
		// .def(init<const CL_Point&, const char *, CL_Component*, optional<CL_StyleManager*> >())
		// .def(init<const char *, CL_Component*, optional<CL_StyleManager*> >())
		// .def(init<const CL_Point&, const std::string&, CL_Component*, CL_StyleManager*>())
		// .def(init<CL_Component*, optional<CL_StyleManager*> >())
		.def("is_checked", &CL_CheckBox::is_checked)
		.def("set_checked", &CL_CheckBox::set_checked)
		.add_property("checked", &CL_CheckBox::is_checked, &CL_CheckBox::set_checked);
}
