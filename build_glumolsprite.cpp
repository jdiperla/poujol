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

#ifdef __APPLE__
#include <sys/types.h>
#include <sys/acl.h>
#endif

#include "stdafx.h"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/iterator.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>
#include <list>

#include "children.h"
#include "glumolsprite.h"
#include "glumolfont.h"
#include "glumolzbuffer.h"
#include "build_glumolsprite.h"

#ifndef WIN32
 #include <libintl.h>
 #define _(String) gettext (String)
#else
#define _(String) String
#endif

using namespace boost::python;
using boost::shared_ptr;

template<class T>
struct glumolsprite_pickle_suite : boost::python::pickle_suite
  {
    static
    boost::python::tuple
    getinitargs(CGlumolSprite_wrap<T> const& w)
    {
		/*
        if(w.parent.get()) {
			//for(Childrens::iterator i = w.parent.get()->children.begin();
			//	i != w.parent.get()->children.end();
			//	i++) {
			//	if((*i).get() == &w)
			//		return boost::python::make_tuple(*i);
			//}
		}
        else
            //return boost::python::make_tuple(object(borrowed(pare.self)));
            //return boost::python::make_tuple(); // w.parent);
        */
        if(w.parent.px) {
            CGlumolSprite_wrap<T> *parent_obj = dynamic_cast<CGlumolSprite_wrap<T>*>(w.parent.px);
            if( parent_obj != NULL ) // Means we have a Python-defined object
            {
                return boost::python::make_tuple(object(borrowed(parent_obj->self)));
            }
        }
        return boost::python::make_tuple();
        /*else
        {
            reference_existing_object::apply<const Base*>::type converter;
            final_obj = object(handle<>(convert(my_obj)));
        }*/
    }
};

void (CGlumolSprite::*fit_into1)(float, float) = &CGlumolSprite::fit_into;
void (CGlumolSprite::*fit_into2)(CL_Sizef) = &CGlumolSprite::fit_into;

void export_glumolsprite_class()
{
    class_<
        Regions,
        ChildrenZ_wrapper<CGlumolRegion, Regions>,
        boost::noncopyable> ChildrenZ_wrap("Regions");

    using boost::python::iterator; // gcc 2.96 bug workaround
        
    ChildrenZ_wrap.def("__getitem__", &ChildrenZ<CGlumolRegion, Regions>::get_item);
    ChildrenZ_wrap.def("__setitem__", &ChildrenZ<CGlumolRegion, Regions>::set_item);
    ChildrenZ_wrap.def("__setitem__", &ChildrenZ<CGlumolRegion, Regions>::set_item2);
    ChildrenZ_wrap.def("append", &ChildrenZ<CGlumolRegion, Regions>::append);
    ChildrenZ_wrap.def("__delitem__", &ChildrenZ<CGlumolRegion, Regions>::remove);
    ChildrenZ_wrap.def("__len__", &ChildrenZ<CGlumolRegion, Regions>::len);
    ChildrenZ_wrap.def("__iter__", iterator<ChildrenZ<CGlumolRegion, Regions> >());
    ChildrenZ_wrap.def("__contains__", &ChildrenZ<CGlumolRegion,Regions>::contains);
                    	
    const char * sprite_doc = _("Represents a graphical object of a game\n" \
                                "This is the base class of Scene, Character, ...\n" \
                                "To create an object, derive a class from Sprite " \
                                "and change the 'current_anim' property to change the " \
                                "image of the sprite.\n" \
                                "A sprite can have children, which simply are other " \
                                "Sprites. For example, " \
                                "the children can be the objects inside an inventory. " \
                                "In this case, the parent of every child is the " \
                                "the inventory. When you move the parent, the children " \
                                "will also move, same for scaling, rotation, etc...");
                                                
    class_<	CGlumolSprite, boost::shared_ptr<CGlumolSprite_wrap<CGlumolSprite> >, boost::noncopyable> CGlumolSprite_wrapper(
		"Sprite", sprite_doc, init<boost::shared_ptr<CGlumolSprite_wrap<CGlumolSprite> > >());
	
    const char *children_doc = _("A list of the children sprites");

    CGlumolSprite_wrapper
        .add_property("children",
                      make_function(&CGlumolSprite::get_children,
                         return_value_policy<reference_existing_object >()),
                      children_doc);

    const char *position_doc = _(
        "The position of the sprite.\n" \
        "The position is expressed relatively to the parent.\n" \
        "Note this not always is the top-left corner position but " \
        "the hotspot's one. See the 'hotspot' property. ");
    
    const char *parent_doc = _(
        "The parent sprite of... the sprite.");
                    
    const char *size_doc = _(
        "A tuple holding the size (width, height) of the sprite.");
                    
    const char *visible_doc = _(
        "Set 'visible' at False to hide the objet and his children, " \
        "at True to show it.");
                    
    const char *playing_doc = _(
        "Set 'playing' at True to animate the sprite.\n" \
        "See also the 'play_mode' property.");
                    
    const char *angle_doc = _(
        "The angle of the sprite, in degrees.\n" \
        "The rotation is done about the hotspot.");
                    
    const char *scale_doc = _(
        "A tuple holding the scale of the object.\n" \
        "The first value is the the scale factor along the X-axis, " \
        "the second one is for Y-axis.\n" \
        "Set at (1.0, 1.0) by default.\n" \
        "Set at (2.0, 2.0) for a two times bigger sprite");
                    
    const char *alpha_doc = _(
        "A float value that defines the transparency of the sprite.\n" \
        "A 0.0 value make the sprite invisible, a 1.0 value make " \
        "the sprite completely opaque.");
                    
    const char *rotate_doc = _(
        "Rotate the sprite by 'angle' degrees.\n" \
        "You can specify a rotation center.");
                    
    const char *stretch_doc = _(
        "Stretch the sprite.\n" \
        "For example, the values 2.0 and 1.0 will make the sprite two times larger.");
                    
    const char *fade_in_doc = _(
        "Make the sprite appear gradually.\n" \
        "The sprite will appear in 'times' seconds");
                    
    const char *fade_out_doc = _(
        "Make the sprite disappear gradually.\n" \
        "The sprite will disappear in 'times' seconds");
                
    const char *color_doc = _(
        "Set the color of the sprite.\n" \
        "Use it if the object is under a colored light for example.\n" \
        "A '(255, 0, 0)' value will make appear the sprite red.");
    
    const char *frame_doc = _(
        "Sets current animation frame. 0 is first frame.");
                    
    const char *currentanim_doc = _(
        "Use this to change the look of your sprite.\n" \
        "For that, use an Animation object, ex :\n" \
        "sprite = MyAnimation()\n");
                
    const char *hotspot_doc = _(
        "A tuple that holds the position of the hotspot.\n" \
        "For example, if you want the position of a character " \
        "to be the position of his right feet, set 'hotspot' at the " \
        "position of the feet, relative to the upper left corner " \
        "of the Sprite.");
                    
    const char *track_position_doc = _(
        "Set at True if you want the sprite to be noticed " \
        "when it enters (or leave) a box.");
                    
    const char *client_to_screen_doc = _(
        "Convert a point expressed in sprite coordinates to screen coordinates.");
        
    const char *screen_to_client_doc = _(
        "Convert a point in screen coordinates to sprite coordinates.");
        
    const char *is_pt_on_doc = _(
        "Returns True if 'point' is inside the sprite.\n" \
        "Returns False otherwise.");
                    
    const char *sort_doc = _(
        "Sort the children of the sprite according to their 'z' property");
                
    const char *on_key_down_doc = _(
        "Event.\n" \
        "Called when a key is pressed on the keyboard. " \
        "The function gets the value of the pressed key in the " \
        "argument 'key'.");
                
    const char *on_key_up_doc = _(
        "Event.\n" \
        "Called when a key of the keyboard is released. " \
        "The function gets the value of the released key in the " \
        "argument 'key'.");
                    
    const char *on_key_down_repeat_doc = _(
        "Event.\n" \
        "Called when a key of the keyboard is holded " \
        "The function gets the code of the pressed key " \
        "in the 'key' argument and the time " \
        "since the key was pressed " \
        "in the 'delay' argument");
                    
    const char *on_mouse_button_down_doc = _(
        "Event.\n" \
        "Called when one of mouse buttons is pressed.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n" \
        "Note that the event corresponding to the pressed button " \
        "will also be called (on_left_button_down for the left " \
        "button, on_left_button_down for the right button, etc...).");
                    
    const char *on_mouse_button_up_doc = _(
        "Event.\n" \
        "Called when one of mouse buttons is released.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n" \
        "The advertissments are the same than the ones " \
        "on_mouse_button_down.");
                
    const char *on_left_button_down_doc = _(
        "Event.\n" \
        "Called when the left button of the mouse is pressed.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n");
    
    const char *on_right_button_down_doc = _(
        "Event.\n" \
        "Called when the right button of the mouse is pressed.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n");
    
    const char *on_left_button_up_doc = _(
        "Event.\n" \
        "Called when the left button of the mouse is released.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n");
    
    const char *on_right_button_up_doc = _(
        "Event.\n" \
        "Called when the right button of the mouse is released.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n");
    
    const char *on_mouse_wheel_up_doc = _(
        "Event.\n" \
        "Called when the left button of the mouse is released.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n");

    const char *rotation_hotspot_doc = _("rotation_hotspot");
                
    const char *scale_hotspot_doc = _("scale_hotspot");

    const char *rotation_origin_doc = _("rotation_origin");
                
    const char *scale_origin_doc = _("scale_origin");

    const char *on_mouse_wheel_down_doc = _(
        "Event.\n" \
        "Called when the left button of the mouse is released.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n");
                
    const char *on_mouse_move_doc = _(
        "Event.\n" \
        "Called when the mouse is moved.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n");

    const char *on_left_button_down_repeat_doc = _(
        "Event.\n" \
        "Called when the left button of the mouse is hold pressed.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument and the time since the key was pressed " \
        "in the 'delay' argument");
                    
    const char *on_right_button_down_repeat_doc = _(
        "Event.\n" \
        "Called when the right button of the mouse is hold pressed.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument and the time since the key was pressed " \
        "in the 'delay' argument");

    const char *on_middle_button_down_repeat_doc = _(
        "Event.\n" \
        "Called when the middle button of the mouse is hold pressed.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument and the time since the key was pressed " \
        "in the 'delay' argument");

    const char *on_middle_button_down_doc = _(
        "Event.\n" \
        "Called when the middle button of the mouse is pressed.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n");

    const char *on_middle_button_up_doc = _(
        "Event.\n" \
        "Called when the middle button of the mouse is released.\n" \
        "The function gets the mouse position in the 'position' " \
        "argument as a tuple.\n");
                        
    const char *on_before_draw_doc = _(
        "Event.\n" \
        "Called when the sprite is about to be drawn.");
        
    const char *on_after_draw_doc = _(
        "Event.\n" \
        "Called just after the sprite had been drawn.");
                    
    const char *on_focus_doc = _(
        "Event.\n" \
        "Called when the sprite gains the focus.\n"
        "That occurs when the mouse moves above " \
        "the sprite. A pixel detection is used.");
		
    const char *on_lose_focus_doc = _(
        "Event.\n" \
        "Called when the sprite loses the focus.\n"
        "That occurs when the mouse 'leave' " \
        "the sprite. A pixel detection is used.");
		
    const char *on_frame_changed_doc = _(
        "Event.\n" \
        "Called when the current frame of the animation "
        "is changed. This only happens when the 'playing' " \
        "property is set at True.\n");
		
    const char *on_animation_finished_doc = _(
        "Event.\n" \
        "Called when the animation has finished playing.\n" \
        "See also play_pingpong, play_reverse and play_backward.");

    CGlumolSprite_wrapper
		.def(init<CGlumolScreen*>())
		
		//.def(init<SHARED_PTR(CGlumolSprite), int, int, CL_Color>())
		//.def(init<SHARED_PTR(CGlumolScreen), int, int, CL_Color>())
           
        .add_property("position",
            make_function(
                &CGlumolSprite_wrap<CGlumolSprite>::get_position, return_internal_reference<>()),
            &CGlumolSprite_wrap<CGlumolSprite>::set_position,
            position_doc)
		
        .add_property("parent",
			&CGlumolSprite_wrap<CGlumolSprite>::Get_Parent3,
            &CGlumolSprite_wrap<CGlumolSprite>::Set_parent,
            parent_doc)
        

		.add_property("x", &CGlumolSprite::get_x, &CGlumolSprite::set_x)
		.add_property("y", &CGlumolSprite::get_y, &CGlumolSprite::set_y)
		.add_property("z", &CGlumolSprite::get_z, &CGlumolSprite::set_z)

        .add_property("size",
            &CGlumolSprite_wrap<CGlumolSprite>::get_size,
            &CGlumolSprite_wrap<CGlumolSprite>::set_size,
            size_doc)

        .add_property("visible",
            &CGlumolSprite::is_visible,
            &CGlumolSprite::set_visible,
            visible_doc)

        .add_property("playing",
            &CGlumolSprite::is_playing,
            &CGlumolSprite::set_playing,
            playing_doc)

        .def_readwrite("pixel_test", &CGlumolSprite::pixel_test)
		
        .add_property("transparent",
            &SpriteContainer::is_transparent,
            &SpriteContainer::set_transparent)

        .add_property("zbuffer",
            &SpriteContainer::is_zbuffer,
            &SpriteContainer::set_zbuffer)

        .add_property("angle",
            &SpriteContainer::get_angle,
            &SpriteContainer::set_angle,
            angle_doc)
        
        .def("fit_into", fit_into1)
        .def("fit_into", fit_into2)

		.add_property("scale",
                    &CGlumolSprite_wrap<CGlumolSprite>::get_scale,
                    &CGlumolSprite_wrap<CGlumolSprite>::set_scale,
                    scale_doc)
                    
		.add_property("alpha",
                    &CGlumolSprite::get_alpha,
                    &CGlumolSprite::set_alpha,
                    alpha_doc)
                    
		.def("rotate", &CGlumolSprite::rotate, rotate_doc)
		
        .def("stretch", &CGlumolSprite::etirer, stretch_doc)
                    
		.def("fade_in", &CGlumolSprite::fade_in, fade_in_doc)
		
        .def("fade_out", &CGlumolSprite::fade_out, fade_out_doc)

		.add_property("color",
                    &CGlumolSprite_wrap<CGlumolSprite>::get_couleur,
                    &CGlumolSprite_wrap<CGlumolSprite>::set_couleur,
                    color_doc)
                    
		.add_property("regions",
            make_function(&CGlumolSprite_wrap<CGlumolSprite>::get_regions,
                return_value_policy<reference_existing_object >()))

        .add_property("blend_mode_src",
            &CGlumolSprite::get_blend_func_src,
            &CGlumolSprite::set_blend_func_src)

        .add_property("blend_mode_dest",
            &CGlumolSprite::get_blend_func_dest, &CGlumolSprite::set_blend_func_dest)

        .add_property("current_frame",
            &CGlumolSprite::get_frame,
            &CGlumolSprite::set_current_frame,
            frame_doc)

        .add_property("current_anim", 
            &CGlumolSprite::get_currentanim,
            &CGlumolSprite::set_currentanim,
            currentanim_doc)

		.add_property("hotspot",
            &CGlumolSprite::get_hotspot,
            &CGlumolSprite::set_hotspot,
            hotspot_doc)
                    
		.add_property("rotation_hotspot",
            &SpriteContainer::get_rotation_hotspot,
            &SpriteContainer::set_rotation_hotspot,
            rotation_hotspot_doc)
		
		.add_property("scale_hotspot",
			&SpriteContainer::get_scale_hotspot,
			&SpriteContainer::set_scale_hotspot,
			scale_hotspot_doc)

    	.add_property("rotation_origin",
            &SpriteContainer::get_rotation_origin,
            &SpriteContainer::set_rotation_origin,
            rotation_origin_doc)
		
		.add_property("scale_origin",
			&SpriteContainer::get_scale_origin,
			&SpriteContainer::set_scale_origin,
			scale_origin_doc)

		.add_property("track_position",
			&CGlumolSprite::get_track_position,
			&CGlumolSprite::set_track_position,
            track_position_doc)

        .def("center", &CGlumolSprite::center)

		.def("client_to_screen", &SpriteContainer::client_to_screen,
			client_to_screen_doc)

		.def("screen_to_client", &SpriteContainer::screen_to_client,
			screen_to_client_doc)
            
		.def("is_pt_on",
			&CGlumolSprite::is_point_in_surface,
			is_pt_on_doc)

        .def("sort_z", &CGlumolSprite::sort_z, sort_doc)
        
        .def("print_children", &CGlumolSprite::print_children)
		
		.def("on_key_down",
            &CGlumolSprite::on_key_down,
            &CGlumolSprite_wrap<CGlumolSprite>::default_on_key_down,
            on_key_down_doc)

        .def("on_key_up",
            &CGlumolSprite::on_key_up,
            &CGlumolSprite_wrap<CGlumolSprite>::default_on_key_up,
            on_key_up_doc)

        .def("on_key_down_repeat",
            &CGlumolSprite::on_key_down_repeat, &CGlumolSprite_wrap<CGlumolSprite>::default_on_key_down_repeat,
            on_key_down_repeat_doc)

        .def("on_mouse_button_down",
            &CGlumolSprite::on_mouse_button_down, &CGlumolSprite_wrap<CGlumolSprite>::default_on_mouse_button_down,
            on_mouse_button_down_doc)
            
        .def("on_mouse_button_up",
            &CGlumolSprite::on_mouse_button_up, &CGlumolSprite_wrap<CGlumolSprite>::default_on_mouse_button_up,
            on_mouse_button_up_doc)
            
        .def("on_left_button_down",
            &CGlumolSprite::on_left_button_down, &CGlumolSprite_wrap<CGlumolSprite>::default_on_left_button_down,
            on_left_button_down_doc)
            
        .def("on_right_button_down",
            &CGlumolSprite::on_right_button_down, &CGlumolSprite_wrap<CGlumolSprite>::default_on_right_button_down,
            on_right_button_down_doc)
                    
		.def("on_left_button_up",
            &CGlumolSprite::on_left_button_up, &CGlumolSprite_wrap<CGlumolSprite>::default_on_left_button_up,
            on_left_button_up_doc)
                    
		.def("on_right_button_up",
            &CGlumolSprite::on_right_button_up, &CGlumolSprite_wrap<CGlumolSprite>::default_on_right_button_up,
            on_right_button_up_doc)
                
        .def("on_middle_button_down",
            &CGlumolSprite::on_middle_button_down, &CGlumolSprite_wrap<CGlumolSprite>::default_on_middle_button_down,
            on_middle_button_down_doc)

        .def("on_middle_button_up",
            &CGlumolSprite::on_middle_button_up, &CGlumolSprite_wrap<CGlumolSprite>::default_on_middle_button_up,
            on_middle_button_up_doc)
                    
		.def("on_mouse_wheel_up",
            &CGlumolSprite::on_mouse_wheel_up, &CGlumolSprite_wrap<CGlumolSprite>::default_on_mouse_wheel_up,
            on_mouse_wheel_up_doc)
                    
		.def("on_mouse_wheel_down",
            &CGlumolSprite::on_mouse_wheel_down, &CGlumolSprite_wrap<CGlumolSprite>::default_on_mouse_wheel_down,
            on_mouse_wheel_down_doc)
                    
		.def("on_mouse_move",
            &CGlumolSprite::on_mouse_move,
            &CGlumolSprite_wrap<CGlumolSprite>::default_on_mouse_move,
            on_mouse_move_doc)
                    
		.def("on_left_button_down_repeat",
            &CGlumolSprite::on_left_button_down_repeat, &CGlumolSprite_wrap<CGlumolSprite>::default_on_left_button_down_repeat,
            on_left_button_down_repeat_doc)
		
        .def("on_right_button_down_repeat",
            &CGlumolSprite::on_right_button_down_repeat,
            &CGlumolSprite_wrap<CGlumolSprite>::default_on_right_button_down_repeat,
            on_right_button_down_repeat_doc)

        .def("on_middle_button_down_repeat",
            &CGlumolSprite::on_middle_button_down_repeat, &CGlumolSprite_wrap<CGlumolSprite>::default_on_middle_button_down_repeat,
            on_middle_button_down_repeat_doc)
                    
		.def("on_mouse_wheel_down",
            &CGlumolSprite::on_mouse_wheel_down, &CGlumolSprite_wrap<CGlumolSprite>::default_on_mouse_wheel_down,
            on_mouse_wheel_down_doc)
		
        .def("on_mouse_wheel_up",
            &CGlumolSprite::on_mouse_wheel_up, &CGlumolSprite_wrap<CGlumolSprite>::default_on_mouse_wheel_up,
            on_mouse_wheel_up_doc)
                    
		.def("on_before_draw",
            &CGlumolSprite::on_before_draw, &CGlumolSprite_wrap<CGlumolSprite>::default_on_before_draw,
            on_before_draw_doc)
                    
		.def("on_after_draw",
            &CGlumolSprite::on_after_draw,
            &CGlumolSprite_wrap<CGlumolSprite>::default_on_after_draw,
            on_after_draw_doc)
                    
		.def("on_focus",
			&CGlumolSprite::on_focus,
			&CGlumolSprite_wrap<CGlumolSprite>::default_on_focus,
			on_focus_doc)
                    
        .def("on_lose_focus",
            &CGlumolSprite::on_lose_focus , &CGlumolSprite_wrap<CGlumolSprite>::default_on_lose_focus,
            on_lose_focus_doc)
		
		.def("on_frame_changed",
			&CGlumolSprite::on_frame_changed, &CGlumolSprite_wrap<CGlumolSprite>::default_on_frame_changed,
			on_frame_changed_doc)
                    
        .def("on_animation_finished",
            &CGlumolSprite::on_animation_finished , &CGlumolSprite_wrap<CGlumolSprite>::default_on_animation_finished,
            on_animation_finished_doc)
        
        .def_pickle(glumolsprite_pickle_suite<CGlumolSprite>())
	 ;

    objects::class_value_wrapper<
        shared_ptr<CGlumolSprite>
      , objects::make_ptr_instance<CGlumolSprite, objects::pointer_holder<shared_ptr<CGlumolSprite>,CGlumolSprite> >
    >();
}
