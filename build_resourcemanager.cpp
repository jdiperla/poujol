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
#include <ClanLib/core.h>
#include <ClanLib/gui.h>

using namespace boost::python;

struct stdlist_to_python
	: to_python_converter<std::vector<std::string>, stdlist_to_python>
{
	static PyObject* convert(std::vector<std::string> const& x)
    {
		PyObject *l = PyList_New(x.size());
		int j = 0;
		for(std::vector<std::string>::const_iterator i = x.begin(); i != x.end(); j++, i++) {
			PyObject* obj = PyString_FromString((*i).c_str()); 
			PyList_SetItem(l, j, obj);
		}
		return l;
    }
};

void export_resource_manager()
{
	std::vector<std::string> (CL_ResourceManager::*fx1)() = &CL_ResourceManager::get_all_resources;
	std::vector<std::string> (CL_ResourceManager::*fx2)(const std::string&) = &CL_ResourceManager::get_all_resources;
	std::vector<std::string> (CL_ResourceManager::*fx3)(const std::string&) = &CL_ResourceManager::get_resources_of_type;
	std::vector<std::string> (CL_ResourceManager::*fx4)(const std::string&, const std::string&) = &CL_ResourceManager::get_resources_of_type;
	stdlist_to_python();
	class_<CL_ResourceManager>("ResourceManager", init<>())
		.def(init<const char *>())
		.def("add_resources", &CL_ResourceManager::add_resources)
		.def("remove_resources", &CL_ResourceManager::remove_resources)
		.def("load_all", &CL_ResourceManager::load_all)
		.def("unload_all", &CL_ResourceManager::unload_all)
		.def("load_section", &CL_ResourceManager::load_section)
		.def("unload_section", &CL_ResourceManager::unload_section)
		.def("exists", &CL_ResourceManager::exists)
		.def("get_resource", &CL_ResourceManager::get_resource, return_internal_reference<>())
		.def("get_all_sections", &CL_ResourceManager::get_all_sections)
		.def("get_all_resources", fx1)
		.def("get_all_resources", fx2)
		.def("get_resources_of_type", fx3)
		.def("get_resources_of_type", fx4)
	;

	class_<CL_ComponentManager>("ComponentManager", init<const std::string&, CL_ResourceManager*, CL_Component*, optional<CL_StyleManager*> >());
}
