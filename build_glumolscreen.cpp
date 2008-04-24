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

#ifdef __APPLE__
#include <sys/types.h>
#include <sys/acl.h>
#endif

#include <boost/python.hpp>
#include "children.h"
#include <boost/shared_ptr.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "glumolsprite.h"

using namespace boost::python;
using boost::shared_ptr;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	DrawText_overloads2, draw_text, 4, 5)

void export_glumolscreen_class()
{
    typedef Childrens ar;

    class_<
		Childrens,
		ChildrenZ_wrapper<CGlumolSprite, Childrens>,
		boost::noncopyable> ChildrenZ_wrap("Children");

	using boost::python::iterator; // gcc 2.96 bug workaround
	
    ChildrenZ_wrap.def("__getitem__", &ChildrenZ<CGlumolSprite, Childrens>::get_item);
    ChildrenZ_wrap.def("__setitem__", &ChildrenZ<CGlumolSprite, Childrens>::set_item);
    ChildrenZ_wrap.def("__setitem__", &ChildrenZ<CGlumolSprite, Childrens>::set_item2);
    ChildrenZ_wrap.def("append", &ChildrenZ<CGlumolSprite, Childrens>::append);
    ChildrenZ_wrap.def("__delitem__", &ChildrenZ<CGlumolSprite, Childrens>::remove);
    ChildrenZ_wrap.def("__len__", &ChildrenZ<CGlumolSprite, Childrens>::len);
    ChildrenZ_wrap.def("__iter__", iterator<ChildrenZ<CGlumolSprite, Childrens> >());
    ChildrenZ_wrap.def("__contains__", &ChildrenZ<CGlumolSprite,Childrens>::contains);
                
    objects::class_value_wrapper<
        shared_ptr<ChildrenZ<CGlumolSprite, ar> >, objects::make_ptr_instance<ChildrenZ<CGlumolSprite, ar>, objects::pointer_holder<shared_ptr<ChildrenZ<CGlumolSprite, ar> >, ChildrenZ<CGlumolSprite, ar> > >
>();

    class_<Times, boost::noncopyable>("Times", init<>())
		.def(vector_indexing_suite<Times>());
    
	class_<Hotspots, boost::noncopyable>("Hotspots", init<>())
		.def(vector_indexing_suite<Hotspots>());
        
	class_<MoveOffsets, boost::noncopyable>("MoveOffsets", init<>())
		.def(vector_indexing_suite<MoveOffsets>());
        
	class_<Orders, boost::noncopyable>("Orders", init<>())
		.def(vector_indexing_suite<Orders>());
	
    class_<CGlumolScreen> CGlumolScreen_wrap("Screen", init<>());
	CGlumolScreen_wrap
		.add_property("background_color", &CGlumolScreen::get_background_color, &CGlumolScreen::set_background_color)
		.def("draw_line", &CGlumolScreen::draw_line) 
		.def("draw_pixel", &CGlumolScreen::draw_pixel)
		.def("draw_rect", &CGlumolScreen::draw_rect)
		.def("draw_text", &CGlumolScreen::draw_text, DrawText_overloads2())

		.def("get_children", &CGlumolScreen::get_children,
							 return_value_policy<reference_existing_object >())

        .add_property("children",
			CGlumolScreen_wrap.attr("get_children"))

		.def("sort_z",  &CGlumolScreen::sort_z)
        .add_property("current_font",
            &CGlumolScreen::get_current_font,
            &CGlumolScreen::set_current_font)
		;
} 
