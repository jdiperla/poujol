#include "stdafx.h"
#include <boost/python.hpp>
#include <ClanLib/gui.h>
#include <ClanLib/core.h>

using namespace boost::python;

void export_frame()
{
	class_<CL_Frame, bases<CL_Component>, boost::noncopyable>("Frame", init<CL_Component*, CL_StyleManager*>())
		.def(init<const CL_Rect&, CL_Component*, CL_StyleManager*>())
		.add_property("fill", &CL_Frame::is_fill_enabled, &CL_Frame::enable_fill);
}
