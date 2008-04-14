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


