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


#include "misc_classes.h"
#include <vector>
#include <list>
#include "glumolregion.h"

class SPolygon;
typedef struct Ppoly_t* tpoly;

struct CPathfinder
{
	std::list<CL_Point> waypoints;
	std::vector<CGlumolRegion*> regions;

	CPathfinder();
	~CPathfinder();
	void add_region(CGlumolRegion *region);
	int	search_path(tpoly *array, int nbpolys, CL_Point start, CL_Point goal);
	int find_path(CL_Point start, CL_Point goal);
	tpoly *build_poly_array(CGlumolRegion *region);
	std::list<CL_Point> get_waypoints();
#ifdef _DEBUG
	void test();
#endif
};
