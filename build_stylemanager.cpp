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
#include <ClanLib/core.h>
#include <ClanLib/guistylesilver.h> 

using namespace boost::python;

struct CL_StyleManager_wrap : public CL_StyleManager
{
	CL_StyleManager_wrap(PyObject* self_) :
		CL_StyleManager(NULL),
                self(self_) {}
	CL_StyleManager_wrap(PyObject* self_, CL_ResourceManager* resources)
		: CL_StyleManager(resources), self(self_) {}
	CL_StyleManager_wrap(CL_ResourceManager* resources)
		: CL_StyleManager(resources) {}

	virtual void connect_styles(
		const std::string &type,
		CL_Component *owner) { }

	PyObject* self;
};

struct CL_StyleSilverManager_wrap : public CL_StyleManager_Silver
{
	CL_StyleSilverManager_wrap(PyObject* self_) :
		CL_StyleManager_Silver(NULL),
                self(self_) {}
	CL_StyleSilverManager_wrap(PyObject* self_, CL_ResourceManager* resources)
		: CL_StyleManager_Silver(resources), self(self_) {}
	CL_StyleSilverManager_wrap(CL_ResourceManager* resources)
		: CL_StyleManager_Silver(resources) {}

	/*virtual void connect_styles(
		const std::string &type,
		CL_Component *owner) { }*/

	PyObject* self;
};

void export_stylemanager()
{
	class_<CL_StyleManager, bases< >, CL_StyleManager_wrap, boost::noncopyable>("StyleManager", init<CL_ResourceManager*>());
	class_<CL_StyleManager_Silver, bases< CL_StyleManager >, CL_StyleSilverManager_wrap, boost::noncopyable>("StyleManager_Silver", init<CL_ResourceManager*>());
}
