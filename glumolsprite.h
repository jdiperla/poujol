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

#ifndef __GLUMOLSPRITE_H__
#define __GLUMOLSPRITE_H__

#include "poujol.h"
#include <ClanLib/display.h>
#include <vector>
#include <string>
#ifdef USE_BOOST
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#endif
#include "glumolregion.h"
#include "glumolanimation.h"
#include "misc_classes.h"
#include "tmat.h"

#ifdef TRANSPARENT
#undef TRANSPARENT
#endif

#define TRANSPARENT    0
#define FADING         1
#define TRACK_POSITION 2
#define HAS_ZBUFFER    3

class CGlumolFont;

class glumol_timer
{

public:
	DWORD Id;
	DWORD Milliseconds;
	DWORD Start;

	glumol_timer(DWORD, DWORD);
	void start();
	bool Update();
};

class timer
{
private :
	long begin;
public:

	timer();
	void start();
	long get_time_elapsed();
	void set_time_elapsed(DWORD t);

};

class Isometrie : public tmat<double>
{
public:
	Isometrie() :
		tmat<double>(3, 3) {
		(*this).tab[6] = 0;
		(*this).tab[7] = 0;
		(*this).tab[8] = 1;
	}
	CL_Pointf apply(CL_Pointf point);
};

class RotationMatrix : public Isometrie
{
public:
	RotationMatrix(CL_Pointf centre, double angle) {
		double cosa = cos(angle * PI / 180);
		double sina = sin(angle * PI / 180);

		(*this).tab[0] = cosa;
		(*this).tab[1] = -sina;
		(*this).tab[2] = centre.x * (1 - cosa) + centre.y * sina;
		(*this).tab[3] = sina;
		(*this).tab[4] = cosa;
		(*this).tab[5] = centre.y * (1 - cosa) - centre.x * sina;
	}
};

class TranslationMatrix : public Isometrie
{
public:
	TranslationMatrix(CL_Pointf offset) {
		(*this).tab[0] = 1;
		(*this).tab[1] = 0;
		(*this).tab[2] = offset.x;
		(*this).tab[3] = 0;
		(*this).tab[4] = 1;
		(*this).tab[5] = offset.y;
	}
};

class IdentityMatrix : public Isometrie
{
public:
	IdentityMatrix() {
		(*this).tab[0] = 1;
		(*this).tab[1] = 0;
		(*this).tab[2] = 0;
		(*this).tab[3] = 0;
		(*this).tab[4] = 1;
		(*this).tab[5] = 0;
	}
};

class ScaleMatrix : public Isometrie
{
public:
	ScaleMatrix(double rapport_x, double rapport_y) {
		(*this).tab[0] = rapport_x;
		(*this).tab[1] = 0;
		(*this).tab[2] = 0;
		(*this).tab[3] = 0;
		(*this).tab[4] = rapport_y;
		(*this).tab[5] = 0;
	}
};

class NullMatrix: public Isometrie
{
public:
	NullMatrix() {
		(*this).tab[0] = 0;
		(*this).tab[1] = 0;
		(*this).tab[2] = 0;
		(*this).tab[3] = 0;
		(*this).tab[4] = 0;
		(*this).tab[5] = 0;
		(*this).tab[6] = 0;
		(*this).tab[7] = 0;
		(*this).tab[8] = 0;
	}
};

struct CGlumolScreen;
class CGlumolSprite;
class CDrawSprite;

class Childrens : public std::vector<SHARED_PTR(CGlumolSprite) >
{
};

class SpriteContainer : public CL_Sprite
{
public:
    CL_Pointf Rotation_Hotspot, rotation_hotspot;
    CL_Pointf Scale_Hotspot, scale_hotspot;
    CL_Pointf Hotspot, hotspot;
    CL_Pointf Position, position;

    float Angle, angle;
    float Scale_x, Scale_y;
    float scale_x, scale_y;
    float Alpha, alpha;
    int alpha_mask, red_mask, green_mask, blue_mask;

    int fade_delay;
    float fade_incr;
    int fade_time;
    int flags;
    CL_Origin rotation_origin;
    CL_Origin scale_origin;

    Childrens children;
	Regions regions;

    SpriteContainer();
    
    Childrens &get_children() { return children; }

    CL_Pointf client_to_screen(const CL_Pointf &pt);
    CL_Pointf screen_to_client(const CL_Pointf &pt);

#ifdef USE_BOOST
	WEAK_PTR(CGlumolSprite) parent;
    SHARED_PTR(CGlumolSprite) get_parent() { return parent.lock(); }
    void set_parent(SHARED_PTR(CGlumolSprite) &sprite) { parent = sprite; }
#else
	CGlumolSprite *parent;
    CGlumolSprite *get_parent() { return parent; }
    void set_parent(CGlumolSprite *sprite) { parent = sprite; }
#endif

    void set_transparent(bool state);
    bool is_transparent();

    void set_track_position(bool state);
    bool get_track_position();

    void set_zbuffer(bool state);
    bool is_zbuffer();

    void set_fading(bool state);
    bool is_fading();

	void set_rotation_hotspot(const CL_Pointf &pt);
	CL_Pointf get_rotation_hotspot(void);

	void set_scale_hotspot(const CL_Pointf &pt);
	CL_Pointf get_scale_hotspot(void);

    void set_rotation_origin(CL_Origin origin);
    CL_Origin get_rotation_origin();

    void set_scale_origin(CL_Origin origin);
    CL_Origin get_scale_origin();

    CL_Pointf apply(tmat<double> mat, CL_Pointf point);
	void turn(float alpha, CL_Pointf center = CL_Pointf(0, 0), bool use_own_center = true);

	float get_angle();
	void set_angle(float angle);
};

class CGlumolSprite : public SpriteContainer
{
public:
	bool dead;
	bool visible;
	bool playing;
	bool check_collision;
	bool pixel_test;
	int z;
	int current_order;
	int darking_factor;
	int delta;
    CDrawSprite *drawsprite;
	CL_PixelBuffer buffer;
	CL_Sizef size;
	std::string filename;
	std::list<glumol_timer*> timers;
    std::vector<SHARED_PTR(CGlumolRegion) > active_regions;
	SHARED_PTR(CGlumolAnimation) currentanim;
    timer *animation_timer;
	unsigned int update_time;
	CL_BlendFunc src_blend_mode;
	CL_BlendFunc dest_blend_mode;

	CGlumolSprite(CGlumolSprite *parent);
	CGlumolSprite(CGlumolScreen *parent);
	//CGlumolSprite(CGlumolSprite *parent, int width, int height, CL_Color color);
	//CGlumolSprite(SHARED_PTR(CGlumolScreen) &parent, int width, int height, CL_Color color);

	virtual ~CGlumolSprite(void);

	void init(CGlumolSprite *parent);
	void build(int cx, int cy, CL_Color color);

	SHARED_PTR(CGlumolRegion) get_region_at(const CL_Pointf &pt, bool w = false);

	void fade_in(float delay);
	void fade_out(float delay);

	void sort_z();

	Childrens &get_children() { return children; }

        /* Position */

	void set_position(const CL_Pointf &pt);
	CL_Pointf const& get_position(void) const {
        return Position;
    }

	void set_hotspot(const CL_Pointf &pt);
	CL_Pointf get_hotspot(void);

	float get_x(void) { return position.x; }
	void set_x(float x) { set_position(CL_Pointf(x, position.y)); }

	float get_y(void){ return position.y; }
	void set_y(float y){ set_position(CL_Pointf(position.x, y)); }

	int get_z(void) { return z; }
	void set_z(int Z) ;

	float get_scale_x();
	void set_scale_x(float);

	float get_scale_y();
	void set_scale_y(float);

	void fit_into(CL_Sizef size);
	void fit_into(float w, float h);

	void update_scale();

	CL_Sizef get_size(void) { return size; }
	void set_size(const CL_Sizef& S);

	bool is_playing() { return playing; }
	void set_playing(bool state) { playing = state; }

	bool is_visible() { return visible; }
	void set_visible(bool state) { visible = state; }

//	bool get_check_collision() { return check_collision; }
//	void set_check_collision(bool c) { check_collision = c; }

	void set_darking_factor(int s) {
		darking_factor = s;
	}

	int get_darking_factor() {
		return darking_factor;
	}

	void etirer(float factor_x, float factor_y, CL_Pointf center = CL_Pointf(0, 0), bool use_own_center = true);

    void set_alpha(float a);
	float get_alpha();
	void update_alpha();

	void set_blend_func_src(CL_BlendFunc func){src_blend_mode = func;}
	CL_BlendFunc get_blend_func_src(){return src_blend_mode;}

	void set_blend_func_dest(CL_BlendFunc func){ dest_blend_mode = func; }
	CL_BlendFunc get_blend_func_dest(){ return dest_blend_mode; }

	void set_frame(unsigned int frame);
    int get_frame();
    void set_current_frame(int order);

	void set_currentanim(SHARED_PTR(CGlumolAnimation) bande);
	SHARED_PTR(CGlumolAnimation) get_currentanim() { return currentanim; }

	bool is_dead() {
		return dead;
	}

    void print_children();
    CGlumolSprite* get_first_animed_parent();

	void init_hotspot();
	bool is_point_in_surface(const CL_Pointf& point);
    void center();

	void update();
	void update_anim();
	void kill();

	SHARED_PTR(CGlumolSprite) get_focused_object(int x, int y);
	CDrawSprite *start_drawings();

	void rotate(DWORD delay, DWORD speed, rotation_dir dir);
	void rotate_to(DWORD angle, DWORD speed, rotation_dir dir);
	void move(DWORD direction, DWORD delay, DWORD speed );
	void move_to(CL_Pointf pt, DWORD speed);
	void zoom(DWORD speed, DWORD delay);
	void zoom_on(CL_Pointf pt, DWORD speed);

	/* Events */

	virtual void on_key_down(int key) { }
	virtual void on_key_up(int key) { }
	virtual void on_key_down_repeat(int key, unsigned int miliseconds) { }

	virtual void on_mouse_button_down(unsigned int button) { }
	virtual void on_mouse_button_up(unsigned int button) { }

	virtual void on_left_button_down();
	virtual void on_right_button_down() { }
	virtual void on_left_button_up() { }
	virtual void on_right_button_up() { }

	virtual void on_middle_button_down() { }
	virtual void on_middle_button_up() { }

	virtual void on_mouse_wheel_up() { }
	virtual void on_mouse_wheel_down() { }

	virtual void on_mouse_move(CL_Pointf, CL_Pointf, float axis) { }

	virtual void on_left_button_down_repeat(unsigned int milliseconds) { }
	virtual void on_right_button_down_repeat(unsigned int milliseconds) { }
	virtual void on_middle_button_down_repeat(unsigned int milliseconds) { }

	virtual void on_before_draw() { }
	virtual void on_after_draw() { }
	virtual void on_draw();

	virtual void on_focus(SHARED_PTR(CGlumolSprite) newfocus) { }
	virtual void on_lose_focus(SHARED_PTR(CGlumolSprite) newfocus) { }
	virtual void on_timer(DWORD IDTimer) { }

	virtual void on_frame_changed(int new_frame) { };
	virtual void on_animation_finished(){};
};

struct CGlumolScreen : public SpriteContainer
{
	CL_Color background_color;
	SHARED_PTR(CGlumolFont) font;

	CGlumolScreen();
	virtual ~CGlumolScreen();

	void draw();
	void update();
	SHARED_PTR(CGlumolSprite) get_focused_object(int x, int y);
	void sort_z();

	// Draw basics

	void draw_line(const CL_Pointf& begin, const CL_Pointf& end, const CL_Color& color);
	void draw_pixel( const CL_Pointf& pos, const CL_Color& color);
	void draw_rect(const CL_Rectf& rect, const CL_Color& color);
	void fill_rect(const CL_Rectf&  rect, const CL_Color& color);
	void draw_text(const char *text, int x, int y, const CL_Color& color, CL_Sizef max_size = CL_Sizef(0, 0));

	//Properties

	void set_background_color(const CL_Color& color);
	CL_Color get_background_color();

	void set_current_font(SHARED_PTR(CGlumolFont));
	SHARED_PTR(CGlumolFont) get_current_font();
};

extern CGlumolScreen* the_screen;

class CDrawSprite
{
public:
	CGlumolSprite *sprite;
	CL_GraphicContext *gc;
	CL_PixelBuffer px;
	CL_Canvas canvas;

	CDrawSprite(CGlumolSprite *s);

	CDrawSprite()
	{
		throw CL_Error("This function should never be executed");
	}

	~CDrawSprite();

	void DrawLine(const CL_Pointf& begin, const CL_Pointf& end, const CL_Color& color);
	void DrawPixel(const CL_Pointf& pos, const CL_Color& color);
	void DrawRect(const CL_Rectf& rect, const CL_Color& color);
	void FillRect(const CL_Rectf&  rect, const CL_Color& color);
	void DrawText(const char *text, int x, int y, const CL_Color& color, CL_Sizef max_size = CL_Sizef(0, 0));
};

#endif
