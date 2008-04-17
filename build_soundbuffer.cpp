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
#include <ClanLib/core.h>
#include <boost/python/manage_new_object.hpp>

using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	play_overloads, play, 0, 2);

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	prepare_overloads, prepare, 0, 2);

void export_sound_buffer()
{
/*	void (CL_SoundBuffer::*fx1)(const std::string&) = &CL_SoundBuffer::play;
	void (CL_InputBox::*fx2)(int) = &CL_InputBox::set_text;
	void (CL_InputBox::*fx3)(double) = &CL_InputBox::set_text;
*/
	
	class_<CL_SoundBuffer> CL_SoundBuffer_wrapper("Sound",
		init<const std::string&, CL_ResourceManager*>());
	CL_SoundBuffer_wrapper
		.def(init<>())
		.def(init<CL_SoundProvider*, optional<bool> >())
		.def(init<const std::string&, optional<bool, const std::string&> >())
		.add_property("volume", &CL_SoundBuffer::get_volume, &CL_SoundBuffer::set_volume)
		.add_property("pan", &CL_SoundBuffer::get_pan, &CL_SoundBuffer::set_pan)
		.add_property("playing", &CL_SoundBuffer::is_playing)
		.def("stop", &CL_SoundBuffer::stop)
		.def("play", &CL_SoundBuffer::play, play_overloads())
		.def("prepare", &CL_SoundBuffer::prepare, prepare_overloads())
		.def("add_filter", &CL_SoundBuffer::add_filter)
		.def("remove_filter", &CL_SoundBuffer::remove_filter)
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


