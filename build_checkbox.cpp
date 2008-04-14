#include "stdafx.h"
#include <boost/python.hpp>
#include <ClanLib/gui.h>

using namespace boost::python;

void export_checkbox()
{
	class_<CL_CheckBox, bases<CL_Button>, CL_CheckBox, boost::noncopyable>("CheckBox", init< CL_Component*, optional<CL_StyleManager*> >())
		.def(init<const CL_Point&, const char *, CL_Component*, optional<CL_StyleManager*> >())
		.def(init<const char *, CL_Component*, optional<CL_StyleManager*> >())
		.def("is_checked", &CL_CheckBox::is_checked)
		.def("set_checked", &CL_CheckBox::set_checked)
		.add_property("checked", &CL_CheckBox::is_checked, &CL_CheckBox::set_checked);
}
