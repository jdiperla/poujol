/*
    This software may only be used by you under license from AT&T Corp.
    ("AT&T").  A copy of AT&T's Source Code Agreement is available at
    AT&T's Internet website having the URL:
    <http://www.research.att.com/sw/tools/graphviz/license/source.html>
    If you received this software without first entering into a license
    with AT&T, you have an infringing copy of this software and cannot use
    it without violating AT&T's intellectual property rights.
*/

#ifndef VISIBILITY_H
#define VISIBILITY_H

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
//#include <vispath.h>
#include <pathgeom.h>


typedef COORD** array2;
typedef	unsigned char boolean;

#define	OBSCURED	0.0
#define EQ(p,q)		((p.x == q.x) && (p.y == q.y))
#define NEQ(p,q)	(!EQ(p,q))
#define NIL(p)		((p)0)
#define	CW			0
#define	CCW			1

struct vconfig_s {
	int			Npoly;
	int			N;		/* number of points in walk of barriers */
	Ppoint_t	*P;		/* barrier points */
	int			*start;
	int			*next;	
	int			*prev;

	/* this is computed from the above */
	array2		vis;
} ;

extern "C" COORD* ptVis (vconfig_t *, int, Ppoint_t);
extern "C" int directVis (Ppoint_t, int, Ppoint_t, int, vconfig_t *);
extern "C" void visibility (vconfig_t *);
extern "C" int* makePath (Ppoint_t p, int pp, COORD* pvis,
               Ppoint_t q, int qp, COORD* qvis,
               vconfig_t* conf);

extern "C" vconfig_t *Pobsopen(Ppoly_t **obstacles, int n_obstacles);

/* close a visibility graph, freeing its storage */
extern "C" void Pobsclose(vconfig_t *config);

/* route a polyline from p0 to p1, avoiding obstacles.
 * if an endpoint is inside an obstacle, pass the polygon's index >=0
 * if the endpoint is not inside an obstacle, pass POLYID_NONE
 * if the endpoint location is not known, pass POLYID_UNKNOWN
 */

extern "C" int Pobspath(vconfig_t *config, Ppoint_t p0, int poly0,
	Ppoint_t p1, int poly1, 
	Ppolyline_t *output_route);

#define POLYID_NONE		-1111
#define POLYID_UNKNOWN	-2222
	       
#endif
