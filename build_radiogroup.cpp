
#include "stdafx.h"
#include <boost/python.hpp>
#include <ClanLib/gui.h>
#include <ClanLib/core.h>

using namespace boost::python;

struct CL_RadioGroup_wrap: public CL_RadioGroup
{
	/*CL_RadioGroup_wrap(PyObject* self_) :
		self(self_)
	{
		init_slots();
	}

	void init_slots()
	{
		CL_Slot s1 = sig_selection_changed().connect(this, &CL_RadioGroup_wrap::on_selection_changed);
	}

	void on_selection_changed(CL_RadioButton *button) { call_method<void>(self, "on_selection_changed", boost::ref(button)); }
	void default_on_selection_changed(CL_RadioButton *button) {}*/

	PyObject* self;
};

void export_radio_group()
{
	/*class_<CL_RadioGroup, CL_RadioGroup_wrap, boost::noncopyable>("RadioGroup")
		//.def("get_toggled", &CL_RadioGroup::get_toggled, return_value_policy<reference_existing_object>())
		//.def("get_buttons", &CL_RadioGroup::get_buttons, return_value_policy<copy_const_reference>())
		.def("add", &CL_RadioGroup::add)
		.def("remove", &CL_RadioGroup::remove)
		.add_property("count", &CL_RadioGroup::get_count)
		//.def("on_selection_changed", &CL_RadioGroup_wrap::on_selection_changed)
;*/
}
