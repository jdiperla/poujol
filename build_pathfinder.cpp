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
#include "pathfinder.h"
#include <string>

using namespace boost::python;

struct stdpointlist_to_python
	: to_python_converter<std::list<CL_Point>, stdpointlist_to_python>
{
	static PyObject* convert(std::list<CL_Point> const& x)
    {
		PyObject *l = PyList_New(x.size());
		int j = 0;
		for(std::list<CL_Point>::const_iterator i = x.begin(); i != x.end(); j++, i++) {
			PyObject *l2 = PyList_New(2);
			PyList_SetItem(l2, 0, PyInt_FromLong((*i).x));
			PyList_SetItem(l2, 1, PyInt_FromLong((*i).y));
			PyList_SetItem(l, j, l2);
		}
		return l;
    }
};

struct CPathfinder_wrap : public CPathfinder
{
	CPathfinder_wrap(PyObject* self_) :
		self(self_) {
    }

	PyObject* self;
};

void export_pathfinder()
{
	stdpointlist_to_python();
	class_<CPathfinder, CPathfinder_wrap, boost::noncopyable> CPathfinder_wrapper("Pathfinder");
	CPathfinder_wrapper
		.def("add_region", &CPathfinder::add_region)
		.def("find_path", &CPathfinder::find_path)
		.def("get_waypoints", &CPathfinder::get_waypoints)
		.add_property("waypoints",
			CPathfinder_wrapper.attr("get_waypoints"))
	;
}
