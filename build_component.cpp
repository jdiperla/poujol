
#include "stdafx.h"
#include <boost/python.hpp>
#include <ClanLib/display.h>
#include <ClanLib/gui.h>
#include <iostream>
#include "component_derived.h"

using namespace boost::python;

struct CL_Component_wrap : public CL_Component, public Component_derived
{
	CL_Component_wrap(PyObject* self_, CL_Component* parent, CL_StyleManager *style = NULL)
		: CL_Component(parent, style), Component_derived(self_)
	{
		/*slot_paint = sig_paint().connect(this, &CL_Component_wrap::on_paint);
		slot_paint_children = sig_paint_children().connect(this, &CL_Component_wrap::on_paint_children);
		slot_begin_paint = sig_begin_paint().connect(this, &CL_Component_wrap::on_begin_paint);
		slot_end_paint = sig_end_paint().connect(this, &CL_Component_wrap::on_end_paint);*/
		init_slots(this, this);
	}

	CL_Component_wrap(PyObject* self_, const CL_Rect &rect, CL_Component* parent, CL_StyleManager *style = NULL)
		: CL_Component(rect, parent, style), Component_derived(self_)
	{
		init_slots(this, this);
	}

/*	void init_slots_2()
	{
		slot_paint = sig_paint().connect(this, &CL_Component_wrap::on_paint);
		slot_paint_children = sig_paint_children().connect(this, &CL_Component_wrap::on_paint_children);
		slot_begin_paint = sig_begin_paint().connect(this, &CL_Component_wrap::on_begin_paint);
		slot_end_paint = sig_end_paint().connect(this, &CL_Component_wrap::on_end_paint);
		slot_mouse_down = sig_mouse_down().connect(this, &CL_Component_wrap::on_mouse_down);
		slot_mouse_up = sig_mouse_up().connect(this, &CL_Component_wrap::on_mouse_up);
		slot_mouse_enter = sig_mouse_enter().connect(this, &CL_Component_wrap::on_mouse_enter);
		slot_mouse_leave = sig_mouse_leave().connect(this, &CL_Component_wrap::on_mouse_leave);
		slot_got_focus = sig_got_focus().connect(this, &CL_Component_wrap::on_got_focus);
		slot_lost_focus = sig_lost_focus().connect(this, &CL_Component_wrap::on_lose_focus);
		slot_key_down = sig_key_down().connect(this, &CL_Component_wrap::on_key_down);
		slot_key_up = sig_key_up().connect(this, &CL_Component_wrap::on_key_up);
		slot_set_options = sig_set_options().connect(this, &CL_Component_wrap::on_set_options);
		slot_close = sig_close().connect(this, &CL_Component_wrap::on_close);
		slot_resize = sig_resize().connect(this, &CL_Component_wrap::on_resize);
		slot_move = sig_move().connect(this, &CL_Component_wrap::on_move);
		slot_child_before_add = sig_child_before_add().connect(this, &CL_Component_wrap::on_child_before_add);
		slot_child_before_remove = sig_child_before_remove().connect(this, &CL_Component_wrap::on_child_before_remove);
		slot_child_add = sig_child_add().connect(this, &CL_Component_wrap::on_child_add);
		slot_child_remove = sig_child_remove().connect(this, &CL_Component_wrap::on_child_remove);
		slot_transform_coords = sig_transform_coords().connect(this, &CL_Component_wrap::on_transform_coords);
		slot_get_minimum_size = sig_get_minimum_size().connect(this, &CL_Component_wrap::on_get_minimum_size);
		slot_get_maximum_size = sig_get_maximum_size().connect(this, &CL_Component_wrap::on_get_maximum_size);
		slot_get_preferred_size = sig_get_preferred_size().connect(this, &CL_Component_wrap::on_get_preferred_size);
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
*/
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	add_child_overloads, add_child, 1, 2)

void export_component()
{
	class_<Component_derived, boost::noncopyable> Component_derived_wrapper("Component_derived", init<>());
	class_<CL_Component, bases<Component_derived>, CL_Component_wrap, boost::noncopyable> CL_Component_wrapper("Component", init<CL_Component*, optional<CL_StyleManager*> >());
	CL_Component_wrapper
		.def(init<const CL_Rect&, CL_Component*, optional<CL_StyleManager*> >())
		.def("get_client_area", &CL_Component::get_client_area, return_value_policy<reference_existing_object>())
		.def("set_client_area", &CL_Component::set_client_area)
		.add_property("client_area",
			CL_Component_wrapper.attr("get_client_area"),
			CL_Component_wrapper.attr("set_client_area"))
		.def("get_parent", &CL_Component::get_parent, return_value_policy<reference_existing_object>())
		.def("set_parent", &CL_Component::set_parent)
		.add_property("parent",
			CL_Component_wrapper.attr("get_parent"),
			CL_Component_wrapper.attr("set_parent"))
		.def("get_root_parent", &CL_Component::get_root_parent, return_value_policy<reference_existing_object>())
		.add_property("root_parent",
			CL_Component_wrapper.attr("get_root_parent"))
		.def("get_gui_manager", &CL_Component::get_gui_manager, return_value_policy<reference_existing_object>())
		.def("set_gui_manager", &CL_Component::set_gui_manager)
		.add_property("gui_manager",
			CL_Component_wrapper.attr("get_gui_manager"),
			CL_Component_wrapper.attr("set_gui_manager"))
		.def("get_style_manager", &CL_Component::get_style_manager, return_value_policy<reference_existing_object>())
		.add_property("style_manager",
			CL_Component_wrapper.attr("get_style_manager"))
		.add_property("width", &CL_Component::get_width, &CL_Component::set_width)
		.add_property("height", &CL_Component::get_height, &CL_Component::set_height)
		.def("get_screen_rect", &CL_Component::get_screen_rect)
		.add_property("screen_rect",
			CL_Component_wrapper.attr("get_screen_rect"))
		.def("get_position", &CL_Component::get_position, return_value_policy<copy_const_reference>())
		.add_property("position",
			CL_Component_wrapper.attr("get_position"))
		.add_property("minimum_size", &CL_Component::get_minimum_size, &CL_Component::set_minimum_size)
		.add_property("maximum_size", &CL_Component::set_maximum_size, &CL_Component::set_maximum_size)
		.add_property("preferred_size", &CL_Component::get_preferred_size, &CL_Component::set_preferred_size)
		.def("get_vertical_policy", &CL_Component::get_vertical_policy, return_value_policy<copy_const_reference>())
		.def("set_vertical_policy", &CL_Component::set_vertical_policy)
		.add_property("vertical_policy",
			CL_Component_wrapper.attr("get_vertical_policy"),
			CL_Component_wrapper.attr("set_vertical_policy"))
		.def("get_horizontal_policy", &CL_Component::get_horizontal_policy, return_value_policy<copy_const_reference>())
		.def("set_horizontal_policy", &CL_Component::set_horizontal_policy)
		.add_property("horizontal_policy",
			CL_Component_wrapper.attr("get_horizontal_policy"),
			CL_Component_wrapper.attr("set_horizontal_policy"))
		.def("get_layout", &CL_Component::get_layout, return_value_policy<copy_const_reference>())
		.def("set_layout", &CL_Component::set_layout)
		.add_property("layout",
			CL_Component_wrapper.attr("get_layout"),
			CL_Component_wrapper.attr("set_layout"))
		.add_property("event_passing", &CL_Component::get_event_passing, &CL_Component::set_event_passing)
		.def("has_child", &CL_Component::has_child)
		.def("get_children", &CL_Component::get_children, return_value_policy<copy_const_reference>())
		.add_property("children",
			CL_Component_wrapper.attr("get_children"))
		.add_property("children_rect", &CL_Component::get_children_rect)
		.def("get_component_at", &CL_Component::get_component_at, return_value_policy<reference_existing_object>())
		.def("has_focus", &CL_Component::has_focus)
		.def("set_focus", &CL_Component::set_focus)
		.add_property("focus",
			CL_Component_wrapper.attr("has_focus"),
			CL_Component_wrapper.attr("set_focus"))
		.def("get_current_focus", &CL_Component::get_focus, return_value_policy<reference_existing_object>())
		.add_property("current_focus",
			CL_Component_wrapper.attr("get_current_focus"))
		.add_property("focusable", &CL_Component::is_focusable, &CL_Component::set_focusable)
		.add_property("has_mouse_over", &CL_Component::has_mouse_over)
		.add_property("visible", &CL_Component::is_visible, &CL_Component::show)
		.add_property("enabled", &CL_Component::is_enabled, &CL_Component::enable)
		.add_property("mouse_captured", &CL_Component::is_mouse_captured)
		.add_property("is_modal", &CL_Component::is_modal)
		.add_property("has_modal_parent", &CL_Component::has_modal_parent)
		.add_property("is_topmost", &CL_Component::is_topmost, &CL_Component::set_topmost_flag)
		.def("get_style", &CL_Component::get_style, return_value_policy<reference_existing_object>())
		.def("set_style", &CL_Component::set_style)
		.add_property("style",
			CL_Component_wrapper.attr("get_style"),
			CL_Component_wrapper.attr("set_style"))
		.add_property("tab_id", &CL_Component::get_tab_id, &CL_Component::set_tab_id)
		.def("set_size", &CL_Component::set_size)
		.def("raise", &CL_Component::raise)
		.def("lower", &CL_Component::lower)
		.def("update", &CL_Component::update)
		.def("paint", &CL_Component::paint)
		.def("run", &CL_Component::run)
		.def("quit", &CL_Component::quit)
		.def("close", &CL_Component::close)
		.def("add_child", &CL_Component::add_child, add_child_overloads())
		.def("remove_child", &CL_Component::remove_child)
		.def("find_preferred_size", &CL_Component::find_preferred_size)
		.def("find_tab_id", &CL_Component::find_tab_id, return_value_policy<reference_existing_object>())
		.def("on_paint", &CL_Component_wrap::default_on_paint)
		.def("on_paint_children", &CL_Component_wrap::default_on_paint_children)
		.def("on_begin_paint", &CL_Component_wrap::default_on_begin_paint)
		.def("on_end_paint", &CL_Component_wrap::default_on_end_paint)
		.def("on_mouse_down", &CL_Component_wrap::default_on_mouse_down)
		.def("on_mouse_up", &CL_Component_wrap::default_on_mouse_up)

		.def("on_mouse_enter", &CL_Component_wrap::default_on_mouse_enter)
		.def("on_mouse_leave", &CL_Component_wrap::default_on_mouse_leave)
		.def("on_key_down", &CL_Component_wrap::default_on_key_down)
		.def("on_key_up", &CL_Component_wrap::default_on_key_up)
		.def("on_mouse_up", &CL_Component_wrap::default_on_mouse_up)
		.def("on_set_options", &CL_Component_wrap::default_on_set_options)
		.def("on_got_focus", &CL_Component_wrap::default_on_got_focus)
		.def("on_lose_focus", &CL_Component_wrap::default_on_lose_focus)
		.def("on_close", &CL_Component_wrap::default_on_close)
		.def("on_move", &CL_Component_wrap::default_on_move)
		.def("on_resize", &CL_Component_wrap::default_on_resize)
		.def("on_child_before_add", &CL_Component_wrap::default_on_child_before_add)
		.def("on_child_add", &CL_Component_wrap::default_on_child_add)
		.def("on_child_before_remove", &CL_Component_wrap::default_on_child_before_remove)
		.def("on_child_remove", &CL_Component_wrap::default_on_child_remove)
		.def("on_transform_coords", &CL_Component_wrap::default_on_transform_coords)
		.def("on_get_minimum_size", &CL_Component_wrap::default_on_get_minimum_size)
		.def("on_get_maximum_size", &CL_Component_wrap::default_on_get_maximum_size)
		.def("on_get_preferred_size", &CL_Component_wrap::default_on_get_preferred_size)

	;
}

void init_slots(Component_derived *compod, CL_Component *component)
{
	compod->slot_paint = component->sig_paint().connect(compod, &Component_derived::on_paint);
	compod->slot_paint_children = component->sig_paint_children().connect(compod, &Component_derived::on_paint_children);
	compod->slot_begin_paint = component->sig_begin_paint().connect(compod, &Component_derived::on_begin_paint);
	compod->slot_end_paint = component->sig_end_paint().connect(compod, &Component_derived::on_end_paint);
	compod->slot_mouse_down = component->sig_mouse_down().connect(compod, &Component_derived::on_mouse_down);
	compod->slot_mouse_up = component->sig_mouse_up().connect(compod, &Component_derived::on_mouse_up);
	compod->slot_mouse_enter = component->sig_mouse_enter().connect(compod, &Component_derived::on_mouse_enter);
	compod->slot_mouse_leave = component->sig_mouse_leave().connect(compod, &Component_derived::on_mouse_leave);
	compod->slot_got_focus = component->sig_got_focus().connect(compod, &Component_derived::on_got_focus);
	compod->slot_lost_focus = component->sig_lost_focus().connect(compod, &Component_derived::on_lose_focus);
	compod->slot_key_down = component->sig_key_down().connect(compod, &Component_derived::on_key_down);
	compod->slot_key_up = component->sig_key_up().connect(compod, &Component_derived::on_key_up);
	compod->slot_set_options = component->sig_set_options().connect(compod, &Component_derived::on_set_options);
	compod->slot_close = component->sig_close().connect(compod, &Component_derived::on_close);
	compod->slot_resize = component->sig_resize().connect(compod, &Component_derived::on_resize);
	compod->slot_move = component->sig_move().connect(compod, &Component_derived::on_move);
	compod->slot_child_before_add = component->sig_child_before_add().connect(compod, &Component_derived::on_child_before_add);
	compod->slot_child_before_remove = component->sig_child_before_remove().connect(compod, &Component_derived::on_child_before_remove);
	compod->slot_child_add = component->sig_child_add().connect(compod, &Component_derived::on_child_add);
	compod->slot_child_remove = component->sig_child_remove().connect(compod, &Component_derived::on_child_remove);
	compod->slot_transform_coords = component->sig_transform_coords().connect(compod, &Component_derived::on_transform_coords);
	compod->slot_get_minimum_size = component->sig_get_minimum_size().connect(compod, &Component_derived::on_get_minimum_size);
	compod->slot_get_maximum_size = component->sig_get_maximum_size().connect(compod, &Component_derived::on_get_maximum_size);
	compod->slot_get_preferred_size = component->sig_get_preferred_size().connect(compod, &Component_derived::on_get_preferred_size);
}
