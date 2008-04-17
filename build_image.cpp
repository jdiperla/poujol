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
#include <ClanLib/display.h>

using namespace boost::python;

void export_image()
{
	class_<CL_Image, bases<CL_Component>, boost::noncopyable>("Image", init<CL_Component*, optional<CL_StyleManager*> >())
/*		
		.def(init<const CL_Rect&, CL_Component*, optional<CL_StyleManager*> >())
		.def(init<const CL_Rect&, CL_Surface*, CL_Component*, optional<CL_StyleManager*> >())
*/
        .def(init<CL_Surface*, bool, CL_Component*, optional<CL_StyleManager*> >())
        .def("get_surface", &CL_Image::get_surface, return_value_policy<reference_existing_object>())
        .def("set_surface", &CL_Image::set_surface)
        .add_property("mode", &CL_Image::get_mode, &CL_Image::set_mode)
	;

	enum_<CL_Image::Mode>("Mode")
        .value("normal", CL_Image::normal)
        .value("center", CL_Image::center)
        .value("stretch", CL_Image::stretch)
        .value("tile", CL_Image::stretch);
}
