#include "stdafx.h"
#include <boost/python.hpp>
#include <ClanLib/sound.h>

using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	fade_overloads, fade_to_volume, 1, 2);

struct CL_SoundFilter_wrap : public CL_SoundFilter
{
	CL_SoundFilter_wrap(PyObject* self_) :
		self(self_) {}
	CL_SoundFilter_wrap(PyObject* self_, const CL_SoundFilter& filtre)
		: CL_SoundFilter(filtre), self(self_) {}

	virtual void filter(
		int* *sample_data, int num_samples, int channels)
	{
	}

	PyObject* self;
};

void export_sound_filter()
{
/*
    void (CL_SoundBuffer::*fx1)(const std::string&) = &CL_SoundBuffer::play;
	void (CL_InputBox::*fx2)(int) = &CL_InputBox::set_text;
	void (CL_InputBox::*fx3)(double) = &CL_InputBox::set_text;
	void (CL_FadeFilter::*fx1)(float, int) = &CL_FadeFilter::fade_to_volume;
*/
	
	class_<CL_SoundFilter, bases< >, CL_SoundFilter_wrap, boost::noncopyable> CL_SoundFilter_wrapper("SoundFilter",
		init<>());
	CL_SoundFilter_wrapper
		.def(init<const CL_SoundFilter&>())
		.def("filter", &CL_SoundFilter::filter)
	;
	class_<CL_EchoFilter, bases<CL_SoundFilter>, boost::noncopyable> ("EchoFilter",
		init<optional <int, int> >())
		.def("filter", &CL_EchoFilter::filter)
	;
	class_<CL_FadeFilter, bases<CL_SoundFilter>, boost::noncopyable> ("FadeFilter",
		init<float>())
        .def("get_volume", &CL_FadeFilter::get_volume)
		.def("set_volume", &CL_FadeFilter::set_volume)
		.add_property("volume", &CL_FadeFilter::get_volume, &CL_FadeFilter::set_volume)
		.def("fade_to_volume", &CL_FadeFilter::fade_to_volume, fade_overloads())
	;
	class_<CL_InverseEchoFilter, bases<CL_SoundFilter>, boost::noncopyable> ("InverseEchoFilter",
		init<optional<int> >())
		.def("filter", &CL_InverseEchoFilter::filter)
    ;
}


