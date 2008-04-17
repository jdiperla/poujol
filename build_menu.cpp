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
#include "component_derived.h"

using namespace boost::python;

static CL_Point to_point(const CL_Pointf &pt)
{
	return CL_Point(int(pt.x), int(pt.y));
}

struct CL_Menu_wrap: public CL_Menu, public Component_derived
{
	CL_Menu_wrap(PyObject* self_, CL_Component *parent, CL_StyleManager *style = NULL, bool vertical = false) :
		CL_Menu(parent, style, vertical),
		Component_derived(self_)
	{
		init_slots_2();
	}

	CL_Menu_wrap(PyObject* self_, CL_MenuNode* parent_node, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_Menu(parent_node, parent, style),
		Component_derived(self_)
	{
		init_slots_2();
	}

	CL_Menu_wrap(PyObject* self_, const CL_Pointf& point, CL_Component *parent, CL_StyleManager *style = NULL, bool vertical = false) :
		CL_Menu(to_point(point), parent, style, vertical),
		Component_derived(self_)
	{
		init_slots_2();
	}

	void init_slots_2()
	{
		CL_Slot s1 = sig_reposition().connect(this, &CL_Menu_wrap::on_reposition);
		init_slots(this, this);
	}

	/*
    CL_Menu *create_menu(
		const char *path,
		const char *labels)
	{
		return CL_Menu::create_menu(path, labels);
	}
	
	CL_Menu_wrap(PyObject* self_, const CL_Rect &rect, CL_Component *parent, CL_StyleManager *style = NULL, bool vertical = false) :
		self(self_),
		CL_Menu(rect, parent, style, vertical) { init_slots(); }
    */

	void on_reposition(int x, int y) { call_method<void>(self, "on_reposition", x, y); }
	void default_on_reposition(int x, int y) {}
};

void export_menu()
{
	void (CL_Menu::*fx1)(int, int) = &CL_Menu::reposition;
	void (CL_Menu::*fx2)() = &CL_Menu::reposition;
	
	class_<CL_Menu, bases<CL_Component, Component_derived>, CL_Menu_wrap, boost::noncopyable> CL_Menu_wrapper("Menu", init<CL_MenuNode*, CL_Component*, CL_StyleManager*>());
	CL_Menu_wrapper
		.def(init<const CL_Pointf&, CL_Component*, optional<CL_StyleManager*, bool> >())
        // .def(init<const CL_Rect&, CL_Component*, optional<CL_StyleManager*, bool> >())
		.def(init<CL_Component*, optional<CL_StyleManager*, bool> >())
		.add_property("is_open", &CL_Menu::is_open)
		.def("open", &CL_Menu::open)
		.add_property("vertical", &CL_Menu::is_vertical, &CL_Menu::set_vertical)
		.add_property("root_collapsing", &CL_Menu::is_root_collapsing, &CL_Menu::set_root_collapsing)
		.add_property("has_mouse_over", &CL_Menu::has_mouse_over)
		.add_property("has_mouse_in_submenus", &CL_Menu::has_mouse_in_submenus)
		.def("get_parent_node", &CL_Menu::get_parent_node, return_value_policy<reference_existing_object>())
		.add_property("parent_node",
			CL_Menu_wrapper.attr("get_parent_node"))
		.def("get_root_menu", &CL_Menu::get_root_menu, return_value_policy<reference_existing_object>())
		.add_property("root_menu",
			CL_Menu_wrapper.attr("get_root_menu"))
		.def("get_node_list", &CL_Menu::get_node_list, return_value_policy<copy_non_const_reference>())
		.add_property("node_list",
			CL_Menu_wrapper.attr("get_node_list"))
		.add_property("items_height", &CL_Menu::get_items_height)
		.add_property("items_width", &CL_Menu::get_items_height)
		.add_property("is_click_to_open", &CL_Menu::is_click_to_open)
		.def("reposition", fx1)
		.def("reposition", fx2)
		.def("add_node", &CL_Menu::add_node)
		.def("remove_node", &CL_Menu::remove_node)
		.def("collapse", &CL_Menu::collapse)
		.def("collapse_submenus", &CL_Menu::collapse_submenus)
		.def("create_menu", &CL_Menu_wrap::create_menu, return_value_policy<manage_new_object>())
		.def("create_node", &CL_Menu::create_node, return_value_policy<manage_new_object>())
		.def("create_item", &CL_Menu::create_item, return_value_policy<manage_new_object>())
		.def("create_toggle_item", &CL_Menu::create_toggle_item, return_value_policy<manage_new_object>())
		.def("on_reposition", &CL_Menu_wrap::on_reposition)
	;
}


