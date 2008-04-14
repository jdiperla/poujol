#include "stdafx.h"

#include <boost/python/list.hpp>
#include <boost/python/def.hpp>
#include <boost/python/operators.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/enum.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/return_internal_reference.hpp>
#include <boost/python/implicit.hpp>
#include <boost/python/manage_new_object.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/call_method.hpp>
#include <boost/python/detail/api_placeholder.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/tuple.hpp>

#include "glumolregion.h"

#ifndef WIN32
 #include <libintl.h>
 #define _(String) gettext (String)
#else
#define _(String) String
#endif

using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	from_list_overloads, from_list, 1, 2)
        
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	scale_overloads, scale, 2, 3)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	scale_overloads2, scale, 1, 2)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	rotate_overloads, rotate, 1, 2)
    boost::python::list get_bouncing_circle();

class CGlumolRegion_wrap : public CGlumolRegion
{
public:
    PyObject *self;

    CGlumolRegion_wrap(PyObject* self_)
     : self(self_) { }

    CGlumolRegion_wrap(PyObject* self_, const CGlumolRegion &rg)
     : CGlumolRegion(rg),  self(self_) { }

    CGlumolRegion_wrap(PyObject* self_, double d1, double d2, double d3, double d4)
     : CGlumolRegion(d1, d2, d3, d4),  self(self_) { }

    CGlumolRegion_wrap(PyObject* self_, double d1, double d2, double d3, double d4, double d5, int d6)
     : CGlumolRegion(d1, d2, d3, d4, d5, d6),  self(self_) { }

     static boost::shared_ptr<CGlumolRegion> from_list(boost::python::list l)
     {
        boost::shared_ptr<CGlumolRegion> reg(new CGlumolRegion());
     	int n = boost::python::len(l);
        for(int i = 0; i < n; i++) {
            gpc_vertex_list vl;
            boost::python::list l2 = (boost::python::list) l[i];
            int m = len(l2);
            for(int j = 0; j < m; j++) {
                boost::python::list l3 = (boost::python::list) l2[j];
                vl.push_back(gpc_vertex(extract<double>(l3[0]), extract<double>(l3[1])));
            }
            reg->poly->contour.push_back(vl);
        }
        return reg;
     }

     boost::python::list to_list()
     {
        boost::python::list l;
        for(std::vector<gpc_vertex_list>::iterator it = poly->contour.begin();
            it != poly->contour.end(); it++) {
            boost::python::list l2;
            for(std::vector<gpc_vertex>::iterator it2 = it->begin();
                it2 != it->end(); it2++) {
                boost::python::list l3;
                l3.append(it2->x);
                l3.append(it2->y);
                l2.append(l3);
            }
            l.append(l2);
        }
        return l;
    }

    boost::python::list get_bouncing_circle()
    {
        boost::python::list l;
        double x, y, r;
        getBoundingCircle(&x, &y, &r);
        l.append(x);
        l.append(y);
        l.append(r);
        return l;
    }

    void on_enter_in_box(boost::shared_ptr<CGlumolSprite> sprite) {
        call_method<void>(self, "on_enter_region", sprite);
    }
             
    void default_on_enter_in_box(boost::shared_ptr<CGlumolSprite> sprite) { 
    }
    
    void on_leave_box(boost::shared_ptr<CGlumolSprite> sprite) {
        call_method<void>(self, "on_leave_region", sprite);
    }
            
    void default_on_leave_box(boost::shared_ptr<CGlumolSprite> sprite) { 
    }
            
    void on_move_in_box(boost::shared_ptr<CGlumolSprite> sprite) {
        call_method<void>(self, "on_move_in_region", sprite);
    }
             
    void default_on_move_in_box(boost::shared_ptr<CGlumolSprite> sprite) { 
    }
};

class gpc_vertex_wrap : public gpc_vertex
{
public:
    gpc_vertex_wrap(PyObject* self_)
     : self(self_) { }

    gpc_vertex_wrap(PyObject* self_, const gpc_vertex &rg)
     : gpc_vertex(rg),  self(self_) { }

    gpc_vertex_wrap(PyObject* self_, double v1, double v2)
     : gpc_vertex(v1, v2),  self(self_) { }

	gpc_vertex_wrap(PyObject* self_, list l)
     : self(self_)
	{
		x = extract<double>(l[0]);
		y = extract<double>(l[1]);
	}

	gpc_vertex_wrap(PyObject* self_, tuple l)
     : self(self_)
	{
		x = extract<double>(l[0]);
		y = extract<double>(l[1]);
	}

	list to_list()
	{
		list l;
		l.append(x);
		l.append(y);
		return l;
	}

	PyObject *self;
};

class gpc_vertex_list_wrap : public gpc_vertex_list
{
public:
    gpc_vertex_list_wrap(PyObject* self_)
     : self(self_) { }

    gpc_vertex_list_wrap(PyObject* self_, bool b)
     : gpc_vertex_list(b), self(self_) { }

    gpc_vertex_list_wrap(PyObject* self_, const gpc_vertex_list &rg)
     : gpc_vertex_list(rg),  self(self_) { }

	gpc_vertex_list_wrap(PyObject *self_, list l, bool bhole = false)
	 : self(self_)
	{
		flag = false;
		hole = bhole ? 1 : 0;
		int n = boost::python::len(l);
		for(int i = 0; i < n; i++) {
			push_back(gpc_vertex(extract<double>(l[i][0]), extract<double>(l[i][1])));
		}
	}

	list to_list()
	{
		list l;
		for(std::vector<gpc_vertex>::iterator it = begin();
			it != end(); it++) {
			list l2;
			l2.append(it->x);
			l2.append(it->y);
			l.append(l2);
		}
		return l;
	}

	PyObject *self;
};

struct glumolregion_pickle_suite : boost::python::pickle_suite
  {
    static
    boost::python::tuple
    getinitargs(CGlumolRegion const& w)
    {
        return boost::python::make_tuple();
    }
    
    static bool getstate_manages_dict() { return true; }
  };
  
void add_glumol_region()
{
    objects::class_value_wrapper<
        boost::shared_ptr<CGlumolRegion>,
        objects::make_ptr_instance<CGlumolRegion,
            objects::pointer_holder<boost::shared_ptr<CGlumolRegion>,CGlumolRegion>
        >
    >();
            
    class_<gpc_vertex, gpc_vertex_wrap>("Vertex")
		.def(init<>())
		.def(init<double, double>())
		.def(init<list>())
		.def_readwrite("x", &gpc_vertex::x)
		.def_readwrite("y", &gpc_vertex::y)
		.def("to_list", &gpc_vertex_wrap::to_list)
	;

	implicitly_convertible<gpc_vertex, list>();

    const char *class_contours_doc = _(
        "The class Contour represents a contour of a region. Basically, it's a list of points.\n"
        "You can create a contour from a list and convert it to a Python list.");

    const char *contour_init_doc = _(
        "Creates an empty contour contour. Set 'hole' to True if the contour is an hole of a region.");
    
    const char *contour_init2_doc = _(
        "Creates a contour from a list of points (given as tuples).");

    const char *hole_doc = _(
        "Set to True when the contour is an hole of a region.");

    const char *contour_to_list = _(
        "Return the contour as a list of tuples.");

	class_<gpc_vertex_list, gpc_vertex_list_wrap>(
        "Contour", class_contours_doc, init<>())
		.def(init<bool>(contour_init_doc))
		.def(init<list, optional<bool> >(contour_init2_doc))
		.add_property("hole",
            &gpc_vertex_list::hole,
            hole_doc)
		.def(vector_indexing_suite<gpc_vertex_list>())
		// .def("from_list", &gpc_vertex_list::from_list, return_value_policy<manage_new_object>())
        // .staticmethod("from_list")
		.def("to_list",
            &gpc_vertex_list_wrap::to_list,
            contour_to_list)
	;

	class_<gpc_contours>("Contours")
		.def(vector_indexing_suite<gpc_contours>())
	;

	bool (CGlumolRegion::*fx1)(double, double) = &CGlumolRegion::is_point_in_region;
	bool (CGlumolRegion::*fx2)(CL_Point) = &CGlumolRegion::is_point_in_region;

	void (CGlumolRegion::*fx3)(double, double) = &CGlumolRegion::shift;
	void (CGlumolRegion::*fx4)(CL_Point) = &CGlumolRegion::shift;

	void (CGlumolRegion::*fx5)(double, double, CL_Point) = &CGlumolRegion::scale;
	void (CGlumolRegion::*fx6)(double, double, double, double) = &CGlumolRegion::scale;
	void (CGlumolRegion::*fx7)(double, CL_Point) = &CGlumolRegion::scale;

	void (CGlumolRegion::*fx8)(double, double, double) = &CGlumolRegion::rotate;
	void (CGlumolRegion::*fx9)(double, CL_Point) = &CGlumolRegion::rotate;

	void (CGlumolRegion::*fx10)(double, double, double, double) = &CGlumolRegion::warp_to_box;
	void (CGlumolRegion::*fx11)(CL_Rect) = &CGlumolRegion::warp_to_box;

    const char *region_doc = _(
        "The region "
        "");

    const char *union_doc = _(
        "Returns the union of 2 regions.");

    const char *intersection_doc = _(
        "Returns the intersection of 2 regions.");

    const char *equal_doc = _(
        "Returns True if the 2 specified regions represent the same area.");

    const char *diff_doc = _(
        "Returns True if the 2 specified regions don't represent the same area.");

    const char *minus_doc = _(
        "Returns the area of the first region that is not in the second region");

    const char *xor_doc = _(
        "Returns the first region xor the second region.");

    const char *contours_doc = _(
        "Returns the region contours as a list of Contour objects. See Contour class.");

    const char *from_list_doc = _(
        "Creates the region from a list. Ex : from_list([[[1, 1], [10, 1], [5, 10]]]).");

    const char *to_list_doc = _(
        "Return the region as a list. With a square at position (0, 0), you'd get "
        "[[(0, 0), (10, 0), (10, 10), (0, 10)]];");

    const char *filter_doc = _(
        "Assemble the points that are very near to each other.");

    const char *cut_doc = _(
        "Cut the region, considering the 'strangle' points.\n"
        "+-----------+\n"
        "|           |\n"                                 
        "|           |\n"
        "|           | 'strangle' point\n"
        "|           |/\n"
        "+-----------+----------+\n"          
        "            |          |\n"
        "            |          |\n"
        "            |          |\n"
        "            +----------+\n"
        "Assemble the points that are very near from each other.");

    const char *area_doc = _(
        "Compute the surface of the region");

    const char *get_center_doc = _(
        "Returns the center of the region.");

    const char *get_bouncing_circle_doc = _(
        "Returns the smallest circle that contains the entire region."
        "You get a tuple (x, y, radius).");

    const char *get_bouncing_box_doc = _(
        "Returns a Region object that is the smallest rectangle that contains the entire region.");

    const char *is_point_in_region_doc = _(
        "Checks if the given point lay inside the region.");
        
    const char *shift_doc = _(
        "Shifts the region.");

    const char *scale_doc = _(
        "Scales the region to the specified scale.");

    const char *rotate_doc = _(
        "Rotates the region.");

    const char *warp_to_box_doc = _(
        "Shifts the region.");

    const char *flip_doc = _(
        "Flips the region along the horizontal offset specified.");

    const char *flop_doc = _(
        "Flips the region along the vertical offset specified.");

    const char *is_empty_doc = _(
        "Returns True if the region is empty.");

    const char *intersects_doc = _(
        "Returns True if the region intersects the other one.");

    const char *on_enter_region_doc = _(
        "Called when a sprite with the track_position attribute set to True "
        "enters the region.");

    const char *on_leave_region_doc = _(
        "Called when a sprite with the track_position attribute set to True "
        "leaves the region.");
    
    const char *on_move_in_region_doc = _(
        "Called when a sprite with the track_position attribute set to True "
        "that already entered the region moves inside it.");
    
    class_<CGlumolRegion, CGlumolRegion_wrap> CGlumolRegion_wrapper("Region"); // boost::shared_ptr<CGlumolRegion_wrap> > CGlumolRegion_wrapper("Region");
            
        CGlumolRegion_wrapper
        .def(init<double, double, double, double>())
        .def(init<const CGlumolRegion&>())
        .def(init<double, double, double, double, double, int>())
        
        .def(self - self) //, minus_doc)
        .def(self -= self)
        .def(self + self) //, union_doc)
        .def(self += self)
        .def(self == self) //, equal_doc)
        .def(self != self) //, diff_doc)
        .def(self ^ self) //, xor_doc)
        .def(self ^= self)
        .def(self & self) //, intersection_doc)
        .def(self &= self)
        .def(self | self) //, union_doc)
        .def(self |= self)

        .def_readwrite("type", &CGlumolRegion::type)
        
        .def("add_point", &CGlumolRegion::add_point)
        
        .def("get_contours",
            &CGlumolRegion::get_contours,
            return_internal_reference<>())

        .add_property("contours",
            CGlumolRegion_wrapper.attr("get_contours"),
            contours_doc)

        .def("from_list",
            &CGlumolRegion_wrap::from_list,
            from_list_doc)

        .staticmethod("from_list")

        .def("to_list",
            &CGlumolRegion_wrap::to_list,
            to_list_doc)

        .def("filter",
            &CGlumolRegion::filter,
            filter_doc)
        
        .def("cut",
            &CGlumolRegion::cut,
            cut_doc)
        
        .add_property("num_part_connex", &CGlumolRegion::numPartConnex)
        
        .def("to_string", &CGlumolRegion::print)

        .def("area",
            &CGlumolRegion::getArea,
            area_doc)

        .def("get_center",
            &CGlumolRegion::get_center,
            get_center_doc)

        .def("get_bouncing_circle",
            &CGlumolRegion_wrap::get_bouncing_circle,
            get_bouncing_circle_doc)
        
        .def("get_bouncing_box",
            &CGlumolRegion::get_bouncing_box,
            get_bouncing_box_doc)
        
        .def("is_point_in_region",
            fx1,
            is_point_in_region_doc)
        .def("is_point_in_region",
            fx2,
            is_point_in_region_doc)

        .def("shift",
            fx3,
            shift_doc)
        .def("shift",
            fx4,
            shift_doc)

        .def("scale",
            fx5,
            scale_overloads(scale_doc))
        .def("scale",
            fx6,
            scale_doc)
        .def("scale",
            fx7,
            scale_overloads2(scale_doc))

        .def("rotate",
            fx8,
            rotate_doc)
        .def("rotate",
            fx9,
            rotate_overloads(rotate_doc))

        .def("warp_to_box",
            fx10,
            warp_to_box_doc)
        .def("warp_to_box",
            fx11,
            warp_to_box_doc)

        .def("flip",
            &CGlumolRegion::flip,
            flip_doc)
        .def("flop",
            &CGlumolRegion::flop,
            flop_doc)

        .add_property("is_empty",
            &CGlumolRegion::isEmpty,
            is_empty_doc)

        .def("intersects",
            &CGlumolRegion::intersects,
            intersection_doc)

        .def("on_enter_region",
            &CGlumolRegion::on_enter_in_box,
            &CGlumolRegion_wrap::default_on_enter_in_box,
            on_enter_region_doc)
		
        .def("on_leave_region",
            &CGlumolRegion::on_leave_box,
            &CGlumolRegion_wrap::default_on_leave_box,
            on_leave_region_doc)
        
        .def("on_move_in_region",
            &CGlumolRegion::on_move_in_box,
            &CGlumolRegion_wrap::default_on_move_in_box,
            on_move_in_region_doc)
                
        .enable_pickling()
    ;

	enum_<CGlumolRegion::region_type>("region_type")
		.value("transparent", CGlumolRegion::transparent)
		.value("walkable", CGlumolRegion::walkable)
		.value("non_walkable", CGlumolRegion::non_walkable)
	;

}
