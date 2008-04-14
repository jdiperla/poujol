
#include "stdafx.h"
#include <boost/python.hpp>
#include <ClanLib/gui.h>
#include <ClanLib/core.h>

using namespace boost::python;

void export_radio_button()
{
	class_<CL_RadioButton, bases<CL_Button>, boost::noncopyable>("RadioButton", init<CL_Component*, optional<CL_StyleManager*> >())
		.def(init<const CL_Point&, const char*, CL_Component*, optional<CL_StyleManager*> >())
		.def(init<const char*, CL_Component*, optional<CL_StyleManager*> >())
		.add_property("checked", &CL_RadioButton::is_checked, &CL_RadioButton::set_checked)
;
}
