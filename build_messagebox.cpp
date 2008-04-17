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
#include <string>
#include <ClanLib/gui.h>
#include <ClanLib/core.h>

using namespace boost::python;

int message_box(
		const std::string& text,
		CL_Component* parent,
		CL_StyleManager* style = NULL)
{
	return CL_MessageBox::info(text, parent, style);
}

int message_box2(
		const std::string& title,
		const std::string& text,
		CL_Component* parent,
		CL_StyleManager* style = NULL)
{
	return CL_MessageBox::info(title, text, parent, style);
}

int message_box3(
		const std::string& title,
		const std::string& text,
		const std::string& button1,
		const std::string& button2,
		const std::string& button3,
		CL_Component* parent,
		CL_StyleManager* style = NULL)
{
	return CL_MessageBox::info(title, text, button1, button2, button3, parent, style);
}

void export_message_box()
{
	def("message_box", message_box);
	def("message_box", message_box2);
	def("message_box", message_box3);
}


