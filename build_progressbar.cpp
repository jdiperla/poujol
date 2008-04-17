/*
Glumol - An adventure game creator
Copyright (C) 1998-2008  Sylvain Baubeau & Alexis Contour

This file is part of Glumol.

Glumol is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Glumol is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Glumol.  If not, see <http://www.gnu.org/licenses/>.
*/

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
