
#include "stdafx.h"
#include <boost/python.hpp>
#include <ClanLib/gui.h>
#include <ClanLib/core.h>

using namespace boost::python;

static CL_Rect to_rect(const CL_Rectf &rect)
{
	return CL_Rect((int) rect.left, (int) rect.top,
               (int) rect.right, (int) rect.bottom);
}

struct CL_InputBox_wrap: public CL_InputBox
{
	CL_Slot s1, s2, s3, s4;
	
	CL_InputBox_wrap(PyObject* self_, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_InputBox(parent, style),
                self(self_)
	{
		init_slots();
	}

	void init_slots()
	{
		s1 = sig_changed().connect(this, &CL_InputBox_wrap::on_changed);
		s2 = sig_return_pressed().connect(this, &CL_InputBox_wrap::on_return_pressed);
		s3 = sig_activity().connect(this, &CL_InputBox_wrap::on_activity);
		s4 = sig_validate_character().connect(this, &CL_InputBox_wrap::on_validate_character);
	}

	CL_InputBox_wrap(PyObject* self_, const std::string &title, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_InputBox(title, parent, style),
		self(self_)
        { init_slots(); }

	CL_InputBox_wrap(PyObject* self_, const CL_Rectf &rect, const std::string &title, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_InputBox(to_rect(rect), title, parent, style),
                self(self_)
        { init_slots(); }

	CL_InputBox_wrap(PyObject* self_, const CL_Rectf &rect, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_InputBox(to_rect(rect), parent, style),
                self(self_)
        { init_slots(); }

	void on_changed(const std::string &s) { call_method<void>(self, "on_changed", s); }
	void default_on_changed(const std::string &s) {}
	void on_return_pressed() { call_method<void>(self, "on_return_pressed"); }
	void default_on_return_pressed() {}
	void on_activity() { call_method<void>(self, "on_activity"); }
	void default_on_activity() {}
	void on_validate_character(char &c, bool &b) { b = call_method<bool>(self, "on_validate_character", c); }
	bool default_on_validate_character(char &c) { return true; }

	PyObject* self;
};

void export_input_box()
{
	void (CL_InputBox::*fx1)(const std::string&) = &CL_InputBox::set_text;
	void (CL_InputBox::*fx2)(int) = &CL_InputBox::set_text;
	void (CL_InputBox::*fx3)(double) = &CL_InputBox::set_text;

	class_<CL_InputBox, bases<CL_Component>, CL_InputBox_wrap, boost::noncopyable> CL_InputBox_wrapper("InputBox", init<CL_Component*, CL_StyleManager*>());
	CL_InputBox_wrapper
		.def(init<const CL_Rectf&, CL_Component*, CL_StyleManager*>())
		.def(init<const std::string&, CL_Component*, CL_StyleManager*>())
		.def(init<const CL_Rectf&, const std::string&, CL_Component*, CL_StyleManager*>())
		.def("get_text", &CL_InputBox::get_text, return_value_policy<copy_const_reference>())
		.def("set_text", fx1)
		.def("set_text", fx2)
		.def("set_text", fx3)
		.def("select_all", &CL_InputBox::select_all)
		.def("deselect", &CL_InputBox::deselect)
		.def("clear", &CL_InputBox::clear)
		.def("backspace", &CL_InputBox::backspace)
		.def("del", &CL_InputBox::del)
		.def("cut", &CL_InputBox::cut)
		.def("home", &CL_InputBox::home)
		.def("end", &CL_InputBox::end)
		.def("set_selection", &CL_InputBox::set_selection)
		.def("move_cursor", &CL_InputBox::move_cursor)
		.def("move_cursor_word", &CL_InputBox::move_cursor_word)
	;

	/*
    CL_InputBox_wrapper
		.def("get_marked_text", &CL_InputBox::get_marked_text, return_value_policy<copy_const_reference>())
		.add_property("text",
			CL_InputBox_wrapper.attr("get_text"),
			CL_InputBox_wrapper.attr("set_text"))
		.add_property("marked_text",
			CL_InputBox_wrapper.attr("get_marked_text"))
		.add_property("has_marked_text",
			&CL_InputBox::has_marked_text)
		.add_property("readonly",
			&CL_InputBox::is_read_only, &CL_InputBox::set_read_only)
		.add_property("readonly",
			&CL_InputBox::is_read_only, &CL_InputBox::set_read_only)
		.add_property("max_length",
			&CL_InputBox::get_max_length, &CL_InputBox::set_max_length)
		.add_property("edited",
			&CL_InputBox::is_edited, &CL_InputBox::set_edited)
		.add_property("selection_start",
			&CL_InputBox::get_selection_start)
		.add_property("selection_length",
			&CL_InputBox::get_selection_length)
		.add_property("password_mode",
			&CL_InputBox::in_password_mode, &CL_InputBox::set_password_mode)
		.add_property("cursor_position",
			&CL_InputBox::get_cursor_position, &CL_InputBox::set_cursor_position)
		.def("on_activity", &CL_InputBox_wrap::default_on_activity)
		.def("on_return_pressed", &CL_InputBox_wrap::default_on_return_pressed)
		.def("on_changed", &CL_InputBox_wrap::default_on_changed)
		.def("on_validate_character", &CL_InputBox_wrap::default_on_validate_character)

	;
    */
}
