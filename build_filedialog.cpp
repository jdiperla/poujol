#include "stdafx.h"
#include <boost/python.hpp>
#include <ClanLib/gui.h>

using namespace boost::python;

struct CL_FileDialog_wrap: public CL_FileDialog
{
	CL_Slot s1, s2, s3;
	
	CL_FileDialog_wrap(PyObject* self_, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_FileDialog(parent, style),
		self(self_)
	{
		s1 = sig_file_selected().connect(this, &CL_FileDialog_wrap::on_file_selected);
		s2 = sig_file_highlighted().connect(this, &CL_FileDialog_wrap::on_file_highlighted);
		s3 = sig_dir_entered().connect(this, &CL_FileDialog_wrap::on_dir_entered);
	}

	CL_FileDialog_wrap(PyObject* self_, const std::string &title, const std::string &file,
			const std::string &filter, CL_Component *parent, CL_StyleManager *style = NULL) :
		CL_FileDialog(parent, style),
		self(self_) {
		s1 = sig_file_selected().connect(this, &CL_FileDialog_wrap::on_file_selected);
		s2 = sig_file_highlighted().connect(this, &CL_FileDialog_wrap::on_file_highlighted);
		s3 = sig_dir_entered().connect(this, &CL_FileDialog_wrap::on_dir_entered);
	}

	~CL_FileDialog_wrap()
	{
	}
        
	void on_file_selected(const std::string &s)
	{
		call_method<void>(self, "on_file_selected", boost::ref(s));
	}
	void default_on_file_selected(const std::string &s);
	void on_dir_entered(const std::string &s)
	{
		call_method<void>(self, "on_dir_entered", boost::ref(s));
	}
	void default_on_dir_entered(const std::string &s) { }
	void on_file_highlighted(const std::string &s)
	{
		call_method<void>(self, "on_file_highlighted", boost::ref(s));
	}
	void default_on_file_highlighted(const std::string &s) { }

	PyObject* self;
};

void export_filedialog()
{
	const std::string (*fx1)(CL_Component *) = &CL_FileDialog::open;
	const std::string (*fx2)(const std::string& file, const std::string& filter, CL_Component *) = &CL_FileDialog::open;
	const std::string (*fx3)(CL_Component *) = &CL_FileDialog::save;
	const std::string (*fx4)(const std::string& file, const std::string& filter, CL_Component *) = &CL_FileDialog::save;

	class_<CL_FileDialog, bases<CL_Window>, CL_FileDialog_wrap, boost::noncopyable> CL_FileDialog_wrapper("FileDialog", init<CL_Component*, optional<CL_StyleManager*> >());
	CL_FileDialog_wrapper.def(init<const char *, const char *, const char *, CL_Component*, optional<CL_StyleManager*> >())
		.def("open", fx1)
		.def("open", fx2)
		.def("save", fx3)
		.def("save", fx4)
		.def("get_file", &CL_FileDialog::get_file, return_value_policy<copy_const_reference>())
		.def("set_file", &CL_FileDialog::set_file)
		.def("get_dir", &CL_FileDialog::get_dir, return_value_policy<copy_const_reference>())
		.def("set_dir", &CL_FileDialog::set_dir)
		.def("get_filter", &CL_FileDialog::get_filter, return_value_policy<copy_const_reference>())
		.def("set_filter", &CL_FileDialog::set_filter)
		.def("get_filter", &CL_FileDialog::get_filter, return_value_policy<copy_const_reference>())
		.def("get_path", &CL_FileDialog::get_path, return_value_policy<copy_const_reference>())
		.add_property("is_hidden_files_visible",
			&CL_FileDialog::is_hidden_files_visible,
			&CL_FileDialog::show_hidden_files)
	;
	CL_FileDialog_wrapper.add_property("file",
		CL_FileDialog_wrapper.attr("get_file"),
		CL_FileDialog_wrapper.attr("set_file"))
	.add_property("filter",
		CL_FileDialog_wrapper.attr("get_filter"),
		CL_FileDialog_wrapper.attr("set_filter"))
	.add_property("dir",
		CL_FileDialog_wrapper.attr("get_dir"),
		CL_FileDialog_wrapper.attr("set_dir"))
	.add_property("path",
		CL_FileDialog_wrapper.attr("get_path"))
	.def("refresh", &CL_FileDialog::refresh)
	.def("on_file_selected",
		&CL_FileDialog_wrap::on_file_selected)
	.def("on_file_highlighted", &CL_FileDialog_wrap::on_file_highlighted)
	.def("on_dir_entered", &CL_FileDialog_wrap::on_dir_entered);
}
