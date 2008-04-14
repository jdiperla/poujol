#include "stdafx.h"
#include <boost/python/list.hpp>
#include <boost/python/def.hpp>
#include <boost/python/operators.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/enum.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/copy_non_const_reference.hpp>
#include <boost/python/exception_translator.hpp>
#include <boost/python/return_internal_reference.hpp>
#include <boost/python/implicit.hpp>
#include <boost/python/manage_new_object.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/tuple.hpp>

#include "misc_classes.h"
#include "glumolregion.h"
#include "glumolexception.h"

#ifndef WIN32
 #include <libintl.h>
 #define _(String) gettext (String)
#else
#define _(String) String
#endif

using namespace boost::python;

struct cpoint_pickle_suite : boost::python::pickle_suite
  {
    static
    boost::python::tuple
    getinitargs(CL_Pointf const& w)
    {
        return boost::python::make_tuple(w.x, w.y);
    }
  };

const char *point_doc = _(
    "The class Point represents a geometric point.\n"
    "It simply contains the x and y coordinates, stored as floats.");

const char *point__doc = _(
    "The class Point represents a geometric point.\n"
    "It simply contains the x and y coordinates, stored as integers.");

const char *x_doc = _("The x coordinate.");

const char *y_doc = _("The y coordinate.");

const char *rotate_doc = _(
    "Returns the image of the point after a rotation around "
    "the point 'hotspot' of 'angle' degrees."); 

struct CL_Pointf_wrap : public CL_Pointf
{
public:
    CL_Pointf_wrap(PyObject* self_) : self(self_) { } 
    CL_Pointf_wrap(PyObject* self_, float f1, float f2) : CL_Pointf(f1, f2), self(self_)  { }
    CL_Pointf_wrap(PyObject* self_, const CL_Pointf& p1) : CL_Pointf(p1), self(self_)  { }
    CL_Pointf_wrap(PyObject* self_, tuple t) : self(self_) {
        x = extract<float>(t[0]);
        y = extract<float>(t[1]);
    }
    
    PyObject *self;
};

void export_cpoint_class()
{
    class_<CL_Pointf, CL_Pointf_wrap>("Point", point_doc, init<>())
        .def(init<float, float>()) 
        .def(init<const CL_Pointf&>())
        .def(init<tuple>()) 

        .def_readwrite("x", &CL_Pointf::x, x_doc)
        .def_readwrite("y", &CL_Pointf::y, y_doc)
		
        .def(self + self)
        .def(self += self)
        .def(self - self)
        .def(self -= self)
        .def(self == self)
		
        .def("rotate",
            &CL_Pointf::rotate,
            rotate_doc)
        
        .def_pickle(cpoint_pickle_suite())
    ;
}

struct csize_pickle_suite : boost::python::pickle_suite
{
    static
    boost::python::tuple
    getinitargs(CL_Sizef const& w)
    {
        return boost::python::make_tuple(w.width, w.height);
    }
};

const char *size_doc = _(
    "The class Size represents a geometric size.\n"
    "It simply contains the width and height attributes, stored as floats.");

const char *size__doc = _(
    "The class Size_ represents a geometric size.\n"
    "It simply contains the width and height attributes, stored as integers.");

const char *width_doc = _("The width attribute.");

const char *height_doc = _("The height attribute.");

class CL_Sizef_wrap : public CL_Sizef
{
public:
    CL_Sizef_wrap(PyObject *_self) : self(_self) { } 
    CL_Sizef_wrap(PyObject *_self, float f1, float f2) : CL_Sizef(f1, f2), self(_self) { }
    CL_Sizef_wrap(PyObject *_self, const CL_Sizef& p1) : CL_Sizef(p1), self(_self) { }
    CL_Sizef_wrap(PyObject *_self, tuple t) : self(_self) {
        width = extract<float>(t[0]);
        height = extract<float>(t[1]);
    }
    
    PyObject *self;
};

void export_csize_class()
{
    class_<CL_Sizef, CL_Sizef_wrap>("Size", init<float, float>())
        .def(init<const CL_Sizef&>())
        .def(init<tuple>())
        .def(init<>())
        .def_readwrite("width", &CL_Sizef::width)
        .def_readwrite("height", &CL_Sizef::height)
        .def(self + self)
        .def(self += self)
        .def(self - self)
        .def(self -= self)
        .def(self == self)
        
        .def_pickle(csize_pickle_suite())
    ;
}

void Set_height(CL_Rectf& rect, int sizeheight)
{ 
    CL_Size s((int)rect.get_width(),(int) sizeheight); 
    rect.set_size(s); 
}

void Set_width(CL_Rectf& rect, int sizewidth)
{ 
    CL_Size s((int)sizewidth,(int) rect.get_height()); 
    rect.set_size(s); 
}

CL_Rectf (CL_Rectf::*get_rot_bounds1)(const CL_Pointf&, float)  const = &CL_Rectf::get_rot_bounds; 
CL_Rectf (CL_Rectf::*get_rot_bounds2)(CL_Origin,float,float,float) const = &CL_Rectf::get_rot_bounds; 

struct crect_pickle_suite : boost::python::pickle_suite
{
    static
    boost::python::tuple
    getinitargs(CL_Rectf const& w)
    {
        return boost::python::make_tuple(w.left, w.top, w.right, w.bottom);
    }
};

const char *rect_doc = _(
     "A class for manipulating rectangles. The coordinates are stored as floats.");

const char *rect__doc = _(
     "A class for manipulating rectangles. The coordinates are stored as integers.");

const char *left_doc = _(
    "x coordinate of the top-level corner of the rectangle.");

const char *top_doc = _(
    "y coordinate of the top-level corner of the rectangle.");

const char *right_doc = _(
    "x coordinate of the top left corner of the rectangle.");

const char *bottom_doc = _(
    "x coordinate of the top left corner of the rectangle.");

const char *rect_width_doc = _(
    "The rectangle width.");

const char *rect_height_doc = _(
    "The rectangle height.");

const char *rect_size_doc = _(
    "The rectangle size.");

const char *union_doc = _(
    "Returns the bounding box of this rectangle and the one passed in as parameter.");

const char *is_inside_doc = _(
    "Returns True if the given point is inside the rectangle (or on its boundary) and False otherwise.");

const char *is_overlapped_doc = _(
    "Returns True if rectangle passed is overlapping or inside this rectangle.");

const char *normalize_doc = _(
    "Normalize the rectangle.");

const char *get_rotation_bounds_doc = _(
    "Returns another Rect object containing a rotated version of this one.");

void export_crect_class()
{
    class_<CL_Rectf>("Rect", rect_doc)
        .def(init<float, float, float, float>())
        .def(init<const CL_Pointf&, const CL_Sizef&>())
        .def(init<const CL_Rectf&>())
        .def_readwrite("left", &CL_Rectf::left, left_doc)
        .def_readwrite("top", &CL_Rectf::top, top_doc)
        .def_readwrite("right", &CL_Rectf::right, right_doc)
        .def_readwrite("bottom", &CL_Rectf::bottom, bottom_doc)

        .add_property("height" , &CL_Rectf::get_height, &Set_height, rect_width_doc)
        .add_property("width", &CL_Rectf::get_width, &Set_width, rect_height_doc)	
        .add_property("size", &CL_Rectf::get_size, &CL_Rectf::set_size, rect_size_doc)

        .def(self + self)
        .def(self += self)
        .def(self - self)
        .def(self -= self)
    	.def(self == self)
		
        .def("union", &CL_Rectf::calc_union, union_doc)
        .def("is_inside", &CL_Rectf::is_inside, is_inside_doc)
        .def("is_overlapped", &CL_Rectf::is_overlapped, is_overlapped_doc)
        .def("normalize", &CL_Rectf::normalize, normalize_doc)
        .def("get_rotation_bounds", get_rot_bounds1, get_rotation_bounds_doc)
        .def("get_rotation_bounds", get_rot_bounds2, get_rotation_bounds_doc)
        
        .def_pickle(crect_pickle_suite())
    ;
}

struct cpoint__pickle_suite : boost::python::pickle_suite
  {
    static
    boost::python::tuple
    getinitargs(CL_Point const& w)
    {
        return boost::python::make_tuple(w.x, w.y);
    }
  };

void export_cpoint__class()
{
    class_<CL_Point>("Point_", point__doc, init<>())
        .def(init<int, int>()) 
        .def(init<const CL_Point&>())

        .def_readwrite("x", &CL_Point::x, x_doc)
        .def_readwrite("y", &CL_Point::y, y_doc)
		
        .def(self + self)
        .def(self += self)
        .def(self - self)
        .def(self -= self)
        .def(self == self)
		
        .def("rotate", &CL_Point::rotate, rotate_doc)
        
        .def_pickle(cpoint__pickle_suite());
    ;
}

struct csize__pickle_suite : boost::python::pickle_suite
  {
    static
    boost::python::tuple
    getinitargs(CL_Size const& w)
    {
        return boost::python::make_tuple(w.width, w.height);
    }
  };

void export_csize__class()
{
    class_<CL_Size>("Size_", size__doc, init<int, int>())
        .def(init<const CL_Size&>())

        .def_readwrite("width", &CL_Size::width, width_doc)
        .def_readwrite("height", &CL_Size::height, height_doc)
		
        .def(self + self)
        .def(self += self)
        .def(self - self)
        .def(self -= self)
        .def(self == self)
		
        .def_pickle(csize__pickle_suite())
    ;
}

void Set_height_(CL_Rect& rect, int sizeheight)
{
    CL_Size s((int)rect.get_width(),(int) sizeheight); 
    rect.set_size(s); 
}

void Set_width_(CL_Rect& rect, int sizewidth)
{ 
    CL_Size s((int)sizewidth,(int) rect.get_height()); 
    rect.set_size(s); 
}

CL_Rect (CL_Rect::*get_rot_bounds3)(const CL_Point&, float)  const = &CL_Rect::get_rot_bounds; 
CL_Rect (CL_Rect::*get_rot_bounds4)(CL_Origin,int,int,float) const = &CL_Rect::get_rot_bounds; 

struct crect__pickle_suite : boost::python::pickle_suite
{
    static
    boost::python::tuple
    getinitargs(CL_Rect const& w)
    {
        return boost::python::make_tuple(w.left, w.top, w.right, w.bottom);
    }
};
  
void export_crect__class()
{
    class_<CL_Rect>("Rect_", rect__doc)
        .def(init<int,int,int,int>())
        .def(init<const CL_Point&, const CL_Size&>())
        .def(init<const CL_Rect&>())
	
        .def_readwrite("left", &CL_Rect::left, left_doc)
        .def_readwrite("top", &CL_Rect::top, top_doc)
        .def_readwrite("right", &CL_Rect::right, right_doc)
        .def_readwrite("bottom", &CL_Rect::bottom, bottom_doc)

        .add_property("height" , &CL_Rect::get_height, &Set_height_, rect_height_doc)
        .add_property("width", &CL_Rect::get_width, &Set_width_, rect_width_doc)	
        .add_property("size", &CL_Rect::get_size, &CL_Rect::set_size, rect_size_doc)

        .def(self + self)
        .def(self += self)
        .def(self - self)
        .def(self -= self)
        .def(self == self)
		
        .def("union", &CL_Rect::calc_union, union_doc)
        .def("is_inside", &CL_Rect::is_inside, is_inside_doc)
        .def("is_overlapped", &CL_Rect::is_overlapped, is_overlapped_doc)
        .def("normalize", &CL_Rect::normalize, normalize_doc)
        .def("get_rotation_bounds", get_rot_bounds1, get_rotation_bounds_doc)
        .def("get_rotation_bounds", get_rot_bounds2, get_rotation_bounds_doc)
        
        .def_pickle(crect__pickle_suite())
    ;
}

int Get_Color(CL_Color& color)
{
    return color.color; 
}

struct color_pickle_suite : boost::python::pickle_suite
{
    static
    boost::python::tuple
    getinitargs(CL_Color const& w)
    {
        return boost::python::make_tuple(w.get_red(), w.get_green(),
                                         w.get_blue(), w.get_alpha());
    }
};

const char *color_doc = _(
    "A colour is an object representing a combination of Red"
    ", Green, Blue and Transparency (RGBA) intensity values, and is used to "
    "determine drawing colours.\nValid RGB values are in "
    "the range 0 to 255.");

const char *b_doc = _(
    "The color blue component.");

const char *r_doc = _(
    "The color red component.");

const char *g_doc = _(
    "The color green component.");

const char *alpha_doc = _(
    "The color transparency.");

const char *color_color_doc = _(
    "The color as an integer.");

void export_color_class()
{
    class_<CL_Color>("Color", color_doc)
        .def(init<int,int,int,optional<int> >())
        .add_property("alpha",
            &CL_Color::get_alpha,
            &CL_Color::set_alpha,
            alpha_doc)
        .add_property("b",
            &CL_Color::get_blue,
            &CL_Color::set_blue,
            b_doc)
        .add_property("g",
            &CL_Color::get_green,
            &CL_Color::set_green,
            g_doc) 
        .add_property("r",
            &CL_Color::get_red,
            &CL_Color::set_red,
            r_doc) 
        .add_property("color",
            &Get_Color,
            &CL_Color::set_color,
            color_color_doc) 
		
        // .def( int(self))
        // .def(self + self)
	
        .def_readonly("aliceblue", &CL_Color::aliceblue)
        .def_readonly("antiquewhite", &CL_Color::antiquewhite)
        .def_readonly("aqua", &CL_Color::aqua)
        .def_readonly("aquamarine", &CL_Color::aquamarine)
        .def_readonly("azure", &CL_Color::azure)
        .def_readonly("beige", &CL_Color::beige)
        .def_readonly("bisque", &CL_Color::bisque)
        .def_readonly("black", &CL_Color::black)
        .def_readonly("blanchedalmond", &CL_Color::blanchedalmond)
        .def_readonly("blue", &CL_Color::blue)
        .def_readonly("blueviolet", &CL_Color::blueviolet)
        .def_readonly("brown", &CL_Color::brown)
        .def_readonly("burlywood", &CL_Color::burlywood)
        .def_readonly("cadetblue", &CL_Color::cadetblue)
        .def_readonly("chartreuse", &CL_Color::chartreuse)
        .def_readonly("chocolate", &CL_Color::chocolate)
        .def_readonly("coral", &CL_Color::coral)
        .def_readonly("cornflowerblue", &CL_Color::cornflowerblue)
        .def_readonly("cornsilk", &CL_Color::cornsilk)
        .def_readonly("crimson", &CL_Color::crimson)
        .def_readonly("cyan", &CL_Color::cyan)
        .def_readonly("darkblue", &CL_Color::darkblue)
        .def_readonly("darkcyan", &CL_Color::darkcyan)
        .def_readonly("darkgoldenrod", &CL_Color::darkgoldenrod)
        .def_readonly("darkgray", &CL_Color::darkgray)
        .def_readonly("darkgreen", &CL_Color::darkgreen)
        .def_readonly("darkgrey", &CL_Color::darkgrey)
        .def_readonly("darkkhaki", &CL_Color::darkkhaki)
        .def_readonly("darkmagenta", &CL_Color::darkmagenta)
        .def_readonly("darkolivegreen", &CL_Color::darkolivegreen)
        .def_readonly("darkorange", &CL_Color::darkorange)
        .def_readonly("darkorchid", &CL_Color::darkorchid)
        .def_readonly("darkred", &CL_Color::darkred)
        .def_readonly("darksalmon", &CL_Color::darksalmon)
        .def_readonly("darkseagreen", &CL_Color::darkseagreen)
        .def_readonly("darkslateblue", &CL_Color::darkslateblue)
        .def_readonly("darkslategray", &CL_Color::darkslategray)
        .def_readonly("darkslategrey", &CL_Color::darkslategrey)
        .def_readonly("darkturquoise", &CL_Color::darkturquoise)
        .def_readonly("darkviolet", &CL_Color::darkviolet)
        .def_readonly("deeppink", &CL_Color::deeppink)
        .def_readonly("deepskyblue", &CL_Color::deepskyblue)
        .def_readonly("dimgray", &CL_Color::dimgray)
        .def_readonly("dimgrey", &CL_Color::dimgrey)
        .def_readonly("dodgerblue", &CL_Color::dodgerblue)
        .def_readonly("firebrick", &CL_Color::firebrick)
        .def_readonly("floralwhite", &CL_Color::floralwhite)
        .def_readonly("forestgreen", &CL_Color::forestgreen)
        .def_readonly("fuchsia", &CL_Color::fuchsia)
        .def_readonly("gainsboro", &CL_Color::gainsboro)
        .def_readonly("ghostwhite", &CL_Color::ghostwhite)
        .def_readonly("gold", &CL_Color::gold)
        .def_readonly("goldenrod", &CL_Color::goldenrod)
        .def_readonly("gray", &CL_Color::gray)
        .def_readonly("grey", &CL_Color::grey)
        .def_readonly("green", &CL_Color::green)
        .def_readonly("greenyellow", &CL_Color::greenyellow)
        .def_readonly("honeydew", &CL_Color::honeydew)
        .def_readonly("hotpink", &CL_Color::hotpink)
        .def_readonly("indianred", &CL_Color::indianred)
        .def_readonly("indigo", &CL_Color::indigo)
        .def_readonly("ivory", &CL_Color::ivory)
        .def_readonly("khaki", &CL_Color::khaki)
        .def_readonly("lavender", &CL_Color::lavender)
        .def_readonly("lavenderblush", &CL_Color::lavenderblush)
        .def_readonly("lawngreen", &CL_Color::lawngreen)
        .def_readonly("lemonchiffon", &CL_Color::lemonchiffon)
        .def_readonly("lightblue", &CL_Color::lightblue)
        .def_readonly("lightcoral", &CL_Color::lightcoral)
        .def_readonly("lightcyan", &CL_Color::lightcyan)
        .def_readonly("lightgoldenrodyellow", &CL_Color::lightgoldenrodyellow)
        .def_readonly("lightgray", &CL_Color::lightgray)
        .def_readonly("lightgreen", &CL_Color::lightgreen)
        .def_readonly("lightgrey", &CL_Color::lightgrey)
        .def_readonly("lightpink", &CL_Color::lightpink)
        .def_readonly("lightsalmon", &CL_Color::lightsalmon)
        .def_readonly("lightseagreen", &CL_Color::lightseagreen)
        .def_readonly("lightskyblue", &CL_Color::lightskyblue)
        .def_readonly("lightslategray", &CL_Color::lightslategray)
        .def_readonly("lightslategrey", &CL_Color::lightslategrey)
        .def_readonly("lightsteelblue", &CL_Color::lightsteelblue)
        .def_readonly("lightyellow", &CL_Color::lightyellow)
        .def_readonly("lime", &CL_Color::lime)
        .def_readonly("limegreen", &CL_Color::limegreen)
        .def_readonly("linen", &CL_Color::linen)
        .def_readonly("magenta", &CL_Color::magenta)
        .def_readonly("maroon", &CL_Color::maroon)
        .def_readonly("mediumaquamarine", &CL_Color::mediumaquamarine)
        .def_readonly("mediumblue", &CL_Color::mediumblue)
        .def_readonly("mediumorchid", &CL_Color::mediumorchid)
        .def_readonly("mediumpurple", &CL_Color::mediumpurple)
        .def_readonly("mediumseagreen", &CL_Color::mediumseagreen)
        .def_readonly("mediumslateblue", &CL_Color::mediumslateblue)
        .def_readonly("mediumspringgreen", &CL_Color::mediumspringgreen)
        .def_readonly("mediumturquoise", &CL_Color::mediumturquoise)
        .def_readonly("mediumvioletred", &CL_Color::mediumvioletred)
        .def_readonly("midnightblue", &CL_Color::midnightblue)
        .def_readonly("mintcream", &CL_Color::mintcream)
        .def_readonly("mistyrose", &CL_Color::mistyrose)
        .def_readonly("moccasin", &CL_Color::moccasin)
        .def_readonly("navajowhite", &CL_Color::navajowhite)
        .def_readonly("navy", &CL_Color::navy)
        .def_readonly("oldlace", &CL_Color::oldlace)
        .def_readonly("olive", &CL_Color::olive)
        .def_readonly("olivedrab", &CL_Color::olivedrab)
        .def_readonly("orange", &CL_Color::orange)
        .def_readonly("orangered", &CL_Color::orangered)
        .def_readonly("orchid", &CL_Color::orchid)
        .def_readonly("palegoldenrod", &CL_Color::palegoldenrod)
        .def_readonly("palegreen", &CL_Color::palegreen)
        .def_readonly("paleturquoise", &CL_Color::paleturquoise)
        .def_readonly("palevioletred", &CL_Color::palevioletred)
        .def_readonly("papayawhip", &CL_Color::papayawhip)
        .def_readonly("peachpuff", &CL_Color::peachpuff)
        .def_readonly("peru", &CL_Color::peru)
        .def_readonly("pink", &CL_Color::pink)
        .def_readonly("plum", &CL_Color::plum)
        .def_readonly("powderblue", &CL_Color::powderblue)
        .def_readonly("purple", &CL_Color::purple)
        .def_readonly("rosybrown", &CL_Color::rosybrown)
        .def_readonly("royalblue", &CL_Color::royalblue)
        .def_readonly("red", &CL_Color::red)
        .def_readonly("saddlebrown", &CL_Color::saddlebrown)
        .def_readonly("salmon", &CL_Color::salmon)
        .def_readonly("sandybrown", &CL_Color::sandybrown)
        .def_readonly("seagreen", &CL_Color::seagreen)
        .def_readonly("seashell", &CL_Color::seashell)
        .def_readonly("sienna", &CL_Color::sienna)
        .def_readonly("silver", &CL_Color::silver)
        .def_readonly("skyblue", &CL_Color::skyblue)
        .def_readonly("slateblue", &CL_Color::slateblue)
        .def_readonly("slategray", &CL_Color::slategray)
        .def_readonly("slategrey", &CL_Color::slategrey)
        .def_readonly("snow", &CL_Color::snow)
        .def_readonly("springgreen", &CL_Color::springgreen)
        .def_readonly("steelblue", &CL_Color::steelblue)
        .def_readonly("tan", &CL_Color::tan)
        .def_readonly("teal", &CL_Color::teal)
        .def_readonly("thistle", &CL_Color::thistle)
        .def_readonly("tomato", &CL_Color::tomato)
        .def_readonly("turquoise", &CL_Color::turquoise)
        .def_readonly("violet", &CL_Color::violet)
        .def_readonly("wheat", &CL_Color::wheat)
        .def_readonly("white", &CL_Color::white)
        .def_readonly("whitesmoke", &CL_Color::whitesmoke)
        .def_readonly("yellow", &CL_Color::yellow)
        .def_readonly("yellowgreen", &CL_Color::yellowgreen)
        
        .def_pickle(color_pickle_suite())
    ; 
}

void export_enums()
{
	enum_<CL_Origin>("origin")
		.value("origin_top_left", origin_top_left)
        .value("origin_top_right", origin_top_right)
		.value("origin_top_center", origin_top_center)
        .value("origin_bottom_left", origin_bottom_left)
		.value("origin_bottom_right", origin_bottom_right)
        .value("origin_bottom_center", origin_bottom_center)
		.value("origin_center", origin_center)
        .value("origin_center_left", origin_center_left)
		.value("origin_center_right", origin_center_right)
        ;

	enum_<CL_BlendFunc>("blend_mode")
	 	.value("blend_zero",blend_zero)                      
        .value("blend_one",blend_one)                 
	    .value("blend_dest_color",  blend_dest_color)
	    .value("blend_one_minus_dest_color", blend_one_minus_dest_color)
	    .value("blend_src_alpha", blend_src_alpha)
	    .value("blend_one_minus_src_alpha",   blend_one_minus_src_alpha)
	    .value("blend_dst_alpha",blend_dst_alpha)
	    .value("blend_one_minus_dest_alpha",blend_one_minus_dest_alpha)
	    .value("blend_src_alpha_saturate",blend_src_alpha_saturate) 
	    .value("blend_constant_color", blend_constant_color) 
	    .value("blend_one_minus_constant_color",blend_one_minus_constant_color)
	    .value("blend_constant_alpha",blend_constant_alpha)
	    .value("blend_one_minus_constant_alpha", blend_one_minus_constant_alpha)
		;

	enum_<rotation_dir>("rotation_dir")
		.value("clockwise", clockwise)
        .value("counter_clockwise", counterclockwise)
        ;

	enum_<play_mode>("play_mode")
	//	.value("backward", backward)
		.value("loop", loop)
        .value("pingpong", pingpong)
        ;
}

void clanlib_translator(CL_Error const& x) {
	PyErr_SetString(PyExc_UserWarning, x.message.c_str());
}

void glumol_translator(GlumolException const& x) {
	PyErr_SetString(PyExc_UserWarning, x.message.c_str());
}

void export_exceptions()
{
	register_exception_translator<CL_Error>(clanlib_translator);
	register_exception_translator<GlumolException>(glumol_translator);
}


void export_console_class()
{
	class_<console_stdout>("_console")
		.def("write", &console_stdout::write)
		;
}


