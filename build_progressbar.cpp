#include "stdafx.h"
#include <boost/python.hpp>
#include <ClanLib/gui.h>
#include <ClanLib/core.h>

using namespace boost::python;

void export_progress_bar()
{
	class_<CL_ProgressBar, bases<CL_Component>, boost::noncopyable> CL_ProgressBar_wrapper("ProgressBar", init<CL_Component*, optional<CL_StyleManager*> >());
	CL_ProgressBar_wrapper
		.def(init<int, CL_Component*, optional<CL_StyleManager*> >())
		.def(init<const CL_Rect&, int, CL_Component*, optional<CL_StyleManager*> >())

		.add_property("progress", &CL_ProgressBar::get_progress, &CL_ProgressBar::set_progress)
		.add_property("steps", &CL_ProgressBar::get_steps, &CL_ProgressBar::set_steps)
		.add_property("percentage", &CL_ProgressBar::get_percentage)
		.def("increase", &CL_ProgressBar::increase)
		.def("reset", &CL_ProgressBar::reset)
    ;
}
