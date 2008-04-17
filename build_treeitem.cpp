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

struct CL_TreeItem_wrap: public CL_TreeItem
{
	CL_TreeItem_wrap(PyObject* self_, CL_TreeNode* node, CL_Component* parent, CL_StyleManager* style = NULL) :
		CL_TreeItem(node, parent, style),
                self(self_)
	{
		init_slots();
	}

	void init_slots()
	{
		CL_Slot s1 = sig_clicked().connect(this, &CL_TreeItem_wrap::on_clicked);
	}

	void on_clicked() { call_method<void>(self, "on_clicked"); }
	void default_on_clicked() {}

	PyObject* self;
};

void export_tree_item()
{
	class_<CL_TreeItem, bases<CL_Component>, CL_TreeItem_wrap, boost::noncopyable> CL_TreeItem_wrapper("TreeItem", init<CL_TreeNode*, CL_Component*, CL_StyleManager*>());
	CL_TreeItem_wrapper
		.def("get_text", &CL_TreeItem::get_text, return_value_policy<copy_const_reference>())
		.def("set_text", &CL_TreeItem::set_text)
//		.def("set_checkbox", &CL_TreeItem::set_, return_value_policy<reference_existing_object>())
//		.def("get_component", &CL_TreeItem::get_component, return_value_policy<reference_existing_object>())
		.def("get_node", &CL_TreeItem::get_node, return_value_policy<reference_existing_object>())
		.add_property("node",
			CL_TreeItem_wrapper.attr("get_node"))
		.add_property("text_margin", &CL_TreeItem::get_text_margin, &CL_TreeItem::set_text_margin)
		.add_property("custom_height", &CL_TreeItem::get_custom_height, &CL_TreeItem::set_custom_height)
		.def("on_clicked", &CL_TreeItem_wrap::on_clicked)
	;
}


