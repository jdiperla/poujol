#include "stdafx.h"
#include <boost/python.hpp>
#include <ClanLib/gui.h>
#include <ClanLib/core.h>

using namespace boost::python;

struct CL_ListBox_wrap: public CL_ListBox
{
	CL_ListBox_wrap(PyObject* self_, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_ListBox(parent, style),
                self(self_)
	{
		init_slots();
	}

	void init_slots()
	{
		CL_Slot s1 = sig_highlighted().connect(this, &CL_ListBox_wrap::on_highlighted);
		CL_Slot s2 = sig_activated().connect(this, &CL_ListBox_wrap::on_activated);
		CL_Slot s3 = sig_selection_changed().connect(this, &CL_ListBox_wrap::on_selection_changed);
		CL_Slot s4 = sig_item_added().connect(this, &CL_ListBox_wrap::on_item_added);
		CL_Slot s5 = sig_item_removed().connect(this, &CL_ListBox_wrap::on_item_removed);
		CL_Slot s6 = sig_clear().connect(this, &CL_ListBox_wrap::on_clear);
	}

	CL_ListBox_wrap(PyObject* self_, const CL_Rect &rect, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_ListBox(rect, parent, style),
                self(self_)
        { init_slots(); }

	void on_highlighted(int i) { call_method<void>(self, "on_highlighted", i); }
	void default_on_highlighted(int i) {}
	void on_activated(int i) { call_method<int>(self, "on_activated", i); }
	void default_on_activated(int i) {}
    void on_selection_changed() { call_method<void>(self, "on_selection_changed"); }
	void default_on_selection_changed() {}
	void on_item_added(int i) { call_method<void>(self, "on_item_added", i); }
	void default_on_item_added(int i) {}
	void on_item_removed(int i) { call_method<void>(self, "on_item_removed", i); }
	void default_on_item_removed(int i) {}
	void on_clear() { call_method<void>(self, "on_clear"); }
	void default_on_clear() {}

	PyObject* self;
};

void export_listbox()
{
	int (CL_ListBox::*fx1)(CL_ListItem*, int, bool) = &CL_ListBox::insert_item;
	int (CL_ListBox::*fx2)(const std::string& text, int) = &CL_ListBox::insert_item;
	void (CL_ListBox::*fx3)(CL_ListItem*, int) = &CL_ListBox::change_item;
	void (CL_ListBox::*fx4)(const std::string& text, int) = &CL_ListBox::change_item;

	class_<CL_ListBox, bases<CL_Component>, CL_ListBox_wrap, boost::noncopyable> CL_ListBox_wrapper("ListBox", init<CL_Component*, optional<CL_StyleManager*> >());
	CL_ListBox_wrapper
		.def(init<const CL_Rect&, CL_Component*, optional<CL_StyleManager*> >())
		.add_property("count", &CL_ListBox::get_count)
		.def("get_current_text", &CL_ListBox::get_current_text, return_value_policy<copy_const_reference>())
		//.def("get_item", &CL_ListBox::get_item, return_value_policy<reference_existing_object>())
		.def("get_text", &CL_ListBox::get_text, return_value_policy<copy_const_reference>())
		.def("get_current_text", &CL_ListBox::get_current_text, return_value_policy<copy_const_reference>())
		.add_property("selected", &CL_ListBox::is_selected, &CL_ListBox::set_selected)
		.add_property("selected", &CL_ListBox::is_multi_selection, &CL_ListBox::set_multi_selection)
		.add_property("current_item", &CL_ListBox::get_current_item, &CL_ListBox::set_current_item)
		.add_property("item_height", &CL_ListBox::get_item_height, &CL_ListBox::set_item_height)
		.add_property("top_item", &CL_ListBox::get_top_item, &CL_ListBox::set_top_item)
		.add_property("text",
			CL_ListBox_wrapper.attr("get_text"))
		.add_property("current_text",
			CL_ListBox_wrapper.attr("get_current_text"))
		.def("insert_item", fx1)
		.def("insert_item", fx2)
		.def("remove_item", &CL_ListBox::remove_item)
		.def("change_item", fx3)
		.def("change_item", fx4)
		.def("clear_selection", &CL_ListBox::clear_selection)
		.def("select_all", &CL_ListBox::select_all)
		.def("invert_selection", &CL_ListBox::invert_selection)
		.def("sort", &CL_ListBox::sort)
		.def("clear", &CL_ListBox::clear)
		.def("get_items", &CL_ListBox::get_items, return_value_policy<copy_non_const_reference>())
//		.def("get_selected_items", &CL_ListBox::get_selected_items, return_value_policy<copy_const_reference>())

		.def("on_highlighted", &CL_ListBox_wrap::on_highlighted)
		.def("on_activated", &CL_ListBox_wrap::on_activated)
		.def("on_selection_changed", &CL_ListBox_wrap::on_selection_changed)
		.def("on_item_added", &CL_ListBox_wrap::on_item_added)
		.def("on_item_removed", &CL_ListBox_wrap::on_item_removed)
		.def("on_clear", &CL_ListBox_wrap::on_clear)
    ;
}
