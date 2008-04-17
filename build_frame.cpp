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

void export_frame()
{
	class_<CL_Frame, bases<CL_Component>, boost::noncopyable>("Frame", init<CL_Component*, CL_StyleManager*>())
		.def(init<const CL_Rect&, CL_Component*, CL_StyleManager*>())
		.add_property("fill", &CL_Frame::is_fill_enabled, &CL_Frame::enable_fill);
}
