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
#include <ClanLib/sound.h>

using namespace boost::python;

void export_sound_output()
{
	class_<CL_SoundOutput>("SoundOutput", init<>())
		.def(init<int>())
//		.def(init<const CL_SoundOutput_Description&>())
		.add_property("mixing_frequency",
			&CL_SoundOutput::get_mixing_frequency)
		.add_property("global_volume",
			&CL_SoundOutput::get_global_volume,
			&CL_SoundOutput::set_global_volume)
		.add_property("global_pan",
			&CL_SoundOutput::get_global_pan,
			&CL_SoundOutput::set_global_pan)
		.def("add_filter", &CL_SoundOutput::add_filter)
		.def("remove_filter", &CL_SoundOutput::remove_filter)
		.def("stop_all", &CL_SoundOutput::stop_all)
		.def("get_name", &CL_SoundOutput::get_name, return_value_policy<copy_const_reference>()) 
	;
}


