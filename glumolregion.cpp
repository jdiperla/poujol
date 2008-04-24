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
#include "glumolregion.h"
#include "PolyUtil.h"

// using namespace boost::python;

CGlumolRegion::CGlumolRegion(void)
{
    type = walkable;
}

CGlumolRegion::~CGlumolRegion(void)
{
}

void CGlumolRegion::update_region()
{
}

void CGlumolRegion::add_point(CL_Point point)
{
}

CL_Point CGlumolRegion::get_center()
{
	double x, y;
	getCenter(&x, &y);
	return CL_Point((int) x, (int) y);
}

bool CGlumolRegion::is_point_in_region(double x, double y)
{
	return poly_p_point_inside(getGpcSPolygon(), x, y);
}

bool CGlumolRegion::is_point_in_region(CL_Point point)
{
	return poly_p_point_inside(getGpcSPolygon(), point.x, point.y);
}

CL_Rectf CGlumolRegion::get_bouncing_box()
{
	double x0, x1, y0, y1;
	poly_p_boundingbox(getGpcSPolygon(), &x0, &x1, &y0, &y1);
	return CL_Rectf((float) x0, (float) y0, (float) x1, (float) y1);
}

void CGlumolRegion::shift(CL_Point point)
{
	poly_p_shift(getGpcSPolygon(), point.x, point.y);
}

void CGlumolRegion::shift(double x, double y)
{
	poly_p_shift(getGpcSPolygon(), x, y);
}

void CGlumolRegion::scale(double ratio_x, double ratio_y, CL_Point center)
{
	poly_p_scale(getGpcSPolygon(), ratio_x, ratio_y, center.x, center.y);
}

void CGlumolRegion::scale(double ratio_x, double ratio_y, double center_x, double center_y)
{
	poly_p_scale(getGpcSPolygon(), ratio_x, ratio_y, center_x, center_y);
}

void CGlumolRegion::scale(double ratio, CL_Point center)
{
	poly_p_scale(getGpcSPolygon(), ratio, ratio, center.x, center.y);
}

void CGlumolRegion::rotate(double angle, CL_Point center)
{
	poly_p_rotate(getGpcSPolygon(), angle, center.x, center.y);
}

void CGlumolRegion::rotate(double angle, double center_x, double center_y)
{
	poly_p_rotate(getGpcSPolygon(), angle, center_x, center_y);
}

void CGlumolRegion::warp_to_box(CL_Rect rect)
{
	poly_p_warpToBox(getGpcSPolygon(), rect.left, rect.right, rect.top, rect.bottom, NULL);
}

void CGlumolRegion::warp_to_box(double x0, double y0, double x1, double y1)
{
	poly_p_warpToBox(getGpcSPolygon(), x0, x1, y0, y1, NULL);
}

void CGlumolRegion::flip(double x)
{
	poly_p_flip(getGpcSPolygon(), x);
}

void CGlumolRegion::flop(double y)
{
	poly_p_flop(getGpcSPolygon(), y);
}

bool CGlumolRegion::intersects(const CGlumolRegion &s)
{
	CGlumolRegion reg = s;
	SPolygon *p = Intersection(&reg);
	reg = *p;
	delete p;
	return ! reg.isEmpty();
}

CGlumolRegion CGlumolRegion::operator&(const CGlumolRegion &s)
{
    CGlumolRegion reg = s;
    SPolygon *p = Intersection(&reg);
    reg = *p;
    delete p;
    return reg;
}

gpc_contours &CGlumolRegion::get_contours()
{
	return getGpcSPolygon()->contour;
}

void CGlumolRegion::print()
{
	std::cout << poly->contour.size() << " contours " << this << std::endl;
	for(unsigned int i = 0; i < poly->contour.size(); i++) {
		std::cout << "Contour " << poly->contour[i].size() << " vertices " << std::endl;
		if(poly->contour[i].hole)
			std::cout << "C'est un trou" << std::endl;
		else
			std::cout << "Pas un trou" << std::endl;
		for(unsigned int j = 0; j < poly->contour[i].size(); j++) {
			std::cout << "Vertex " << poly->contour[i][j].x << " " << poly->contour[i][j].y << std::endl;
		}
	}
}

void CGlumolRegion::on_enter_in_box(SHARED_PTR(CGlumolSprite) sprite)
{
}

void CGlumolRegion::on_leave_box(SHARED_PTR(CGlumolSprite) sprite)
{
}

void CGlumolRegion::on_move_in_box(SHARED_PTR(CGlumolSprite) sprite)
{
}
