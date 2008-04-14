/* 	$Id: PolyUtil.cpp,v 1.1.1.1 2005/03/10 02:35:11 bob Exp $	 */
#include "stdafx.h"
#include "PolyUtil.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

gpc_polygon * poly_p_new() {
  gpc_polygon *p = new(gpc_polygon);
  return p;
}

/*int poly_p_clone(gpc_polygon *s, gpc_polygon *t) {
    gpc_vertex_list *svl, *tvl;
    gpc_vertex *sv, *tv;
    int i, j;
    // may be done much faster with copying whole memory areas? /
    t->contour.size() = s->contour.size();
    t->contour.init(s->contour.size()); //.contour = (gpc_vertex_list *)malloc(sizeof(gpc_vertex_list) * t->contour.size());
    t->hole = (int *)malloc(sizeof(int)*t->contour.size());
    if ((! t->contour.size()) || (! t->hole)) return -1;
    for (i=0; i < t->contour.size(); i++) {
	t->hole[i] = s->hole[i];
	svl = s->contour.contour + i;
	tvl = t->contour.contour + i;
	tvl->size() = svl->size();
	if (! (tvl->vertex = (gpc_vertex *)malloc(sizeof(gpc_vertex) * tvl->size())))
	    return -1;
	memcpy(tvl->vertex, svl->vertex, sizeof(gpc_vertex) * svl->size());
    }
    return 0;
}
*/
double poly_c_area(gpc_vertex_list *vl) {
  unsigned int i;
  double a = 0.0;
  for (i=0; i < vl->size()-1; i++)
      a += ((vl->at(i).x + vl->at(i+1).x) *
	    (vl->at(i+1).y - vl->at(i).y));
  /* close the loop */
  a += ((vl->at(i).x + vl->at(0).x) *
	(vl->at(0).y - vl->at(i).y));
  return 0.5*fabs(a);
}

double poly_p_area(gpc_polygon *p) {
  unsigned int i;
  double a = 0.0;
  for (i = 0; i < p->contour.size(); i++)
    a += ((p->contour[i].hole) ? -1.0 : 1.0) * poly_c_area(&p->contour[i]);
  return a;
}
/*
int poly_c_orientation(gpc_vertex_list *vl) {
  // return 1 for cw, -1 for ccw) /
  gpc_vertex *v, *a, *b, *c;
  int i, m;
  double area;
  // find lowest rightmost vertex /
  v = b = vl->vertex;
  m = 0;
  for (i=0; i < vl->size(); i++) {
    v = vl->vertex + i;
    if ((v->y < b->y) || ((v->y == b->y) && (v->x > b->x))) {
      m = i; b = v;
    }
  }
  // now m is the lowest rightmost vertex, find orientation /
  a = vl->vertex + ((m + (vl->size()-1)) % vl->size());
  c = vl->vertex + ((m+1) % vl->size());
  area = (a->x*b->y - a->y*b->x + a->y*c->x - a->x*c->y + 
	  b->x*c->y - c->x*b->y);
  if (area > 0)
    return 1;
  else if (area < 0)
    return -1;
  else
    return 0;
}
*/
int poly_c_center(gpc_vertex_list *vl, double *cx, double *cy){
  return 1;
  /*
  gpc_vertex *v;
  double x=0.0, y=0.0, a;
  int i;
  for (i=0; i < vl->size()-1; i++) {
    v = vl->vertex + i;
    a = v->x * (v+1)->y - (v+1)->x * v->y;
    x += (v->x + (v+1)->x) * a;
    y += (v->y + (v+1)->y) * a;
  }
  // close the loop /
  v = vl->vertex + (vl->size()-1);
  a = v->x * vl->vertex->y - vl->vertex->x * v->y;
  x += (v->x + vl->vertex->x) * a;
  y += (v->y + vl->vertex->y) * a;
  // this algorithm needs ccw ordered points,
     so we have to calculate the order and multiply,
     what a waste of cpu power... 
     If you know a better way, let me know... //
  a = 6.0*poly_c_area(vl)*poly_c_orientation(vl);
  if (a == 0)
      return 1;
  *cx = x / a;
  *cy = y / a;
  return 0;
  */
}

int poly_p_center(gpc_polygon *p, double *cx, double *cy){
  double *x, *y, *a, A=0.0, X=0.0, Y=0.0;
  unsigned int i;
  size_t s;
  s = sizeof(double)*p->contour.size();
  a =  (double *)malloc(s);
  x = (double *)malloc(s);
  y = (double *)malloc(s);
  for (i=0; i < p->contour.size(); i++) {
    a[i] = ((p->contour[i].hole) ? -1.0 : 1.0) * poly_c_area(&p->contour[i]);
	if (poly_c_center(&p->contour[i], x+i, y+i) != 0)
	return 1;
  }
  for (i=0; i < p->contour.size(); i++)
    A += a[i];
  for (i=0; i < p->contour.size(); i++) {
    X += a[i] * x[i];
    Y += a[i] * y[i];
  }
  if (A == 0)
      return 1;
  *cx = X / A;
  *cy = Y / A;
  free(a);
  free(x);
  free(y);
  return 0;
}

int poly_c_point_inside(gpc_vertex_list *vl, double x, double y){
  unsigned int i, j;
  int c=0;
  gpc_vertex vi, vj;
  for (i=0, j=vl->size()-1; i < vl->size(); j = i++) {
      vi = vl->at(i);
      vj = vl->at(j);
      if ((((vi.y <= y) && (y < vj.y)) || ((vj.y <= y) && (y < vi.y))))
	  if ((x < (vj.x - vi.x) * (y - vi.y) / (vj.y - vi.y) + vi.x))
	      c = !c;
  }
  return c;
}

int poly_p_point_inside(gpc_polygon *p, double x, double y){
  unsigned int i;
  int inSolid = 0, inHole = 0;
  for(i=0; i< p->contour.size(); i++) {
      /* loop over solid contours */
      if (p->contour[i].hole == 0) {
	  inSolid = poly_c_point_inside(&p->contour[i], x, y);
	  if (inSolid == -1)
	      return inSolid; /* error */
	  if (inSolid > 0)
	      break;
      }
  }
  if (inSolid == 0) 
      return 0;
  for(i=0; i < p->contour.size(); i++) {
      /* loop over holes */
      if (p->contour[i].hole == 1) {
	  inHole = poly_c_point_inside(&p->contour[i], x, y);
	  if (inHole == -1)
	      return inHole; /* error */
	  if (inHole > 0) 
	      break;
      }
  }
  return ((inHole > 0) ? 0 : 1);
}

void poly_c_boundingbox(gpc_vertex_list *vl, double *x0, double *x1, double *y0, double *y1) {
    unsigned int i;
    gpc_vertex v;
    v = vl->at(0);
    *x0 = *x1 = v.x;
    *y0 = *y1 = v.y;
    for (i=1; i < vl->size(); i++) {
	v = vl->at(i);
	if (v.x < *x0) *x0 = v.x;
	if (v.x > *x1) *x1 = v.x;
	if (v.y < *y0) *y0 = v.y;
	if (v.y > *y1) *y1 = v.y;
    }
}

void poly_p_boundingbox(gpc_polygon *p, double *x0, double *x1, double *y0, double *y1) {
  unsigned int i;
  double X0, X1, Y0, Y1; 
  if (p->contour.size() <= 0) {
    *x0 = *x1 = *y0 = *y1 = 0.0;
    return;
  }
  poly_c_boundingbox(&p->contour[0], x0, x1, y0, y1);
  for (i=1; i < p->contour.size(); i++) {
    poly_c_boundingbox(&p->contour[i], &X0, &X1, &Y0, &Y1);
    if (X0 < *x0) *x0 = X0;
    if (X1 > *x1) *x1 = X1;
    if (Y0 < *y0) *y0 = Y0;
    if (Y1 > *y1) *y1 = Y1;
  }
}

void poly_p_shift(gpc_polygon *p, double x, double y) {
  unsigned int i, j;
  gpc_vertex_list *vl;
  for (i=0; i < p->contour.size(); i++) {
    vl = &p->contour[i];
    for (j=0; j < vl->size(); j++) {
      vl->at(j).x += x;
      vl->at(j).y += y;
    }
  }
}

void poly_p_scale(gpc_polygon *p, double xs, double ys, 
	     double xc, double yc) {
  unsigned int i, j;
  gpc_vertex_list *vl;
  for (i=0; i < p->contour.size(); i++) {
    vl = &p->contour[i];
    for (j=0; j < vl->size(); j++) {
      vl->at(j).x = xc + xs * (vl->at(j).x - xc);
      vl->at(j).y = yc + ys * (vl->at(j).y - yc);
    }
  }
}

void poly_p_rotate(gpc_polygon *p, double alpha, 
	      double xc, double yc) {
  unsigned int i, j;
  double x, y, l, a;
  gpc_vertex_list *vl;
  for (i=0; i < p->contour.size(); i++) {
    vl = &p->contour[i];
    for (j=0; j < vl->size(); j++) {
      x = vl->at(j).x - xc;
      y = vl->at(j).y - yc;
      l = sqrt(x*x + y*y);
      a = alpha + ((l != 0.0) ? acos(x/l) * 
		   ((y>0.0) ? 1.0 : -1.0) : 0.0);
      vl->at(j).x = xc + l * cos(a);
      vl->at(j).y = yc + l * sin(a);
    }
  }
}

void poly_p_warpToBox(gpc_polygon *p, double x0, double x1, 
		 double y0, double y1, double *bb) {
  unsigned int i, j;
  double xscale, yscale, bx0, bx1, by0, by1;
  if (bb) {
      bx0 = bb[0];
      bx1 = bb[1];
      by0 = bb[2];
      by1 = bb[3];
  } else
      poly_p_boundingbox(p, &bx0, &bx1, &by0, &by1);
  xscale = ((bx1 > bx0) ? (x1-x0)/(bx1-bx0) : 1.0); 
  yscale = ((by1 > by0) ? (y1-y0)/(by1-by0) : 1.0);
  for (i=0; i < p->contour.size(); i++) {
    for (j=0; j < p->contour[i].size(); j++) {
      p->contour[i][j].x = x0 + xscale*(p->contour[i][j].x - bx0);
      p->contour[i][j].y = y0 + yscale*(p->contour[i][j].y - by0);
    }
  }
}

void poly_p_flip(gpc_polygon *p, double x) {
  unsigned int i, j;
  double x2 = 2.0 * x;
  gpc_vertex_list *vl;
  for (i=0; i < p->contour.size(); i++) {
    vl = &p->contour[i];
    for (j=0; j < vl->size(); j++) {
      vl->at(j).x = x2 - vl->at(j).x;
    }
  }
}

void poly_p_flop(gpc_polygon *p, double y) {
  unsigned int i, j;
  double y2 = 2.0 * y;
  gpc_vertex_list *vl;
  for (i=0; i < p->contour.size(); i++) {
    vl = &p->contour[i];
    for (j=0; j < vl->size(); j++) {
      vl->at(j).y = y2 - vl->at(j).y;
    }
  }
}


