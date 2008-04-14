#include "stdafx.h"
#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/detail/api_placeholder.hpp>
#include "glumolfont.h"

#include <libintl.h>
#define _(String) gettext (String)

using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	get_size_overloads, get_size, 1, 2)

struct font_pickle_suite : boost::python::pickle_suite
  {
    static
    boost::python::tuple
    getinitargs(CGlumolFont const& w)
    {
        return boost::python::make_tuple("Arial", 10);
    }
  };
  
struct CGlumolFont_wrap : public CGlumolFont
{
	CGlumolFont_wrap(PyObject* self_, const char* name, int w, int h = 0, bool b1 = false, bool b2 = false, bool b3 = false, bool b4 = false) :
        CGlumolFont(name, w, h, b1, b2, b3, b4), self(self_)
	{
	}

    CGlumolFont_wrap(PyObject* self_, const std::string &letters, const std::string &filename, tuple widths) :
        CGlumolFont(), self(self_)
	{
        std::vector<int> widths2;
		int n = boost::python::len(widths);
		for(int i = 0; i < n; i++) {
			widths2.push_back(extract<int>(widths[i]));
		}
        add_letters(letters.c_str(), filename.c_str(), widths2);
	}

	~CGlumolFont_wrap()
	{
	}

	PyObject* self;
};

void (CGlumolFont::*set_color)(float, float, float, float) = &CGlumolFont::set_color;
void (CGlumolFont::*set_color2)(const CL_Color&) = &CGlumolFont::set_color;
CL_Color (CGlumolFont::*get_color_)(void) const = &CGlumolFont::get_color;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(set_color_overloads, set_color, 3, 4)

void export_glumolfont()
{
    const char *font_doc = _(
        "The Font class represents a drawable font.\n"
        "To create a font, you can either :\n"
        "    * On Windows, you can use Font('Arial', 10) to create an Arial font of size 10.\n"
        "    * On all platforms, use Font(filename, letters, widths) where"
        "filename is a path of an image that contains the characters, "
        "letters is a string that contains the characters of the font, "
        "and widths is a tuple filled with the characters widths");
    
    const char *draw_doc = _(
        "Draw the string 'text' at the position 'position'.\n");

    const char *get_size_doc = _(
        "Returns the drawn size of a string.\n");

    const char *color_doc = _(
        "The font color.\n");

    const char *scale_doc = _(
        "The font scale.\n");

    const char *alpha_doc = _(
        "The font transparency.\n");

    class_<CGlumolFont, CGlumolFont_wrap, boost::noncopyable>
        CGlumolFont_wrap("Font",
                         font_doc,
                         init<const char*, int, optional<int, bool, bool, bool, bool> >());

    CGlumolFont_wrap
        .def(init<const std::string &, const std::string &, tuple>())
        .def("draw",
            &CGlumolFont::draw,
            draw_doc)
        .def("get_size",
            &CGlumolFont::get_size,
            get_size_overloads(get_size_doc))
        .def("add_letters", &CGlumolFont::add_letters)
        .def("set_color", set_color, set_color_overloads())
        .def("set_color", set_color2)
        .add_property("scale",
            &CGlumolFont::get_scale,
            &CGlumolFont::set_scale,
            scale_doc)
		.add_property("alpha",
            &CGlumolFont::get_alpha,
            &CGlumolFont::set_alpha,
            alpha_doc)
        .add_property("color",
            get_color_,
           	CGlumolFont_wrap.attr("set_color"),
            color_doc)
        .def_pickle(font_pickle_suite())
    ;       
}
