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

using namespace boost::python;

static CL_Rect to_rect(const CL_Rectf &rect)
{
	return CL_Rect((int) rect.left, (int) rect.top, (int) rect.right, (int) rect.bottom);
}

struct CL_Window_wrap : public CL_Window
{
	CL_Window_wrap(PyObject* self_, CL_Component* component, CL_StyleManager* style = NULL) :
		CL_Window(component, style), self(self_)
	{
	}

	/*CL_Window_wrap(PyObject* self_, const CL_Rectf& rect, CL_Component* component, CL_StyleManager* style = NULL) :
		CL_Window(to_rect(rect), component, style), self(self_)
	{
	}*/

	CL_Window_wrap(PyObject* self_, const CL_Rectf& rect, const char *s, CL_Component* component, CL_StyleManager* style = NULL) :
		CL_Window(to_rect(rect), s, component, style), self(self_)
	{
		/*slot_paint = sig_paint().connect(this, &CL_Window_wrap::on_paint);
		slot_paint_children = sig_paint_children().connect(this, &CL_Window_wrap::on_paint_children);
		slot_begin_paint = sig_begin_paint().connect(this, &CL_Window_wrap::on_begin_paint);
		slot_end_paint = sig_end_paint().connect(this, &CL_Window_wrap::on_end_paint);*/
	}

	PyObject* self;
};

void export_window()
{
	class_<CL_Window, bases<CL_Component>, CL_Window_wrap, boost::noncopyable>("Window", init< CL_Component*, optional<CL_StyleManager*> >())
		//.def(init<const CL_Rectf&, CL_Component*, optional<CL_StyleManager*> >())
		.def(init<const CL_Rectf&, const char *, CL_Component*, optional<CL_StyleManager*> >())
		.def("get_title", &CL_Window::get_title, return_value_policy<copy_const_reference>())
		.def("set_title", &CL_Window::set_title);
}
