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
