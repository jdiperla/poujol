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

struct CL_SoundProvider_wrap : public CL_SoundProvider
{
	CL_SoundProvider_wrap(PyObject* self_) :
		self(self_) {}
	CL_SoundProvider_wrap(PyObject* self_, const CL_SoundProvider& provider)
		: CL_SoundProvider(provider), self(self_) {}

	CL_SoundProvider_Session* begin_session()
	{
		return NULL;
	}

	void end_session(CL_SoundProvider_Session* session)
	{
	}

	PyObject* self;
};

void export_sound_provider()
{
	class_<CL_SoundProvider, bases< >, CL_SoundProvider_wrap, boost::noncopyable>("SoundProvider", init<>())
		.def("begin_session", &CL_SoundProvider::begin_session,
			return_value_policy<reference_existing_object>())
		.def("end_session", &CL_SoundProvider::end_session)
	;
}


