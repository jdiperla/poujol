#include "stdafx.h"
#include <boost/python.hpp>
#include <ClanLib/gui.h>
#include <ClanLib/core.h>

using namespace boost::python;

struct CL_MenuNode_wrap: public CL_MenuNode
{
	CL_MenuNode_wrap(PyObject* self_, CL_Menu* parent_menu, CL_StyleManager *style = NULL) :
		CL_MenuNode(parent_menu, style),
                self(self_)
	{
		init_slots();
	}

	void init_slots()
	{
		CL_Slot s1 = sig_reposition().connect(this, &CL_MenuNode_wrap::on_reposition);
		CL_Slot s2 = sig_clicked().connect(this, &CL_MenuNode_wrap::on_clicked);
	}

	void on_clicked() { call_method<void>(self, "on_clicked"); }
	void default_on_clicked() {}
	void on_reposition() { call_method<void>(self, "on_reposition"); }
	void default_on_reposition() {}

	PyObject* self;
};

void export_menu_node()
{
	class_<CL_MenuNode, bases<CL_Component>, CL_MenuNode_wrap, boost::noncopyable> CL_MenuNode_wrapper("MenuNode", init<CL_Menu*, optional<CL_StyleManager*> >());
	CL_MenuNode_wrapper
		.def("get_data", &CL_MenuNode::get_data, return_value_policy<reference_existing_object>())
		.add_property("has_mouse_over", &CL_MenuNode::has_mouse_over)
		.add_property("has_mouse_in_submenus", &CL_MenuNode::has_mouse_in_submenus)
		.add_property("has_submenu", &CL_MenuNode::has_submenu)
		.add_property("has_open_submenu", &CL_MenuNode::has_open_submenu)
		.def("get_submenu", &CL_MenuNode::get_submenu, return_value_policy<reference_existing_object>())
		.def("set_submenu", &CL_MenuNode::set_submenu)
		.add_property("submenu",
			CL_MenuNode_wrapper.attr("get_submenu"),
			CL_MenuNode_wrapper.attr("set_submenu"))
		.def("get_parent_menu", &CL_MenuNode::get_parent_menu, return_value_policy<reference_existing_object>())
		.def("set_parent_menu", &CL_MenuNode::set_parent_menu)
		.add_property("parent_menu",
			CL_MenuNode_wrapper.attr("get_parent_menu"),
			CL_MenuNode_wrapper.attr("set_parent_menu"))
		.add_property("name", &CL_MenuNode::get_name)
		.def("set_close_on_exit", &CL_MenuNode::set_close_on_click)
		.def("collapse", &CL_MenuNode::collapse)
		.def("open_submenu", &CL_MenuNode::open_submenu)
		.def("reposition_submenus", &CL_MenuNode::reposition_submenus)
		.add_property("name", &CL_MenuNode::get_name, &CL_MenuNode::set_name)
		.def("on_reposition", &CL_MenuNode_wrap::on_reposition)
		.def("on_clicked", &CL_MenuNode_wrap::on_clicked)
	;
}

