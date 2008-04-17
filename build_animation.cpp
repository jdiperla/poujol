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
#include <boost/shared_ptr.hpp>
#include "glumolsprite.h"

#include <libintl.h>
#define _(String) gettext (String)

using namespace boost::python;
using boost::shared_ptr;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	DrawText_overloads, DrawText, 4, 5)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(
	add_frame_overloads, add_frame, 1, 3)

float get_integer(object obj)
{
    if(PyObject_IsInstance(obj.ptr(), (PyObject*) &PyFloat_Type))
        return (int) extract<float>(obj);
    else
        return extract<int>(obj);
}

float get_float(object obj)
{
    if(PyObject_IsInstance(obj.ptr(), (PyObject*) &PyFloat_Type))
        return extract<float>(obj);
    else
        return (float) extract<int>(obj);
}

void set_orders(CGlumolAnimation *anim, object obj)
{
    if(PyObject_IsInstance(obj.ptr(), (PyObject*) &PyList_Type)) {
        anim->orders.clear();
        int i = 0, n = PyList_Size(obj.ptr());
        while(i < n) {
            anim->orders.push_back(get_integer(obj[i++]));
        }
    }
    else if(PyObject_IsInstance(obj.ptr(), (PyObject*) &PyTuple_Type)) {
        anim->orders.clear();
        int i = 0, n = PyTuple_Size(obj.ptr());
        while(i < n) {
            anim->orders.push_back(get_integer(obj[i++]));
        }
    }
    else {
        Orders o = extract<Orders>(obj);
        anim->orders = o;
    }
}

void set_delays(CGlumolAnimation *anim, object obj)
{
    if(PyObject_IsInstance(obj.ptr(), (PyObject*) &PyList_Type)) {
        anim->times.clear();
        int i = 0, n = PyList_Size(obj.ptr());
        while(i < n) {
            anim->times.push_back(get_float(obj[i++]));
        }
    }
    else if(PyObject_IsInstance(obj.ptr(), (PyObject*) &PyTuple_Type)) {
        anim->times.clear();
        int i = 0, n = PyTuple_Size(obj.ptr());
        while(i < n) {
            anim->times.push_back(get_float(obj[i++]));
        }
    }
    else {
        Times t = extract<Times>(obj);
        anim->times = t;
    }
}

void set_move_offsets(CGlumolAnimation *anim, object obj)
{
    if(PyObject_IsInstance(obj.ptr(), (PyObject*) &PyList_Type)) {
        anim->move_offsets.clear();
        int i = 0, n = PyList_Size(obj.ptr());
        while(i < n) {
            if(PyObject_IsInstance(((object) obj[i]).ptr(), (PyObject*) &PyTuple_Type)) {
                anim->move_offsets.push_back(
                    CL_Pointf(get_float(obj[i][0]),
                              get_float(obj[i][1])));
                    i++;
            }
            else {
                anim->move_offsets.push_back(extract<CL_Pointf>(obj[i++]));
            }
        }
    }
    else if(PyObject_IsInstance(obj.ptr(), (PyObject*) &PyTuple_Type)) {
        anim->move_offsets.clear();
        int i = 0, n = PyTuple_Size(obj.ptr());
        while(i < n) {
            if(PyObject_IsInstance(((object) obj[i]).ptr(), (PyObject*) &PyTuple_Type)) {
                anim->move_offsets.push_back(
                    CL_Pointf(get_float(obj[i][0]),
                              get_float(obj[i][1])));
                i++;
            }
            else {
                anim->move_offsets.push_back(extract<CL_Pointf>(obj[i++]));
            }
        }
    }
    else {
        MoveOffsets o = extract<MoveOffsets>(obj);
        anim->move_offsets = o;
    }
}

void set_hotspots(CGlumolAnimation *anim, object obj)
{
    if(PyObject_IsInstance(obj.ptr(), (PyObject*) &PyList_Type)) {
        anim->hotspots.clear();
        int i = 0, n = PyList_Size(obj.ptr());
        while(i < n) {
            if(PyObject_IsInstance(((object) obj[i]).ptr(), (PyObject*) &PyTuple_Type)) {
                anim->hotspots.push_back(
                    CL_Pointf(get_float(obj[i][0]),
                              get_float(obj[i][1])));
                i++;
            }
            else {
                anim->hotspots.push_back(extract<CL_Pointf>(obj[i++]));
            }
        }
    }
    else if(PyObject_IsInstance(obj.ptr(), (PyObject*) &PyTuple_Type)) {
        anim->hotspots.clear();
        int i = 0, n = PyTuple_Size(obj.ptr());
        while(i < n) {
            if(PyObject_IsInstance(((object) obj[i]).ptr(), (PyObject*) &PyTuple_Type)) {
                anim->hotspots.push_back(
                    CL_Pointf(get_float(obj[i][0]),
                              get_float(obj[i][1])));
                i++;
            }
            else {
                anim->hotspots.push_back(extract<CL_Pointf>(obj[i++]));
            }
        }
    }
    else {
        Hotspots o = extract<Hotspots>(obj);
        anim->hotspots = o;
    }
}

struct CGlumolAnimation_wrap : public CGlumolAnimation
{
	CGlumolAnimation_wrap(PyObject* self_, object delays, object orders, object hotspots, object move_offsets)
		: CGlumolAnimation(),  self(self_)
    {
        if(delays)
            set_delays(delays);
        if(orders)
            set_orders(orders);
        if(hotspots)
            set_hotspots(hotspots);
        if(move_offsets)
            set_move_offsets(move_offsets);
    }

    void set_orders(object obj)
    {
        ::set_orders(this, obj);
    }

    void set_delays(object obj)
    {
        ::set_delays(this, obj);
    }
    
    void set_hotspots(object obj)
    {
        ::set_hotspots(this, obj);
    }
    
    void set_move_offsets(object obj)
    {
        ::set_move_offsets(this, obj);
    }
    PyObject* self;
};

struct CAnimation_wrap : public CAnimation
{
    CAnimation_wrap(PyObject* self_, object delays, object orders, object hotspots, object move_offsets) :
        CAnimation(), self(self_)
    {
        if(delays)
            set_delays(delays);
        if(orders)
            set_orders(orders);
        if(hotspots)
            set_hotspots(hotspots);
        if(move_offsets)
            set_move_offsets(move_offsets);
    }

    void set_orders(object obj)
    {
        ::set_orders(this, obj);
    }

    void set_delays(object obj)
    {
        ::set_delays(this, obj);
    }
    
    void set_hotspots(object obj)
    {
        ::set_hotspots(this, obj);
    }
    
    void set_move_offsets(object obj)
    {
        ::set_move_offsets(this, obj);
    }

    PyObject* self;
};

struct glumolanimation_pickle_suite : boost::python::pickle_suite
  {
    static
    boost::python::tuple
    getinitargs(CBandeAnimation const& w)
    {
      return boost::python::make_tuple();
    }
  };
  
void export_animation_class()
{
    const char *animation_doc = _(
        "The appearance of a Sprite.\n"
        "Set the current_anim property of a Sprite instance to an Animation object.");

    const char *nb_frames_doc = _(
        "The number of frames (images) of the animation.");

    const char *hotspots_doc = _(
        "An array that contains the hotspots of all frames (one hotspot for each frame).");

    const char *move_offsets_doc = _(
        "When An array that contains the 'move_offsets' of all frames.\n"
        "When a frame is displayed, the frame's 'move offset' is added to the sprite's position.");

    const char *delays_doc = _(
        "An array that contains the delays the frames will you be displayed for.\n");

    const char *orders_doc = _(
        "An array that contains the frame's order.\n");

	class_<CGlumolAnimation,
           boost::shared_ptr<CGlumolAnimation_wrap>,
           boost::noncopyable>(
               "Animation", init<object, object, object, object>( (
                   boost::python::arg("delays") = object(),
                   boost::python::arg("orders") = object(),
                   boost::python::arg("hotspots") = object(),
                   boost::python::arg("move_offsets") = object()) ) )
		.add_property("nb_frames", &CGlumolAnimation::get_nbframes, nb_frames_doc)
		.add_property("delays",
            make_function(&CGlumolAnimation::get_times,
                return_value_policy<reference_existing_object >()),
            &CGlumolAnimation_wrap::set_delays,
            delays_doc)
        .add_property("hotspots",
            make_function(&CGlumolAnimation::get_hotspots,
                return_value_policy<reference_existing_object >()),
            &CGlumolAnimation_wrap::set_hotspots,
            hotspots_doc)
        .add_property("move_offsets",
            make_function(&CGlumolAnimation::get_move_offsets,
                return_value_policy<reference_existing_object >()),
            &CGlumolAnimation_wrap::set_move_offsets,
            move_offsets_doc)
        .add_property("orders",
            make_function(&CGlumolAnimation::get_orders,
                return_value_policy<reference_existing_object >()),
            &CGlumolAnimation_wrap::set_orders,
            orders_doc)
        .def_pickle(glumolanimation_pickle_suite())
   ; 

    class_<CAnimation,
        bases<CGlumolAnimation>,
        boost::shared_ptr<CAnimation_wrap>,
        boost::noncopyable >(
            "Animation", init<object, object, object, object>( (
                         boost::python::arg("delays") = object(),
                         boost::python::arg("orders") = object(),
                         boost::python::arg("hotspots") = object(),
                         boost::python::arg("move_offsets") = object()) ) )
        .def("build", &CAnimation::build)
        .def("add_frame", &CAnimation::add_frame, add_frame_overloads())
		.add_property("delays",
            make_function(&CGlumolAnimation::get_times,
                return_value_policy<reference_existing_object >()),
            &CAnimation_wrap::set_delays,
            delays_doc)
        .add_property("hotspots",
            make_function(&CGlumolAnimation::get_hotspots,
                return_value_policy<reference_existing_object >()),
            &CAnimation_wrap::set_hotspots,
            hotspots_doc)
        .add_property("move_offsets",
            make_function(&CGlumolAnimation::get_move_offsets,
                return_value_policy<reference_existing_object >()),
            &CAnimation_wrap::set_move_offsets,
            move_offsets_doc)
        .add_property("orders",
            make_function(&CGlumolAnimation::get_orders,
                return_value_policy<reference_existing_object >()),
            &CAnimation_wrap::set_orders,
            orders_doc)
	;
        
        class_<CDrawSprite>("DrawSprite", init<>())
		.def("draw_line", &CDrawSprite::DrawLine) 
		.def("draw_pixel", &CDrawSprite::DrawPixel)
		.def("draw_rect", &CDrawSprite::DrawRect)
		.def("draw_text", &CDrawSprite::DrawText, DrawText_overloads())
		.def("fill_rect", &CDrawSprite::FillRect)
	; 
}
