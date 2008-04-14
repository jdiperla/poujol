
#include "stdafx.h"
#include <boost/python.hpp>
#include <ClanLib/gui.h>

using namespace boost::python;

static CL_Rect to_rect(const CL_Rectf &rect)
{
	return CL_Rect((int) rect.left, (int) rect.top, (int) rect.right, (int) rect.bottom);
}

static CL_Point to_point(const CL_Pointf &pt)
{
	return CL_Point((int) pt.x, (int) pt.y);
}

struct CL_Button_wrap: public CL_Button
{
	CL_Button_wrap(PyObject* self_, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_Button(parent, style),
		self(self_)
	{
		init_slots();
	}

	void init_slots()
	{
		slots.connect(sig_toggled(), this, &CL_Button_wrap::on_toggled);
		slots.connect(sig_clicked(), this, &CL_Button_wrap::on_clicked);
		slots.connect(sig_pressed(), this, &CL_Button_wrap::on_pressed);
		slots.connect(sig_released(), this, &CL_Button_wrap::on_released);
	}

	CL_Button_wrap(PyObject* self_, const std::string &title, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_Button(title, parent, style),
       		self(self_) { init_slots(); }

	CL_Button_wrap(PyObject* self_, const CL_Pointf &point, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_Button(to_point(point), parent, style),
                self(self_) { init_slots(); }

	CL_Button_wrap(PyObject* self_, const CL_Pointf &point, const std::string &title, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_Button(to_point(point), title, parent, style),
                self(self_) { init_slots(); }

	CL_Button_wrap(PyObject* self_, const CL_Rectf &rect, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_Button(to_rect(rect), parent, style),
                self(self_) { init_slots(); }

	CL_Button_wrap(PyObject* self_, const CL_Rectf &rect, const std::string &title, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_Button(to_rect(rect), title, parent, style),
                self(self_) { init_slots(); }

	void on_toggled(bool state) { call_method<void>(self, "on_toggled", state); }
	void default_on_toggled(bool state) {}
	void on_clicked() { call_method<void>(self, "on_clicked"); }
	void default_on_clicked() {}
	void on_pressed() { call_method<void>(self, "on_pressed"); }
	void default_on_pressed() {}
	void on_released() { call_method<void>(self, "on_released"); }
	void default_on_released() {}

	CL_SlotContainer slots;
	PyObject* self;
};

void export_button()
{
	class_<CL_Button,
		   bases<CL_Component>,
		   CL_Button_wrap,
		   boost::noncopyable> CL_Button_wrapper(
				"Button",
				init< CL_Component*, optional<CL_StyleManager*> >());

	CL_Button_wrapper
		.def(init<const std::string &, CL_Component*, optional<CL_StyleManager*> >())
		.def(init<const CL_Rectf &, const std::string &, CL_Component*, optional<CL_StyleManager*> >())
		.def(init<const CL_Rectf &, CL_Component*, optional<CL_StyleManager*> >())
		.def(init<const CL_Pointf &, const std::string &, CL_Component*, optional<CL_StyleManager*> >())
		.def(init<const CL_Pointf &, CL_Component*, optional<CL_StyleManager*> >())
		.def("get_text", &CL_Button::get_text, return_value_policy<copy_const_reference>())
		.def("is_toggle_button", &CL_Button::is_toggle_button)
		.def("is_button_down", &CL_Button::is_down)
		.def("set_text", &CL_Button::set_text)
		.def("set_toggle_mode", &CL_Button::set_toggle_mode)
		.def("set_down", &CL_Button::set_down)
		.def("toggle", &CL_Button::toggle);
	CL_Button_wrapper
		.add_property("text",
			CL_Button_wrapper.attr("get_text"))
		.add_property("toggled",
			CL_Button_wrapper.attr("is_toggle_button"),
			CL_Button_wrapper.attr("set_toggle_mode"))
		.add_property("is_down",
			CL_Button_wrapper.attr("is_button_down"),
			CL_Button_wrapper.attr("set_down"))
		
		.def("on_toggled", &CL_Button_wrap::default_on_toggled)
		.def("on_clicked", &CL_Button_wrap::default_on_clicked)
		.def("on_pressed", &CL_Button_wrap::default_on_pressed)
		.def("on_released", &CL_Button_wrap::default_on_released)
		
	;
}
