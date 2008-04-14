#include "stdafx.h"
#include <boost/python.hpp>
#include <ClanLib/gui.h>
#include <ClanLib/core.h>

using namespace boost::python;

void export_label()
{
	void (CL_Label::*fx1)(const std::string&) = &CL_Label::set_text;
	void (CL_Label::*fx2)(int) = &CL_Label::set_text;
	void (CL_Label::*fx3)(double) = &CL_Label::set_text;

	class_<CL_Label, bases<CL_Component>, boost::noncopyable> CL_Label_wrapper("Label", init<CL_Component*, CL_StyleManager*>());
	CL_Label_wrapper
		.def(init<const char*, CL_Component*, CL_StyleManager*>())
		.def(init<const CL_Point&, const char*, CL_Component*, CL_StyleManager*>())
		.def(init<const CL_Rect&, const char*, CL_Component*, CL_StyleManager*>())

		.def("get_text", &CL_Label::get_text, return_value_policy<copy_const_reference>())
		.def("set_text", fx1)
		.def("set_text", fx2)
		.def("set_text", fx3)
		.def("clear", &CL_Label::clear)
		.add_property("alignment", &CL_Label::get_alignment, &CL_Label::set_alignment)
	;
}
