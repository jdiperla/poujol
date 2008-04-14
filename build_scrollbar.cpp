
#include "stdafx.h"
#include <boost/python.hpp>
#include <ClanLib/gui.h>
#include <ClanLib/core.h>

using namespace boost::python;

struct CL_ScrollBar_wrap: public CL_ScrollBar
{
	CL_ScrollBar_wrap(PyObject* self_, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_ScrollBar(parent, style),
                self(self_) { init_slots(); }

	CL_ScrollBar_wrap(PyObject* self_, int max, int min, bool orientation, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_ScrollBar(min, max, orientation, parent, style),
                self(self_) { init_slots(); }

	CL_ScrollBar_wrap(PyObject* self_, bool orientation, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_ScrollBar(orientation, parent, style),
                self(self_) { init_slots(); }

	CL_ScrollBar_wrap(PyObject* self_, const CL_Rect &rect, int min, int max, bool orientation, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_ScrollBar(rect, min, max, orientation, parent, style),
                self(self_) { init_slots(); }

	CL_ScrollBar_wrap(PyObject* self_, const CL_Rect &rect, bool orientation, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_ScrollBar(rect, orientation, parent, style),
                self(self_) { init_slots(); }

	void init_slots()
	{
		CL_Slot s1 = sig_value_changed().connect(this, &CL_ScrollBar_wrap::on_value_changed);
		CL_Slot s2 = sig_slider_pressed().connect(this, &CL_ScrollBar_wrap::on_slider_pressed);
		CL_Slot s3 = sig_slider_moved().connect(this, &CL_ScrollBar_wrap::on_slider_moved);
		CL_Slot s4 = sig_slider_released().connect(this, &CL_ScrollBar_wrap::on_slider_released);
	}

	void on_value_changed(int i) { call_method<void>(self, "on_value_changed", i); }
	void default_on_value_changed(int i) {}

	void on_slider_pressed() { call_method<void>(self, "on_slider_pressed"); }
	void default_on_slider_pressed() {}

	void on_slider_moved(int i) { call_method<void>(self, "on_slider_moved", i); }
	void default_on_slider_moved(int i) {}

	void on_slider_released() { call_method<void>(self, "on_slider_released"); }
	void default_on_slider_released() {}

	PyObject* self;
};

void export_scroll_bar()
{
	class_<CL_ScrollBar, bases<CL_Component>, boost::noncopyable> CL_ScrollBar_wrapper("ScrollBar", init<CL_Component*, CL_StyleManager*>());
	CL_ScrollBar_wrapper
		.def(init<int, int, bool, CL_Component*, CL_StyleManager*>())
		.def(init<bool, CL_Component*, CL_StyleManager*>())
		.def(init<const CL_Rect&, int, int, bool, CL_Component*, CL_StyleManager*>())
		.def(init<const CL_Rect&, bool, CL_Component*, CL_StyleManager*>())
		.def("increase", &CL_ScrollBar::increase)
		.def("decrease", &CL_ScrollBar::decrease)
		.def("set_range", &CL_ScrollBar::set_range)
		.def("get_slider_rect", &CL_ScrollBar::get_slider_rect, return_value_policy<copy_non_const_reference>()) 
		.add_property("min_value", &CL_ScrollBar::get_min_value, &CL_ScrollBar::set_min_value)
		.add_property("max_value", &CL_ScrollBar::get_max_value, &CL_ScrollBar::set_max_value)
		.add_property("value", &CL_ScrollBar::get_value, &CL_ScrollBar::set_value)
		.add_property("tracking", &CL_ScrollBar::is_tracking, &CL_ScrollBar::set_tracking)
		.add_property("vertical", &CL_ScrollBar::is_vertical, &CL_ScrollBar::set_vertical)
		.add_property("min_slider_length", &CL_ScrollBar::get_min_slider_length, &CL_ScrollBar::set_min_slider_length)
		.add_property("slider_length", &CL_ScrollBar::get_range, &CL_ScrollBar::set_slider_length)
		.add_property("fixed_length_mode", &CL_ScrollBar::is_fixed_length_slider, &CL_ScrollBar::set_fixed_length_mode)
		.add_property("is_dragging_slider", &CL_ScrollBar::is_dragging_slider)
		.def("on_value_changed", &CL_ScrollBar_wrap::on_value_changed)
		.def("on_slider_pressed", &CL_ScrollBar_wrap::on_slider_pressed)
		.def("on_slider_moved", &CL_ScrollBar_wrap::on_slider_moved)
		.def("on_slider_released", &CL_ScrollBar_wrap::on_slider_released)
	;
}


