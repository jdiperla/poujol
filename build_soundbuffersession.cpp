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

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	play_overloads, play, 0, 2);

void export_sound_buffer_session()
{
/*	void (CL_SoundBuffer::*fx1)(const std::string&) = &CL_SoundBuffer::play;
	void (CL_InputBox::*fx2)(int) = &CL_InputBox::set_text;
	void (CL_InputBox::*fx3)(double) = &CL_InputBox::set_text;
*/
	
	class_<CL_SoundBuffer_Session>("SoundSession",
		init<>())
		.add_property("position",
			&CL_SoundBuffer_Session::get_position,
			&CL_SoundBuffer_Session::set_position)
		.add_property("position_relative",
			&CL_SoundBuffer_Session::get_position_relative,
			&CL_SoundBuffer_Session::set_position_relative)
		.add_property("frequency",
			&CL_SoundBuffer_Session::get_frequency,
			&CL_SoundBuffer_Session::set_frequency)
		.add_property("length",
			&CL_SoundBuffer_Session::get_length)
		.add_property("volume", &CL_SoundBuffer_Session::get_volume, &CL_SoundBuffer_Session::set_volume)
		.add_property("pan", &CL_SoundBuffer_Session::get_pan, &CL_SoundBuffer_Session::set_pan)
		.def("stop", &CL_SoundBuffer_Session::stop)
		.def("play", &CL_SoundBuffer_Session::play)
		.def("add_filter", &CL_SoundBuffer_Session::add_filter)
		.def("remove_filter", &CL_SoundBuffer_Session::remove_filter)
		/*.add_property("looping",
            &CL_SoundBuffer_Session::get_looping,
            &CL_SoundBuffer_Session::set_looping)*/
		.add_property("playing", &CL_SoundBuffer_Session::is_playing)

/*
		.def("get_text", &CL_InputBox::get_text, return_value_policy<copy_const_reference>())
		.def("set_text", fx1)
		.def("set_text", fx2)
		.def("set_text", fx3)
		.def("get_marked_text", &CL_InputBox::get_marked_text, return_value_policy<copy_const_reference>())
		.def("select_all", &CL_InputBox::select_all)
		.def("deselect", &CL_InputBox::deselect)
		.def("clear", &CL_InputBox::clear)
		.def("backspace", &CL_InputBox::backspace)
		.def("del", &CL_InputBox::del)
		.def("cut", &CL_InputBox::cut)
		.def("home", &CL_InputBox::home)
		.def("end", &CL_InputBox::end)
		.def("set_selection", &CL_InputBox::set_selection)
		.def("move_cursor", &CL_InputBox::move_cursor)
		.def("move_cursor_word", &CL_InputBox::move_cursor_word)
*/
;

/*	CL_InputBox_wrapper
		.add_property("text",
			CL_InputBox_wrapper.attr("get_text"),
			CL_InputBox_wrapper.attr("set_text"))
		.add_property("marked_text",
			CL_InputBox_wrapper.attr("get_marked_text"))
		.add_property("has_marked_text",
			&CL_InputBox::has_marked_text)
		.add_property("readonly",
			&CL_InputBox::is_read_only, &CL_InputBox::set_read_only)
		.add_property("readonly",
			&CL_InputBox::is_read_only, &CL_InputBox::set_read_only)
		.add_property("max_length",
			&CL_InputBox::get_max_length, &CL_InputBox::set_max_length)
		.add_property("edited",
			&CL_InputBox::is_edited, &CL_InputBox::set_edited)
		.add_property("selection_start",
			&CL_InputBox::get_selection_start)
		.add_property("selection_length",
			&CL_InputBox::get_selection_length)
		.add_property("password_mode",
			&CL_InputBox::in_password_mode, &CL_InputBox::set_password_mode)
		.add_property("cursor_position",
			&CL_InputBox::get_cursor_position, &CL_InputBox::set_cursor_position)

	;
*/
}


