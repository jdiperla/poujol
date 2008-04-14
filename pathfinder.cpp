
#define _WINCON_
#include "stdafx.h"

#include <iostream>
#include <time.h>
#include <fstream>
#include "pathfinder.h"
#include "vis.h"
#include "polygon.h"
#include "PolyUtil.h"

#include <boost/config.hpp>
#include <iostream>                      // for std::cout
#include <utility>                       // for std::pair
#include <algorithm>                     // for std::for_each
#include <boost/utility.hpp>             // for boost::tie
#include <boost/graph/graph_traits.hpp>  // for boost::graph_traits
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;

#define _DEBUG

#ifdef DEBUG_PATHFINDER
void printVconfig(vconfig_t *cp);
#endif

typedef std::pair<gpc_vertex, gpc_vertex> pair_vertex;
typedef adjacency_list<vecS, vecS, undirectedS,
no_property, property<edge_weight_t, double> > Graph;

CPathfinder::CPathfinder()
{
}

CPathfinder::~CPathfinder()
{
}

void CPathfinder::add_region(CGlumolRegion *region)
{
    regions.push_back(region);
}

std::list<CL_Point> CPathfinder::get_waypoints()
{
    return waypoints;
}

double area2(gpc_vertex a, gpc_vertex b, gpc_vertex c)
{
    return ((a.y - b.y)*(c.x - b.x) - (c.y - b.y)*(a.x - b.x));
}

void
make_CW2 (gpc_vertex *coords, int size)
{
    int			i, j, n;
    gpc_vertex  *P;
    gpc_vertex	tP;
    double	area=0.0;
    
    P = coords;
    n = size;
    /* points or lines don't have a rotation */
    if (n > 2) {
        /* check CW or CCW by computing (twice the) area of poly */
        for (i=1; i < n-1; i++) {
            area += area2(P[0], P[i+1], P[i]);
        }
        /* if the area is -ve then the rotation needs to be reversed */
        /* the starting point is left unchanged */
        if (area < 0.0) {
            for (i = 1, j = n-1; i < 1+n/2; i++, j--) {
                tP = P[i];
                P[i] = P[j];
                P[j] = tP;
            }
        }
    }
}

#ifdef DEBUG_PATHFINDER
void printVconfig(vconfig_t *cp)
{
    int i, j;
    int *next, *prev;
    Ppoint_t *pts;
    array2 arr;

    next = cp->next;
    prev = cp->prev;
    pts = cp->P;
    arr = cp->vis;

    printf ("this next prev point\n");
    for (i = 0; i < cp->N; i++)
        printf ("%3d  %3d  %3d    (%3g,%3g)\n", i, next[i],prev[i],
                pts[i].x, pts[i].y);

    printf ("\n\n");

    printf ("%3d ", 0);
    for (j = 0; j < cp->N; j++)
        printf ("%3d ", j);
    printf ("\n");
    for (i = 0; i < cp->N; i++) {
        printf ("%3d ", i);
        for (j = 0; j < cp->N; j++)
            printf ("%3d ", int(arr[i][j]));
        printf ("\n");
    }
}

void gasp_print_point(Ppoint_t p)
{
    printf("%3g %3g\n", p.x, p.y);
}

void gasp_print_obstacles(vconfig_t * conf)
{
    int i, j;
    Ppoly_t poly;
    printf("Obstacles : \n%d\n", conf->Npoly);
    for (i = 0; i < conf->Npoly; i++) {
        poly.ps = &(conf->P[conf->start[i]]);
        poly.pn = conf->start[i + 1] - conf->start[i];
        printf("%d\n", poly.pn);
        for (j = 0; j < poly.pn; j++)
            gasp_print_point(poly.ps[j]);
    }
}
#endif

bool intersects(std::vector<pair_vertex> &list, double Ax, double Ay, double Bx, double By, gpc_vertex *result)
{
    double r, s, Cx, Cy, Dx, Dy;
    
    for(int i = 0; i < list.size(); i++) {
        Cx = list[i].first.x; Cy = list[i].first.y;
        Dx = list[i].second.x; Dy = list[i].second.y;
        r = ((Ay - Cy) * (Dx - Cx) - (Ax - Cx) * (Dy - Cy)) / ((Bx - Ax) * (Dy - Cy) - (By - Ay) * (Dx - Cx));
        s = ((Ay - Cy) * (Bx - Ax) - (Ax - Cx) * (By - Ay)) / ((Bx - Ax) * (Dy - Cy) - (By - Ay) * (Dx - Cx));
        if(r >= 0 && r <= 1 && s >= 0 && s <= 1)
        {
            if(result) {
                result->x = Ax + r * (Bx - Ax);
                result->y = Ay + r * (By - Ay);
            }
            if((Ax == result->x && Ay == result->y) || (Bx == result->x && By == result->y));
            else return true;
        }
    }
    return false;
}

double distance(double x1, double y1, double x2, double y2)
{
    return sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1)); 
}

bool check_point_visibility(gpc_polygon &cur_pol, std::vector<pair_vertex> &list, int i, int x, double X, double Y, double X2, double Y2)
{
    bool edge = false;
    gpc_vertex result;
    if(!intersects(list,
                   X, Y,
                   X2, Y2, &result))
        edge = true;
    if(edge && (i == x)) {
        double middle_x = (X + X2) / 2;
        double middle_y = (Y + Y2) / 2;
        if(!poly_p_point_inside(&cur_pol, middle_x, middle_y))
            edge = false;
    }
    return edge;
}

void find_nearest_point(gpc_polygon &poly, gpc_vertex point, gpc_vertex *vertex)
{
    gpc_vertex goodpoint(-666, -666), reallygoodpoint(-666, -666);
    double mindist = 666666666;
    for(int i = 0; i < poly.contour.size(); i++) {
        int next_index = 0;
        int nbpoints = poly.contour[i].size();
        for(int j = 0; j < nbpoints; j++) {
            gpc_vertex p1 = poly.contour[i][j];
            gpc_vertex p2 = poly.contour[i][(j + 1) % nbpoints];
            double dist = (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);
            double r = ((point.x - p1.x) * (p2.x - p1.x) + (point.y - p1.y) * (p2.y - p1.y)) / dist;
            double s = ((p1.y - point.y) * (p2.x - p1.x) - (p1.x - point.x) * (p2.y - p1.y)) / dist;
            double Px = p1.x + r * (p2.x - p1.x);
            double Py = p1.y + r * (p2.y - p1.y);
            double d = fabs(s) * sqrt(dist);
            
            if(r >= 0 && r <= 1 && d < mindist) {
#ifdef DEBUG_PATHFINDER
                std::cout << "Segment [(" << p1.x << "," << p1.y << "),(" << p2.x << "," << p2.y << ")] Px=" << Px << " Py=" << Py << " d=" << d << " dist=" << dist << " r=" << r << " s=" << s << std::endl; 
#endif
                if(abs(Px - point.x) > abs(Py - point.y)) {
                    double coeff = (Py - point.y) / abs(Px - point.x);
                    Px += (Px - point.x) / abs(Px - point.x);
                    Py += coeff;
                }
                else {
                    double coeff = (Px - point.x) / abs(Py - point.y);
                    Py += (Py - point.y) / abs(Py - point.y);
                    Px += coeff;
                }
                mindist = d;
#ifdef DEBUG_PATHFINDER
                std::cout << "in poly ? " << poly_p_point_inside(&poly, Px, Py) << std::endl;
#endif
                reallygoodpoint = gpc_vertex(Px, Py);
            }
            
            double dist2 = distance(point.x, point.y, p1.x, p1.y);
            if(dist2 < mindist) {
#ifdef DEBUG_PATHFINDER
                std::cout << "vertice in poly ? " << p1.x << " " << p1.y << " " << poly_p_point_inside(&poly, p1.x, p1.y) << std::endl;
#endif
                mindist = dist2;
                reallygoodpoint = p1;
            }
        }
    }
    
    if(reallygoodpoint.x != -666 && reallygoodpoint.y != -666)
        goodpoint = reallygoodpoint;
    
    if(goodpoint.x != -666 && goodpoint.y != -666) {
        *vertex = goodpoint;
    }
}

int CPathfinder::find_path(CL_Point start, CL_Point goal)
{
#ifdef DEBUG_PATHFINDER
    std::cout << "finding path " << regions.size() << " " << start.x << " " << start.y << " " << goal.x << " " << goal.y << std::endl;
#endif
    int nbvertices;
    int regsize = regions.size();

    typedef Ppoly_t* tpoly;

    waypoints.clear();
    
    if(!regsize) {
        waypoints.push_back(start);
        waypoints.push_back(goal);
        return 1;
    }

    CGlumolRegion result_pol = *regions[0];
    for(int i = 1; i < regsize; i++) {
        nbvertices = 0;
        CGlumolRegion *reg = regions[i];
#ifdef DEBUG_PATHFINDER
        std::cout << "pouetpouet" << reg->type << " " << result_pol.type << std::endl;
#endif
        if(reg->type != result_pol.type) {
#ifdef DEBUG_PATHFINDER
            std::cout << "cur_pol -= *reg" << std::endl;
#endif
            result_pol -= *reg;
        }
        else {
#ifdef DEBUG_PATHFINDER
            std::cout << "cur_pol += *reg" << std::endl;
#endif
            result_pol += *reg;
        }
    }

    gpc_polygon cur_pol = *result_pol.getGpcSPolygon();
    gpc_vertex_list boundary;
    gpc_contours holes;
    
    int start_zone = -1;
    int goal_zone = -1;
    bool _break;
    for(int i = 0; i < cur_pol.contour.size(); i++) {
        if(!cur_pol.contour[i].hole) {
            for(int j = 0; j < cur_pol.contour[i].size(); j++) {
                int x = cur_pol.contour[i][j].x;
                int y = cur_pol.contour[i][j].y;
                if(start.x == x && start.y == y) {
                    start_zone = i;
                    _break = true;
                }
                if(goal.x == x && goal.y == y) {
                    goal_zone = i;
                    _break = true;
                }
            }
            if(poly_c_point_inside(&cur_pol.contour[i], start.x, start.y)) {
                start_zone = i;
                _break = true;
            }
            if(poly_c_point_inside(&cur_pol.contour[i], goal.x, goal.y)) {
                goal_zone = i;
                _break = true;
            }
            if(_break) break;
        }
        else {
            if(poly_c_point_inside(&cur_pol.contour[i], start.x, start.y)) {
                return -1;
            }
        }
    }
    
#ifdef DEBUG_PATHFINDER
    std::cout << "start_zone " << start_zone << " goal_zone " << goal_zone << std::endl;
#endif
    if(start_zone == -1)
        return -1;
    
    gpc_polygon pol;
    pol.contour.push_back(cur_pol.contour[start_zone]);

    for(int i = 0; i < cur_pol.contour.size(); i++) {
        if(cur_pol.contour[i].hole) {
            for(int j = 0; j < cur_pol.contour[i].size(); j++) {
                if(poly_c_point_inside(&cur_pol.contour[start_zone], cur_pol.contour[i][j].x, cur_pol.contour[i][j].y)) {
                    pol.contour.push_back(cur_pol.contour[i]);
                    break;
                }
            }
        }
    }
    
    cur_pol = pol;
    
    if(goal_zone == -1) {
        gpc_vertex result;
        find_nearest_point(cur_pol, gpc_vertex(goal.x, goal.y), &result);
#ifdef DEBUG_PATHFINDER
        std::cout << "Best point found " << result.x << " " << result.y << std::endl;
#endif
        int ix = result.x, iy = result.y;
        int i, j, X, Y;
        if(!poly_p_point_inside(&cur_pol, ix, iy)) {
            bool found = false;
            for(i = -1; !found && i <= 1; i++) {
                for(j = -1; !found && j <= 1; j++) {
                    if(i == 0 && j == 0) continue;
                    if(poly_p_point_inside(&cur_pol, ix + i, iy + j)) {
                        X = ix + i; Y = iy + j;
                        found = true;
                    }
                }
            }
            if(found) {
                result.x = X;
                result.y = Y;
            }
        }
        goal.x = result.x;
        goal.y = result.y;
        goal_zone = start_zone;
    }
    
    if(start_zone == -1 || goal_zone == -1 || (start_zone != goal_zone)) {
        return -1;
    }

    int *offsets = new int[cur_pol.contour.size()];
    offsets[0] = 0;
    int nbpoints = 0;
    for(int i = 0; i < cur_pol.contour.size(); i++) {
        nbpoints += cur_pol.contour[i].size();
        if(i)
            offsets[i] = offsets[i - 1] + cur_pol.contour[i - 1].size();
    }

    std::vector<pair_vertex> list;
    std::vector<gpc_vertex> vec;
    for(int i = 0; i < cur_pol.contour.size(); i++) {
        for(int j = 0; j < cur_pol.contour[i].size(); j++)  {
            list.push_back(pair_vertex(cur_pol.contour[i][j], cur_pol.contour[i][(j + 1) % cur_pol.contour[i].size()]));
            vec.push_back(cur_pol.contour[i][j]);
        }
    }
    vec.push_back(gpc_vertex(start.x, start.y));
    vec.push_back(gpc_vertex(goal.x, goal.y));
    
    if(check_point_visibility(cur_pol, list, -1, -2, start.x, start.y, goal.x, goal.y)) {
        waypoints.push_back(goal);
        delete [] offsets;
        return 0;
    }
    
#ifdef DEBUG_PATHFINDER
    std::cout << "Graph has " << nbpoints + 2 << " points" << std::endl;
#endif
    Graph g(nbpoints + 2);
    property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, g);
           
    int z = 0;
    graph_traits<Graph>::edge_descriptor e; bool inserted;
    for(int i = 0, n = 0; i < cur_pol.contour.size(); z += cur_pol.contour[i++].size()) {
        for(int j = 0; j < cur_pol.contour[i].size(); j++, n++)  { // Pour tous les points du polygone
            tie(e, inserted) = add_edge(n, z + (j + 1) % cur_pol.contour[i].size(), g);
            weightmap[e] = distance(cur_pol.contour[i][j].x, cur_pol.contour[i][j].y,
                                    cur_pol.contour[i][(j + 1) % cur_pol.contour[i].size()].x,
                                    cur_pol.contour[i][(j + 1) % cur_pol.contour[i].size()].y);

            tie(e, inserted) = add_edge(n, z + (j == 0 ? cur_pol.contour[i].size() - 1: j - 1), g);
            weightmap[e] = distance(cur_pol.contour[i][j].x, cur_pol.contour[i][j].y,
                                    cur_pol.contour[i][j == 0 ? cur_pol.contour[i].size() - 1: j - 1].x,
                                    cur_pol.contour[i][j == 0 ? cur_pol.contour[i].size() - 1: j - 1].y);
            
            int x, y;
            for(int x = i; x < cur_pol.contour.size(); x++) {
                for(int y = (i == x) ? j + 2 : 0; y < cur_pol.contour[x].size(); y++) {
                    int m = offsets[x] + y;
                    if(check_point_visibility(cur_pol, list, i, x, cur_pol.contour[i][j].x, cur_pol.contour[i][j].y,
                                              cur_pol.contour[x][y].x, cur_pol.contour[x][y].y)) {
                        tie(e, inserted) = add_edge(n, m, g);
                        weightmap[e] = distance(cur_pol.contour[i][j].x, cur_pol.contour[i][j].y,
                                                cur_pol.contour[x][y].x, cur_pol.contour[x][y].y);
                    }
                }
            }
        }
    }
    
    for(int x = 0; x < cur_pol.contour.size(); x++) {
        for(int y = 0; y < cur_pol.contour[x].size(); y++) {
            if(check_point_visibility(cur_pol, list, -1, -2, start.x, start.y, cur_pol.contour[x][y].x, cur_pol.contour[x][y].y)) {
#ifdef DEBUG_PATHFINDER
                std::cout << "Add edge " << nbpoints << " " << offsets[x] + y << std::endl;
#endif
                tie(e, inserted) = add_edge(nbpoints, offsets[x] + y, g);
                weightmap[e] = distance(start.x, start.y,
                                        cur_pol.contour[x][y].x, cur_pol.contour[x][y].y);
            }
#ifdef DEBUG_PATHFINDER
            else
                std::cout << "Intersection" << std::endl;
#endif
            
            if(check_point_visibility(cur_pol, list, -1, -2, goal.x, goal.y, cur_pol.contour[x][y].x, cur_pol.contour[x][y].y)) {
#ifdef DEBUG_PATHFINDER
                std::cout << "Add edge2 " << nbpoints + 1 << " " << offsets[x] + y << std::endl;
#endif
                tie(e, inserted) = add_edge(nbpoints + 1, offsets[x] + y, g);
                weightmap[e] = distance(goal.x, goal.y,
                                        cur_pol.contour[x][y].x, cur_pol.contour[x][y].y);
            }    
#ifdef DEBUG_PATHFINDER
            else
                std::cout << "Intersection2" << std::endl;
#endif
        }
    }

#ifdef DEBUG_PATHFINDER
    boost::property_map<Graph, vertex_index_t>::type 
        vertex_id = get(vertex_index, g);
    boost::property_map<Graph, edge_weight_t>::type
        trans_delay = get(edge_weight, g);
    
    std::cout << "vertices(g) = ";
    typedef graph_traits<Graph>::vertex_iterator vertex_iter;
    std::pair<vertex_iter, vertex_iter> vp;
    for (vp = vertices(g); vp.first != vp.second; ++vp.first)
        std::cout << vec[get(vertex_id, *vp.first)].x << "," << vec[get(vertex_id, *vp.first)].y <<  " ";
    std::cout << std::endl;
    
    std::cout << "edges(g) = ";
    graph_traits<Graph>::edge_iterator ei, ei_end;
    for (tie(ei,ei_end) = edges(g); ei != ei_end; ++ei)
        std::cout << "(" << get(vertex_id, source(*ei, g))
            << "," << get(vertex_id, target(*ei, g)) << ") ";
    std::cout << std::endl;
#endif
    
    typedef graph_traits < Graph >::vertex_descriptor vertex_descriptor;
    std::vector<vertex_descriptor> p(num_vertices(g));
    std::vector<double> d(num_vertices(g));
    vertex_descriptor s = vertex(nbpoints, g);
    
    dijkstra_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));
    
    for(int i = 0; i < p.size(); i++)
        std::cout << "p " << i << " " << p[i] << " d " << d[i] << std::endl;
    
#ifdef DEBUG_PATHFINDER
    std::cout << "i = nbpoints + 1 " << i << std::endl;
#endif
    int i = nbpoints + 1;
    while(p[i] != i && i != nbpoints) {
        waypoints.push_back(CL_Point(vec[i].x, vec[i].y));
#ifdef DEBUG_PATHFINDER
        std::cout << "p[i] " << p[i] << " i " << i << " " << vec[p[i]].x << " " << vec[p[i]].y << std::endl;
#endif
        i = p[i];
    }

    delete [] offsets;

    return 0;
}
