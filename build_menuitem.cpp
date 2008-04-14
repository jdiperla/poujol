#include "stdafx.h"
#include <boost/python.hpp>
#include <ClanLib/gui.h>
#include <ClanLib/core.h>

using namespace boost::python;

void export_menu_item()
{
	void (CL_MenuItem::*fx1)(const std::string&) = &CL_MenuItem::set_text;
	void (CL_MenuItem::*fx2)(int) = &CL_MenuItem::set_text;
	void (CL_MenuItem::*fx3)(double) = &CL_MenuItem::set_text;
	
	class_<CL_MenuItem, bases<CL_Component>, boost::noncopyable> CL_MenuItem_wrapper("MenuItem", init<const char*, CL_Component*, optional<CL_StyleManager*> >());
	CL_MenuItem_wrapper
		.add_property("selected", &CL_MenuItem::is_selected, &CL_MenuItem::set_selected)
		.add_property("toggling", &CL_MenuItem::is_toggling, &CL_MenuItem::set_toggling)
		.add_property("use_icon", &CL_MenuItem::use_icon, &CL_MenuItem::set_use_icon)
		.def("get_text", &CL_MenuItem::get_text, return_value_policy<copy_const_reference>())
		.def("set_text", fx1)
		.def("set_text", fx2)
		.def("set_text", fx3)
		.def("clear", &CL_MenuItem::clear)
	;
}
