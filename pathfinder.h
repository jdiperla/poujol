
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
