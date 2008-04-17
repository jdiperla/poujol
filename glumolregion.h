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

#ifndef _GLUMOLREGION_H_
#define _GLUMOLREGION_H_

#include "poujol.h"
#include "misc_classes.h"
#include "polygon.h"
#include <vector>
#ifdef USE_BOOST
#include <boost/shared_ptr.hpp>
#endif

class CGlumolSprite;

class CGlumolRegion : public SPolygon
{
public:
	enum region_type { transparent, walkable, non_walkable };

	CGlumolRegion(void);
	~CGlumolRegion(void);

	void add_point(CL_Point point);
	void update_region();

	CGlumolRegion(double d1, double d2, double d3, double d4) :
		SPolygon(d1, d2, d3, d4) { type = walkable; }

	CGlumolRegion(double d1, double d2, double d3, double d4, double d5, int d6) :
		SPolygon(d1, d2, d3, d4, d5, d6) { type = walkable; }

	CGlumolRegion(const CGlumolRegion &rg)
	{
		for(unsigned int i = 0; i < rg.poly->contour.size(); i++) {
			gpc_vertex_list l = rg.poly->contour[i];
			poly->contour.push_back(l);
		}
		type = rg.type;
	}

	SPolygon *Union(SPolygon *p)
	{
		return SPolygon::Union(p);
	}

	SPolygon *Intersection(SPolygon *p)
	{
		return SPolygon::Intersection(p);
	}

	SPolygon *Xor(SPolygon *p)
	{
		return SPolygon::Xor(p);
	}

	const CGlumolRegion operator+(const CGlumolRegion &s)
	{
		CGlumolRegion reg = s;
		SPolygon *p = Union(&reg);
		reg = *p;
		delete p;
		return reg;
	}

	const CGlumolRegion operator-(const CGlumolRegion &s)
	{
		CGlumolRegion reg = s;
		SPolygon *p = Minus(&reg);
		reg = *p;
		delete p;
		return reg;
	}

	CGlumolRegion & operator=(const CGlumolRegion & P)
    {
		copySPolygon(P);
		return (*this);
	}

	CGlumolRegion & operator=(const SPolygon& P)
    {
		copySPolygon(P);
		return (*this);
	}

	CGlumolRegion &operator+=(const CGlumolRegion &s)
	{
		CGlumolRegion reg = s;
		*this = *Union(&reg);
		return *this;
	}

	CGlumolRegion &operator-=(const CGlumolRegion &s)
	{
		CGlumolRegion reg = s;
		*this = *Minus(&reg);
		return *this;
	}

	CGlumolRegion operator^(const CGlumolRegion &s)
	{
		CGlumolRegion reg = s;
		SPolygon *p = Xor(&reg);
		reg = *p;
		delete p;
		return reg;
	}

	CGlumolRegion &operator^=(const CGlumolRegion &s)
	{
		CGlumolRegion reg = s;
		*this = *Xor(&reg);
		return *this;
	}

	CGlumolRegion operator&(const CGlumolRegion &s);

	CGlumolRegion &operator&=(const CGlumolRegion &s)
	{
		CGlumolRegion reg = s;
		*this = *Intersection(&reg);
		return *this;
	}

	CGlumolRegion operator|(const CGlumolRegion &s)
	{
		CGlumolRegion reg = s;
		SPolygon *p = Union(&reg);
		reg = *p;
		delete p;
		return reg;
	}

	CGlumolRegion &operator|=(const CGlumolRegion &s)
	{
		CGlumolRegion reg = s;
		*this = *Union(&reg);
		return *this;
	}

	bool operator==(const CGlumolRegion &s)
	{
		CGlumolRegion reg = s;
		SPolygon *p = Minus(&reg);
		bool sarkozy;
		if(p->isEmpty())
			sarkozy = true;
		else
			sarkozy = false;
		delete p;
		return sarkozy;
	}

	bool operator!=(const CGlumolRegion &s)
	{
		return !(*this == s);
	}

	CL_Point get_center();
	bool is_point_in_region(double x, double y);
	bool is_point_in_region(CL_Point point);
	CL_Rectf get_bouncing_box();
	void shift(CL_Point point);
	void shift(double x, double y);
	void scale(double ratio_x, double ratio_y, CL_Point center = CL_Point(0, 0));
	void scale(double ratio_x, double ratio_y, double center_x, double center_y);
	void scale(double ratio, CL_Point center = CL_Point(0, 0));
	void rotate(double angle, CL_Point center = CL_Point(0, 0));
	void rotate(double angle, double center_x, double center_y);
	void warp_to_box(CL_Rect rect);
	void warp_to_box(double x0, double y0, double x1, double y1);
	void flip(double x);
	void flop(double y);
	bool intersects(const CGlumolRegion &s);
	gpc_contours &get_contours();
	void print();

	virtual void on_enter_in_box(SHARED_PTR(CGlumolSprite) sprite);
	virtual void on_move_in_box(SHARED_PTR(CGlumolSprite) sprite);
	virtual void on_leave_box(SHARED_PTR(CGlumolSprite) sprite);

	region_type type;
};

#endif
