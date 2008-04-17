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
#include "misc_classes.h"
#include "glumolsprite.h"
#include "glumolgame.h"
#include "glumolfont.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/Core/Math/origin.h>
#include <ClanLib/Display/Providers/provider_factory.h>
#include <time.h>
#include <exception>
#include <typeinfo>

CGlumolGame* CurrentGame = NULL;

//dumb timer

timer::timer()
{
	start();
}

void timer::start()
{
    begin = CL_System::get_time();
}

long timer::get_time_elapsed()
{
	return (CL_System::get_time() - begin) ;
}

void timer::set_time_elapsed(DWORD t)
{
	begin = CL_System::get_time() + t ;
}

struct Z_compare
{
	bool operator()( SHARED_PTR(CGlumolSprite) a,  SHARED_PTR(CGlumolSprite) b)
	{
		return (a->z < b->z);
	}
};

CGlumolSprite::CGlumolSprite(CGlumolSprite *Parent)
{
	init(Parent);
}

CGlumolSprite::CGlumolSprite(CGlumolScreen *Parent)
{
	init(NULL); //SHARED_PTR_FOO(CGlumolSprite));
}

void CGlumolSprite::build(int cx, int cy, CL_Color color)
{
	size.width = cx;
	size.height = cy;
}

void CGlumolSprite::init(CGlumolSprite *Parent)
{
	position = Position =
	hotspot = Hotspot =
	rotation_hotspot = Rotation_Hotspot =
	scale_hotspot = Scale_Hotspot = CL_Pointf(0, 0);
    rotation_origin = scale_origin = origin_top_left;
    size.width = 0.0;
    size.height = 0.0;
    Angle = 0.0;
	Scale_x = Scale_y = 1.0;
	Alpha = 1.0;
    z = 0;
    set_zbuffer(false);

	if(Parent) {
		alpha = Parent->alpha;
		scale_x = Parent->scale_x;
		scale_y = Parent->scale_y;
		angle = Parent->angle;
	}
	else {
		alpha = 1.0;
		scale_x = scale_y = 1.0;
		angle = 0.0;
	}

	red_mask = 0xff0000;
	green_mask = 0x00ff00;
	blue_mask = 0x0000ff;
	alpha_mask = 0x000000;

    // parent = Parent;
	playing = false;
	visible = true;
	pixel_test = false;
	currentanim = SHARED_PTR_FOO(CGlumolAnimation);
	drawsprite = NULL;
	update_time = fade_time = 0;
	current_order = 0;
	set_fading(false);
	delta = 1;
	animation_timer = new timer();
	dead = false;
    set_track_position(false);
}

CGlumolSprite* CGlumolSprite::get_first_animed_parent()
{
#ifdef USE_BOOST
    CGlumolSprite *Parent = parent.px;
#else
    CGlumolSprite *Parent = parent;
#endif
    while(Parent) {
        if(Parent->currentanim)
            return this;
        #ifdef USE_BOOST
            Parent = Parent->parent.px;
        #else
            Parent = Parent->parent;
        #endif
    }
    return NULL;
}

void CGlumolSprite::center()
{
    CGlumolSprite *Parent = get_first_animed_parent();
    if(Parent) {
        set_position(CL_Pointf((Parent->size.width - size.width) / 2,
                               (Parent->size.height - size.height) / 2));
    }
    else {
        set_position(CL_Pointf((CurrentGame->get_width() - size.width) / 2,
                               (CurrentGame->get_height() - size.height) / 2));
    }
}

void CGlumolSprite::init_hotspot()
{
	//set_alignment(origin_top_left,0,0) ;
	//CL_Sprite::set_rotation_hotspot(origin_top_left, 0,0) ;
}

CGlumolSprite::~CGlumolSprite(void)
{
	parent = WEAK_PTR_FOO(CGlumolSprite);
	int n = children.size() - 1;
	while(n >= 0) {
#ifdef USE_BOOST
		children[n]->parent.reset(); // = NULL; // SHARED_PTR(CGlumolSprite)();
#else
		;
#endif
		n--;
	}
	children.clear();

#ifdef SHOW_DELETIONS
	std::cout << "CGlumolSprite::~CGlumolSprite " << this << std::endl;
	if(currentanim)
        std::cout << "currentanim " << currentanim->name << std::endl;
#endif

	if(drawsprite)
		delete drawsprite;
}

SHARED_PTR(CGlumolRegion) CGlumolSprite::get_region_at(const CL_Pointf &p, bool want_transparent)
{
    for(std::vector<SHARED_PTR(CGlumolRegion) >::iterator j = regions.begin(); j != regions.end(); j++) {
#if DEBUG_FOCUS
        std::cout << "(*j)->is_point_in_region(p.x, p.y) " << (*j)->is_point_in_region(p.x, p.y) << " " << ((*j)->type != CGlumolRegion::transparent) << " " << (*j)->type << std::endl;
#endif
        if(!want_transparent) {
            if( ((*j)->type != CGlumolRegion::transparent) &&
                (*j)->is_point_in_region(p.x, p.y) )
                return (*j);
        }
        else {
            if((*j)->is_point_in_region(p.x, p.y))
                return (*j);
        }
    }
    return SHARED_PTR_FOO(CGlumolRegion);
}

void CGlumolSprite::print_children()
{
    std::cout << "Children of " << this << std::endl;
    for(Childrens::iterator i = children.begin(); i != children.end(); i++) {
#ifdef USE_BOOST
        std::cout << " " << (*i).get() << std::endl;
#else
        std::cout << " " << (*i) << std::endl;
#endif
    }
}

void CGlumolSprite::fit_into(CL_Sizef size)
{
    fit_into(size.width, size.height);
}

void CGlumolSprite::fit_into(float x, float y)
{
    if(size.width && size.height) {
        x = x / size.width;
        y = y / size.height;
        Scale_x = x;
        Scale_y = y;
        update_scale();
    }
}

void CGlumolSprite::set_position(const CL_Pointf& point)
{
    Position = point;
    RotationMatrix rot(CL_Pointf(0, 0), angle);
    ScaleMatrix scale(scale_x, scale_y);
#ifdef USE_BOOST
    CGlumolSprite *Parent = parent.px; // LOCK(parent);
#else
    SHARED_PTR(CGlumolSprite) Parent = LOCK(parent);
#endif
	if(Parent) {
        position = Parent->client_to_screen(point) - apply(rot * scale, Hotspot);
    }
    else
        position = point - apply(rot * scale, Hotspot);
    rotation_hotspot = apply(rot * scale, Rotation_Hotspot) + position;
    scale_hotspot = apply(rot * scale, Scale_Hotspot) + position;

    Childrens childs = children;
    for(Childrens::iterator i = childs.begin(); i != childs.end(); i++) {
        (*i)->set_position((*i)->Position);
    }

	if(get_track_position() && Parent) {
		SHARED_PTR(CGlumolSprite) This;

		CGlumolRegion r(Position.x - Hotspot.x * Scale_x,
                        Position.y,
                        size.width * Scale_x,
                        10000);

		for(Childrens::iterator i = Parent->children.begin(); i != Parent->children.end(); i++) {
#ifdef USE_BOOST
			if((*i).get() == this) {
#else
			if((*i) == this) {
#endif
				This = (*i);
				continue;
			}
			if(!(*i)->currentanim) continue;
			for(std::vector<SHARED_PTR(CGlumolRegion) >::iterator j = (*i)->regions.begin(); j != (*i)->regions.end(); j++) {
				try {
					CGlumolRegion r2 = (r & *(*j));
					if(r2.isEmpty()) {
						z = (*i)->z + 1;
					}
					else {
						z = (*i)->z - 1;
					}
				}
				catch(...) {
					z = (*i)->z + 1;
				}
			}
		}

		Parent->sort_z();

		struct enter_event
		{
			int type;
			SHARED_PTR(CGlumolRegion) region;
		};

		enter_event handlers[32];
		int n = 0;
        
        for(std::vector<SHARED_PTR(CGlumolRegion) >::iterator j = Parent->regions.begin();
			Parent && !Parent->regions.empty() && j != Parent->regions.end(); j++) {
			if((*j)->is_point_in_region(point.x, point.y)) {
				if(std::find(active_regions.begin(),
				   active_regions.end(), *j) != active_regions.end()) {
					if(n < 31) {
						handlers[n].type = 0;
						handlers[n++].region = *j;
					}
				}
				else {
					active_regions.push_back(*j);
					if(n < 31) {
						handlers[n].type = 1;
						handlers[n++].region = *j;
					}
				}
			}
			else {
				std::vector<SHARED_PTR(CGlumolRegion) >::iterator f;
				f = std::find(active_regions.begin(), active_regions.end(), *j);
				if(f != active_regions.end()) {
					if(n < 31) {
						handlers[n].type = 2;
						handlers[n++].region = *j;
					}
					active_regions.erase(f);
				}
			}
		}
		for(int w = 0; w < n; w++) {
			switch(handlers[w].type) {
				case 0:
					handlers[w].region->on_move_in_box(This);
					break;
				case 1:
					handlers[w].region->on_enter_in_box(This);
					break;
                case 2:
					handlers[w].region->on_leave_box(This);
					break;
			}
		}
	}
}

CL_Pointf CGlumolSprite::get_hotspot()
{
	return Hotspot;
}

void CGlumolSprite::set_hotspot(const CL_Pointf& point)
{
	Hotspot = point;
	hotspot = client_to_screen(point);
	set_position(Position);
}

SpriteContainer::SpriteContainer()
{
    flags = 0;
}

void SpriteContainer::set_transparent(bool state)
{
    flags = (flags & ~(1 << TRANSPARENT)) | (state << TRANSPARENT) ;
}

bool SpriteContainer::is_transparent()
{
    return flags & (1 << TRANSPARENT);
}

void SpriteContainer::set_fading(bool state)
{
    flags = (flags & ~(1 << FADING)) | (state << FADING) ;
}

bool SpriteContainer::is_fading()
{
    return flags & (1 << FADING);
}

void SpriteContainer::set_zbuffer(bool state)
{
    flags = (flags & ~(1 << HAS_ZBUFFER)) | (state << HAS_ZBUFFER) ;
}

bool SpriteContainer::is_zbuffer()
{
    return flags & (1 << HAS_ZBUFFER);
}

void SpriteContainer::set_track_position(bool state)
{
     flags = (flags & ~(1 << TRACK_POSITION)) | (state << TRACK_POSITION) ;
}

bool SpriteContainer::get_track_position()
{
    return flags & (1 << TRACK_POSITION);
}

CL_Pointf SpriteContainer::get_rotation_hotspot()
{
	return Rotation_Hotspot;
}

void SpriteContainer::set_rotation_hotspot(const CL_Pointf& point)
{
	Rotation_Hotspot = point;
	rotation_hotspot = client_to_screen(point);
}

CL_Pointf SpriteContainer::get_scale_hotspot()
{
	return Scale_Hotspot;
}

void SpriteContainer::set_scale_hotspot(const CL_Pointf& point)
{
	Scale_Hotspot = point;
	scale_hotspot = client_to_screen(point);
}

CL_Origin SpriteContainer::get_rotation_origin()
{
    return rotation_origin;
}

void SpriteContainer::set_rotation_origin(CL_Origin origin)
{
    rotation_origin = origin;
    CL_Sprite::set_rotation_hotspot(origin,
                                    Rotation_Hotspot.x,
                                    Rotation_Hotspot.y);
}

CL_Origin SpriteContainer::get_scale_origin()
{
    return scale_origin;
}

void SpriteContainer::set_scale_origin(CL_Origin origin)
{
    //CL_Sprite::set_scale_hotspot(
    scale_origin = origin;
}

CL_Pointf Isometrie::apply(CL_Pointf point)
{
	mul(point.x, point.y);
	return point; //CL_Pointf(result.tab[0], result.tab[1]);
}

CL_Pointf SpriteContainer::apply(tmat<double> mat, CL_Pointf point)
{
	mat.mul(point.x, point.y);
	return point;
}

CL_Pointf SpriteContainer::client_to_screen(const CL_Pointf &point)
{
	RotationMatrix rot(CL_Pointf(0, 0), angle);
	ScaleMatrix scale(scale_x, scale_y);
	TranslationMatrix trans(position);
	return apply(trans * scale * rot, point);
}

CL_Pointf SpriteContainer::screen_to_client(const CL_Pointf &point)
{
	RotationMatrix rot(CL_Pointf(0, 0), -angle);
	ScaleMatrix scal(1 / scale_x, 1 / scale_y);
	TranslationMatrix trans(CL_Pointf(-position.x, -position.y));
	CL_Pointf point2 = apply(trans, point);
	point2 = apply(rot, point2);
	return apply(scal, point2);
}

void SpriteContainer::turn(float alpha, CL_Pointf center, bool use_own_center)
{
	if(use_own_center) {
		center = rotation_hotspot;
	}

	RotationMatrix rot(center, alpha);
	scale_hotspot = rot.apply(scale_hotspot);
	hotspot = rot.apply(hotspot);
	position = rot.apply(position);
	angle += alpha;
	CL_Sprite::set_angle(angle);

	if(!use_own_center) {
		rotation_hotspot = rot.apply(rotation_hotspot);
	}
	else {
		Angle += alpha;
	}

	for(Childrens::iterator i = children.begin(); i != children.end(); i++) {
		(*i)->turn(alpha, center, false);
	}
}

void CGlumolSprite::etirer(float factor_x, float factor_y, CL_Pointf center, bool use_own_center)
{
    scale_x *= factor_x;
	scale_y *= factor_y;

	if(currentanim)
        CL_Sprite::set_scale(scale_x, scale_y);
    
    bool track = get_track_position();
    set_track_position(false);
    set_position(Position);
    set_track_position(track);

    Scale_x *= factor_x;
    Scale_y *= factor_y;

	for(Childrens::iterator i = children.begin(); i != children.end(); i++) {
		(*i)->update_scale();
	}
}

float SpriteContainer::get_angle()
{
	return Angle;
}

void SpriteContainer::set_angle(float alpha)
{
	turn(alpha - Angle);
}

float CGlumolSprite::get_scale_x()
{
	return Scale_x;
}

void CGlumolSprite::set_scale_x(float scale)
{
	Scale_x = scale;
	update_scale();
}

float CGlumolSprite::get_scale_y()
{
	return Scale_y;
}

void CGlumolSprite::set_scale_y(float scale)
{
	Scale_y = scale;
	update_scale();
}

void CGlumolSprite::update_scale()
{
    if(currentanim)
	{
#ifdef USE_BOOST
		CGlumolSprite *Parent = parent.px;
#else
		SHARED_PTR(CGlumolSprite) Parent = LOCK(parent);
#endif
		if(Parent)
		{
			float px, py;
            px = Parent->scale_x;
            py = Parent->scale_y;
            scale_x = get_scale_x() * px;
            scale_y = get_scale_y() * py;
			if(currentanim)
                CL_Sprite::set_scale(scale_x, scale_y);
		}
		else
		{
            scale_x = Scale_x;
            scale_y = Scale_y;
            if(currentanim)
                CL_Sprite::set_scale(scale_x, scale_y);
		}
	}
}

void CGlumolSprite::set_size(const CL_Sizef& S)
{
	size = S;
}

void CGlumolSprite::set_z(int Z)
{
	z = Z;
#ifdef USE_BOOST
	CGlumolSprite *Parent = parent.px;
#else
	SHARED_PTR(CGlumolSprite) Parent = LOCK(parent);
#endif
	if (Parent)
		Parent->sort_z();
	else
		the_screen->sort_z();
}

void CGlumolSprite::sort_z()
{
	std::sort(children.begin(), children.end(), Z_compare());
}

void CGlumolSprite::set_currentanim(SHARED_PTR(CGlumolAnimation) bande)
{
    unsigned int frame = current_order;
	currentanim = bande;
	if(currentanim)
	{
        if(frame >= currentanim->orders.size())
            frame = 0;
        set_image_data(*(currentanim->get_sprite()));
        CL_Sprite::set_rotation_hotspot(rotation_origin, 0, 0);
        Hotspot = currentanim->hotspots[0];
	    set_position(Position);
        CL_Size s = get_frame_size(get_current_frame());
		size = CL_Sizef(s.width, s.height);
		alpha_mask = currentanim->alpha_mask;
		red_mask  = currentanim->red_mask;
		green_mask  = currentanim->green_mask;
		blue_mask = currentanim->blue_mask;
    }
	else
		size = CL_Sizef(0, 0);
}

bool operator==(tmat<double> &mat1, tmat<double> &mat2)
{
	if(mat1.ncols() != mat2.ncols() || mat1.nrows() != mat2.nrows())
		return false;
	int n = mat1.ncols() * mat1.nrows();
	for(int i = 0; i < n; i++) {
		if(mat1.tab[i] != mat2.tab[i])
			return false;
	}
	return true;
}

void CGlumolSprite::on_draw()
{
	bool stencil_test = CurrentGame->stencil_test;
	
	on_before_draw();

    if(is_zbuffer()) {
    	CL_GraphicContext *gc = CurrentGame->window.h_window->get_gc();
        if(!CurrentGame->stencil_test)
           gc->clear_stencil();
        CurrentGame->stencil_test = true;
        gc->enable_stencil_write(true, false); 
	    if(currentanim && visible) {
		    draw((int) position.x, (int) position.y);
	    }
        gc->enable_stencil_write(false);
        gc->enable_stencil_test(true);
    }
    else {
	    if(currentanim && visible) {
            draw((int) position.x, (int) position.y);
	    }
    }

	for(Childrens::iterator i = children.begin(); i != children.end(); i++) {
		(*i)->on_draw();
	}

	on_after_draw();

	bool stencil_test2 = CurrentGame->stencil_test;
	if((!stencil_test) && stencil_test2 && !is_zbuffer()) {
		CL_GraphicContext *gc = CurrentGame->window.h_window->get_gc();
		gc->enable_stencil_test(false);
		CurrentGame->stencil_test = false;
	}

}

void CGlumolSprite::kill()
{
	dead = true;
}

int CGlumolSprite::get_frame()
{
    return current_order;
}

void CGlumolSprite::set_current_frame(int frame)
{
    if(currentanim) {
        current_order= frame;

        CL_Pointf p = currentanim->hotspots[current_order];
        if(!(p == Hotspot)) {
            set_hotspot(p);
        }
        CGlumolSprite::set_frame(currentanim->orders[current_order]);
        set_position(Position);
    }
}

void CGlumolSprite::set_frame(unsigned int frame)
{
    if(currentanim) {
        currentanim->pxl_buffer = currentanim->pxl_buffers[frame];

        currentanim->alpha_mask = currentanim->pxl_buffer.get_format().get_alpha_mask();
		currentanim->red_mask = currentanim->pxl_buffer.get_format().get_red_mask();
		currentanim->green_mask = currentanim->pxl_buffer.get_format().get_green_mask();
		currentanim->blue_mask = currentanim->pxl_buffer.get_format().get_blue_mask();

		CL_Sprite::set_frame(frame);
        CL_Pointf p = currentanim->hotspots[current_order];
        if(!(p == Hotspot)) {
            set_hotspot(p);
        }
        on_frame_changed(frame);
    }
}

void CGlumolSprite::fade_out(float delay)
{
	float dec = get_alpha() * 255;
	if(dec == 0.0) return;
	fade_time = 0;
	set_fading(true);
	fade_delay = int(delay / dec * 1000);
	fade_incr = 1 / -dec;
}

void CGlumolSprite::fade_in(float delay)
{
	float dec = (1 - get_alpha()) * 255;
	if(dec == 0.0) return;
	fade_time = 0;
	set_fading(true);
	fade_delay = int(delay / dec * 1000);
	fade_incr = 1 / dec;
	animation_timer->start();
}

void CGlumolSprite::update_anim()
{
	long time_elapsed = animation_timer->get_time_elapsed();

    fade_time += time_elapsed;

	while(is_fading() && (fade_time > fade_delay)) {
		fade_time -= fade_delay;
		CGlumolSprite::set_alpha(get_alpha() + fade_incr);
		float alpha = get_alpha();
		if(alpha < 0) {
			CGlumolSprite::set_alpha(0.0);
			set_fading(false);
		}
		else if(alpha > 1) {
			CGlumolSprite::set_alpha(1.0);
			set_fading(false);
		}
	}

    if(!currentanim) return;

	int total_frames = currentanim->times.size();

	if(total_frames < 2 || !playing) {
		animation_timer->start();
                return;
	}

	DWORD framedelay = (DWORD) currentanim->times[current_order];	//DWORD framedelay = (DWORD) get_frame_delay(get_current_frame());

    update_time += time_elapsed ;

	if(!framedelay) {
		framedelay = 10;
	}

	while(update_time > framedelay)
	{
		update_time -= framedelay;
		current_order = current_order + delta;

		// Beginning or end of loop ?
		if(current_order >= total_frames || current_order < 0)
		{
			if(!is_play_loop())
			{
				int delta_frame = is_play_backward() ? -1 : 1;
				if(delta_frame != delta || !is_play_pingpong())
				{
					on_animation_finished();
					return;
				}
			}

			if(is_play_pingpong())
			{
				delta = -delta;	// Change direction
				if(delta > 0) {
					current_order = 1;
				}
				else
					current_order = total_frames - 2;
			}
			else { // Restart
				if(is_play_backward()) {
					current_order = total_frames - 1;
				}
				else {
					current_order = 0;
				}
			}
		}
		else {
			CL_Pointf p = currentanim->move_offsets[current_order];
			if(p.x || p.y) {
				p.x *= scale_x;
				p.y *= scale_y;
				set_position(Position + p);
			}
		}

		CGlumolSprite::set_frame(currentanim->orders[current_order]);

		CL_Pointf p = currentanim->hotspots[current_order];
		if(!(p == Hotspot)) {
			set_hotspot(p);
		}
	}

	animation_timer->start();
}

void CGlumolSprite::update_alpha()
{
#ifdef USE_BOOST
	CGlumolSprite *Parent = parent.px; // CGlumolSprite *Parent = parent;//.get();
#else
	SHARED_PTR(CGlumolSprite) Parent = LOCK(parent);
#endif
    if(Parent) {
        alpha = Alpha * Parent->get_alpha();
    	if(currentanim)
            CL_Sprite::set_alpha(alpha);
    }
    else {
   	    alpha = Alpha;
	    if(currentanim)
            CL_Sprite::set_alpha(Alpha);
    }
	for(Childrens::iterator i = children.begin(); i != children.end(); i++) {
		(*i)->update_alpha();
	}
}

void CGlumolSprite::set_alpha(float a)
{
	Alpha = a;
	update_alpha();
}

float CGlumolSprite::get_alpha() {
	return alpha;
}

void CGlumolSprite::update()
{
	update_anim();
	update_scale();

    for(std::vector<SHARED_PTR(CGlumolSprite) >::iterator i = children.begin(); i != children.end(); i++) {
        (*i)->update();
    }
}

void CGlumolSprite::on_left_button_down()
{
#ifdef USE_BOOST
	CGlumolSprite *Parent = parent.px;
#else
	SHARED_PTR(CGlumolSprite) Parent = LOCK(parent);
#endif
	if(Parent) {
        Parent->on_left_button_down();
    }
}

void CGlumolSprite::rotate(DWORD delay, DWORD speed, rotation_dir dir)
{

}

void CGlumolSprite::rotate_to(DWORD angle, DWORD speed, rotation_dir dir)
{
}

void CGlumolSprite::move(DWORD direction, DWORD delay, DWORD speed )
{
}

void CGlumolSprite::move_to(CL_Pointf pt, DWORD speed)
{
}

void CGlumolSprite::zoom(DWORD speed, DWORD delay)
{
}

void CGlumolSprite::zoom_on(CL_Pointf pt, DWORD speed)
{
}

// Draw on surface

CDrawSprite *CGlumolSprite::start_drawings()
{
	return new CDrawSprite(this);
}

// SCREEN

CGlumolScreen::CGlumolScreen()
{
	background_color = CL_Color(0,0,0);
}

CGlumolScreen::~CGlumolScreen()
{
	int n = children.size() - 1;
	while(n >= 0) {
		children[n] = SHARED_PTR_FOO(CGlumolSprite);
		n--;
	}
	children.clear();
}

SHARED_PTR(CGlumolSprite) CGlumolSprite::get_focused_object(int x, int y)
{
	for(std::vector<SHARED_PTR(CGlumolSprite) >::reverse_iterator i = children.rbegin(); i != children.rend(); i++){
		SHARED_PTR(CGlumolSprite) obj;
        	if(CurrentGame->mouse_sprite == (*i))
        		continue;
		if(!(*i)->visible || !(*i)->alpha || (*i)->is_transparent())
			continue;
		if((obj = (*i)->get_focused_object(x,y))) {
			return obj;
		}
		if(!obj && (*i)->is_point_in_surface(CL_Pointf(x, y)))
			return (*i);
	}
	return SHARED_PTR_FOO(CGlumolSprite);
}

void CGlumolScreen::update()
{
	for(std::vector<SHARED_PTR(CGlumolSprite) >::iterator i = children.begin(); i != children.end(); i++) {
		if( (*i)->is_dead() )
			children.erase(i);
	}

	draw();
}

SHARED_PTR(CGlumolSprite) CGlumolScreen::get_focused_object(int x, int y )
{
    SHARED_PTR(CGlumolSprite) res;
    for(std::vector<SHARED_PTR(CGlumolSprite) >::reverse_iterator i = children.rbegin(); i != children.rend(); i++) {
        if(CurrentGame->mouse_sprite == (*i))
            continue;
        if(!(*i)->visible || !(*i)->alpha || (*i)->is_transparent())
            continue;
        if(res = (*i)->get_focused_object(x, y))
            return res;
        if(!res && (*i)->is_point_in_surface(CL_Pointf(x, y)))
        	return (*i);
    }
    return SHARED_PTR_FOO(CGlumolSprite);
}

void CGlumolScreen::sort_z()
{
	std::sort(children.begin(), children.end(), Z_compare());

}
void CGlumolScreen::draw()
{
	CL_Display::clear(background_color);

	for(std::vector<SHARED_PTR(CGlumolSprite) >::iterator i = children.begin(); i != children.end(); i++) {
		if((*i)->is_visible()) {
			(*i)->update();
			(*i)->on_draw();
		}
	}

	if(CurrentGame->mouse_sprite) {
		CurrentGame->mouse_sprite->update();
		CurrentGame->mouse_sprite->on_draw();
    }
}

void CGlumolScreen::draw_line(const CL_Pointf& begin, const CL_Pointf& end, const CL_Color& color)
{
	CL_Display::draw_line((int) begin.x,(int) begin.y,(int) end.x, (int)end.y, color);

}

void CGlumolScreen::draw_pixel( const CL_Pointf& pos, const CL_Color& color)
{
	CL_Display::draw_pixel((int)pos.x,(int) pos.y, color);
}

void CGlumolScreen::draw_rect(const CL_Rectf& rect, const CL_Color& color)
{
	CL_Display::draw_rect(To_CL_Rect(rect), color);
}

void CGlumolScreen::fill_rect(const CL_Rectf& rect, const CL_Color& color)
{
	CL_Display::fill_rect(To_CL_Rect(rect), color);
}

void CGlumolScreen::draw_text(const char *text, int x, int y, const CL_Color& color, CL_Sizef max_size)
{
	if(!font) {
		std::cout << "You must set 'game.screen.current_font' before using 'draw_text'" << std::endl;
		return;
	}
	font->set_color(color);
	font->draw(text, x, y, max_size);
}

void CGlumolScreen::set_background_color(const CL_Color& color)
{
	background_color = color;
}

CL_Color CGlumolScreen::get_background_color()
{
	return background_color;
}

void CGlumolScreen::set_current_font(SHARED_PTR(CGlumolFont) Font)
{
	font = Font;
}

SHARED_PTR(CGlumolFont) CGlumolScreen::get_current_font()
{
	return font;
}

bool CGlumolSprite::is_point_in_surface(const CL_Pointf& point)
{
    CL_Pointf pt = screen_to_client(point);

    if(get_region_at(pt))
        return true;

    float sx, sy;
    
    if(!currentanim) {
        sx = 1.0;
        sy = 1.0;
    }
    else get_scale(sx, sy);
    
    if(pt.x < 0 || pt.y < 0 || pt.x > size.width * sx || pt.y > size.height * sy)
		return false;
    
	if(!pixel_test || !currentanim) return true;
	
    pt.x += get_current_frame() * size.width;
	CL_Color pixel;
	char *ptr = ((char*) currentanim->pxl_buffer.get_data()) +
		int((int(pt.y) * currentanim->pxl_buffer.get_width() + int(pt.x)) *
			(currentanim->pxl_buffer.get_format().get_depth() >> 3));
        memcpy(&pixel.color, ptr, (currentanim->pxl_buffer.get_format().get_depth() >> 3));

    // red = green
	// green = blue
	// blue = alpha
	// alpha = red

	int color = pixel.color;

    if(color & alpha_mask) return 1;

    if(!alpha_mask)
        return color != 0;
        
	return ((color & alpha_mask) != 0) &&
		((color & (red_mask | green_mask | blue_mask)));
}

void CDrawSprite::DrawLine(const CL_Pointf& begin, const CL_Pointf& end, const CL_Color& color)
{
	gc->draw_line((int) begin.x, (int) begin.y, (int) end.x, (int) end.y, color);
}

void CDrawSprite::DrawPixel( const CL_Pointf& pos, const CL_Color& color)
{
	gc->draw_pixel((int) pos.x, (int) pos.y, color);
}

void CDrawSprite::DrawRect(const CL_Rectf& rect, const CL_Color& color)
{
	gc->draw_rect(CL_Rect((int) rect.left, (int) rect.top, (int) rect.right, (int) rect.bottom), color);
}

void CDrawSprite::FillRect(const CL_Rectf& rect, const CL_Color& color)
{
	gc->fill_rect(CL_Rect((int) rect.left, (int) rect.top, (int) rect.right, (int) rect.bottom), color);
}

void CDrawSprite::DrawText(const char *text, int x, int y, const CL_Color& color, CL_Sizef max_size)
{
	if(!CurrentGame->screen->font) {
		std::cout << "You must set 'game.screen.current_font' before using 'draw_text'" << std::endl;
		return;
	}
	CurrentGame->screen->font->set_color(color);
	CurrentGame->screen->font->draw_to_gc(text, x, y, gc, max_size);
}

 CDrawSprite::CDrawSprite(CGlumolSprite *s) :
	sprite(s),
	gc(NULL)
{
	canvas = CL_Canvas(sprite->get_frame_surface(0));
	gc = canvas.get_gc();
}

CDrawSprite::~CDrawSprite()
{
	canvas.sync_surface();
}
