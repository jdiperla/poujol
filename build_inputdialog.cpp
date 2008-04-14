#include "stdafx.h"
#include <boost/python.hpp>
#include <ClanLib/gui.h>
#include <ClanLib/core.h>

using namespace boost::python;

void export_input_dialog()
{
	class_<CL_InputDialog, bases<CL_Window>, boost::noncopyable> ("InputDialog",
		init<const char*, const char*, const char*, const char*,
			 CL_Component*, CL_StyleManager*>())
		.def("add_input_box", &CL_InputDialog::add_input_box, return_value_policy<reference_existing_object>())
		.def("add_check_box", &CL_InputDialog::add_check_box, return_value_policy<reference_existing_object>())
		.add_property("result_button", &CL_InputDialog::get_result_button)
	;
}
