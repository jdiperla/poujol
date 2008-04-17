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

struct CL_RadioGroup_wrap: public CL_RadioGroup
{
	/*CL_RadioGroup_wrap(PyObject* self_) :
		self(self_)
	{
		init_slots();
	}

	void init_slots()
	{
		CL_Slot s1 = sig_selection_changed().connect(this, &CL_RadioGroup_wrap::on_selection_changed);
	}

	void on_selection_changed(CL_RadioButton *button) { call_method<void>(self, "on_selection_changed", boost::ref(button)); }
	void default_on_selection_changed(CL_RadioButton *button) {}*/

	PyObject* self;
};

void export_radio_group()
{
	/*class_<CL_RadioGroup, CL_RadioGroup_wrap, boost::noncopyable>("RadioGroup")
		//.def("get_toggled", &CL_RadioGroup::get_toggled, return_value_policy<reference_existing_object>())
		//.def("get_buttons", &CL_RadioGroup::get_buttons, return_value_policy<copy_const_reference>())
		.def("add", &CL_RadioGroup::add)
		.def("remove", &CL_RadioGroup::remove)
		.add_property("count", &CL_RadioGroup::get_count)
		//.def("on_selection_changed", &CL_RadioGroup_wrap::on_selection_changed)
;*/
}
