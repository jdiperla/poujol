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


#ifndef __COMPONENT_DERIVED__
#define __COMPONENT_DERIVED__

#include <boost/python/call.hpp>
#include "component_derived.h"

using namespace boost::python;
class Component_derived
{
public:
	Component_derived(PyObject *self_)
		: self(self_) {
	}

	Component_derived() {
	}

	void on_paint()
	{
		call_method<void>(self, "on_paint");
	}

	void default_on_paint()
	{
	}

	void on_paint_children()
	{
		call_method<void>(self, "on_paint_children");
	}

	void default_on_paint_children()
	{
	}

	void on_begin_paint()
	{
		call_method<void>(self, "on_begin_paint");
	}

	void default_on_begin_paint()
	{
	}

	void on_end_paint()
	{
		call_method<void>(self, "on_end_paint");
	}

	void default_on_end_paint()
	{
	}

	void on_mouse_down(const CL_InputEvent& event)
	{
		call_method<void>(self, "on_mouse_down", event);
	}

	void default_on_mouse_down(const CL_InputEvent&)
	{
	}

	void on_mouse_up(const CL_InputEvent& event)
	{
		call_method<void>(self, "on_mouse_up", event);
	}

	void default_on_mouse_up(const CL_InputEvent&)
	{
	}

	void on_mouse_enter()
	{
		call_method<void>(self, "on_mouse_enter");
	}

	void default_on_mouse_enter()
	{
	}

	void on_mouse_leave()
	{
		call_method<void>(self, "on_mouse_leave");
	}

	void default_on_mouse_leave()
	{
	}

	void on_key_down(const CL_InputEvent& event)
	{
		call_method<void>(self, "on_key_down", event);
	}

	void default_on_key_down(const CL_InputEvent&)
	{
	}

	void on_key_up(const CL_InputEvent& event)
	{
		call_method<void>(self, "on_key_up", event);
	}

	void default_on_key_up(const CL_InputEvent&)
	{
	}

	void on_set_options(const CL_DomElement& elem)
	{
		call_method<void>(self, "on_set_options", elem);
	}

	void default_on_set_options(const CL_DomElement&)
	{
	}

	void on_got_focus()
	{
		call_method<void>(self, "on_got_focus");
	}

	void default_on_got_focus()
	{
	}

	void on_lose_focus()
	{
		call_method<void>(self, "on_lose_focus");
	}

	void default_on_lose_focus()
	{
	}

	void on_close()
	{
		call_method<void>(self, "on_close");
	}

	void default_on_close()
	{
	}

	void on_move(int x, int y)
	{
		call_method<void>(self, "on_move", x, y);
	}

	void default_on_move(int x, int y)
	{
	}

	void on_resize(int x, int y)
	{
		call_method<void>(self, "on_resize", x, y);
	}

	void default_on_resize(int x, int y)
	{
	}

	void on_child_before_add(CL_Component* component)
	{
		call_method<void>(self, "on_child_before_add", component);
	}

	void default_on_child_before_add(CL_Component* component)
	{
	}

	void on_child_add(CL_Component* component)
	{
		call_method<void>(self, "on_child_add", component);
	}

	void default_on_child_add(CL_Component* component)
	{
	}

	void on_child_before_remove(CL_Component* component)
	{
		call_method<void>(self, "on_child_before_remove", component);
	}

	void default_on_child_before_remove(CL_Component* component)
	{
	}

	void on_child_remove(CL_Component* component)
	{
		call_method<void>(self, "on_child_remove", component);
	}

	void default_on_child_remove(CL_Component* component)
	{
	}

	void on_transform_coords(int& x, int& y)
	{
		call_method<void>(self, "on_transform_coords", x, y);
	}

	void default_on_transform_coords(int&, int&)
	{
	}

	void on_get_minimum_size(CL_Size &size)
	{
		call_method<void>(self, "on_get_minimum_size", size);
	}

	void default_on_get_minimum_size(CL_Size &size)
	{
	}

	void on_get_maximum_size(CL_Size &size)
	{
		call_method<void>(self, "on_get_maximum_size", size);
	}

	void default_on_get_maximum_size(CL_Size &size)
	{
	}

	void on_get_preferred_size(CL_Size &size)
	{
		call_method<void>(self, "on_get_preferred_size", size);
	}

	void default_on_get_preferred_size(CL_Size &size)
	{
	}

	PyObject* self;

	CL_Slot slot_paint,
		slot_paint_children,
		slot_begin_paint,
		slot_end_paint,
		slot_mouse_down,
		slot_mouse_up,
		slot_key_down,
		slot_key_up,
		slot_set_options,
		slot_mouse_enter,
		slot_mouse_leave,
		slot_got_focus,
		slot_lost_focus,
		slot_close,
		slot_resize,
		slot_move,
		slot_child_before_add,
		slot_child_before_remove,
		slot_child_add,
		slot_child_remove,
		slot_transform_coords,
		slot_get_minimum_size,
		slot_get_maximum_size,
		slot_get_preferred_size;
};

void init_slots(Component_derived *compod, CL_Component *component);

#endif
