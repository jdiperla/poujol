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

struct CL_TreeNode_wrap : public CL_TreeNode
{
	CL_TreeNode_wrap(PyObject* self_, CL_TreeNode* tn, CL_TreeView* tv) :
		CL_TreeNode(tn, tv),
                self(self_)
		{ }
	
	CL_TreeItem* insert_item2(
		const char* l1, const char* l2 = "",
		const char* l3 = "", const char* l4 = "",
		const char* l5 = "", const char* l6 = "",
		const char* l7 = "", const char* l8 = "")
	{
		return CL_TreeNode::insert_item(l1, l2, l3, l4, l5,
			l6, l7, l8);
	}

	CL_TreeNode* insert_item3(
		CL_Component* label1, CL_Component* label2 = NULL,
		CL_Component* label3 = NULL,
		CL_Component* label4 = NULL,
		CL_Component* label5 = NULL,
		CL_Component* label6 = NULL,
		CL_Component* label7 = NULL,
		CL_Component* label8 = NULL)
	{
		return CL_TreeNode::insert_item(label1, label2, label3, label4, label5,
			label6, label8, label8);
	}
	PyObject* self;
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	insert_item_overloads, insert_item2, 1, 8)

void export_tree_node()
{
	CL_TreeItem* (CL_TreeNode::*fx1)(
		const std::string&, const std::string&, const std::string&,
		const std::string&, const std::string&, const std::string&,
		const std::string&, const std::string&) = &CL_TreeNode::insert_item;
	
/*	CL_TreeItem* (CL_TreeNode_wrap::*fx3)(
		const std::string&, const std::string&, const std::string&,
		const std::string&, const std::string&, const std::string&,
		const std::string&, const std::string&) = &CL_TreeNode_wrap::insert_item2;
*/
	CL_TreeNode* (CL_TreeNode::*fx2)(
		CL_Component*, CL_Component*, CL_Component*,
		CL_Component*, CL_Component*, CL_Component*,
		CL_Component*, CL_Component*) = &CL_TreeNode::insert_item;
	
	class_<CL_TreeNode, CL_TreeNode_wrap, boost::noncopyable> CL_TreeNode_wrapper("TreeNode", init<CL_TreeNode*, CL_TreeView*>());
	CL_TreeNode_wrapper
		.add_property("selectable", &CL_TreeNode::is_selectable, &CL_TreeNode::set_selectable)
		//.add_property("selected", &CL_TreeNode::is_selected, &CL_TreeNode::set_selected)
		.add_property("collapsable", &CL_TreeNode::is_collapsable, &CL_TreeNode::set_collapsable)
		.add_property("collapsed", &CL_TreeNode::is_collapsed, &CL_TreeNode::set_collapsed)
		.add_property("has_children", &CL_TreeNode::has_children)
		.add_property("is_root", &CL_TreeNode::is_root)
		.def("get_current_item", &CL_TreeNode::get_current_item, return_value_policy<reference_existing_object>())
		.add_property("current_item",
			CL_TreeNode_wrapper.attr("get_current_item"))
//		.def("get_user_data", &CL_TreeNode::get_userdata, return_value_policy<return_opaque_pointer>())
//		.add_property("user_data",
//			CL_TreeNode_wrapper.attr("get_user_data"))
		.def("get_component", &CL_TreeNode::get_component, return_value_policy<reference_existing_object>())
		.add_property("component",
			CL_TreeNode_wrapper.attr("get_component"))
		.def("get_treeview", &CL_TreeNode::get_treeview, return_value_policy<reference_existing_object>())
		.add_property("treeview",
			CL_TreeNode_wrapper.attr("get_treeview"))
		.add_property("get_placement_offset", &CL_TreeNode::get_placement_offset)
		.add_property("get_items_height", &CL_TreeNode::get_items_height)
		.add_property("get_items_height", &CL_TreeNode::get_items_height)
		.def("insert_item", fx1, return_value_policy<reference_existing_object>())
		.def("insert_item", fx2, return_value_policy<reference_existing_object>())
		.def("insert_item2", &CL_TreeNode_wrap::insert_item2,
			insert_item_overloads("insert_item2")[
				return_value_policy<reference_existing_object>()])
	//	.def("insert_item", insert_item_overloads(), return_value_policy<reference_existing_object>())
//		.def("insert_item", fx3, insert_item_overloads("xaca")[return_value_policy<reference_existing_object>()]) 
		.def("remove_item", &CL_TreeNode::remove_item)
		.def("clear_selection", &CL_TreeNode::clear_selection)
		// .def("invert_selection", &CL_TreeNode::invert_selection)
		.def("clear", &CL_TreeNode::clear)
	;
}


