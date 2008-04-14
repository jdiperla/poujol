
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


