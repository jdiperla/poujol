
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


