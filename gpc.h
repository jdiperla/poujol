
/*
   ===========================================================================

Project:   Generic Polygon Clipper

A new algorithm for calculating the difference, intersection,
exclusive-or or union of arbitrary polygon sets.

File:      gpc.h
Author:    Alan Murta (email: gpc@cs.man.ac.uk)
Version:   2.31
Date:      4th June 1999

Copyright: (C) 1997-1999, Advanced Interfaces Group,
University of Manchester.

This software is free for non-commercial use. It may be copied,
modified, and redistributed provided that this copyright notice
is preserved on all copies. The intellectual property rights of
the algorithms used reside with the University of Manchester
Advanced Interfaces Group.

You may not use this software, in whole or in part, in support
of any commercial product without the express consent of the
author.

There is no warranty or other guarantee of fitness of this
software for any purpose. It is provided solely "as is".

===========================================================================
 */

#ifndef __gpc_h
#define __gpc_h

#include <stdio.h>
#include <vector>
#include "poujol.h"

#undef _POSIX_C_SOURCE

/*
#include <boost/python/list.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/detail/api_placeholder.hpp>
#include <boost/python/extract.hpp>

using namespace boost::python;
*/

/*
   ===========================================================================
   Constants
   ===========================================================================
 */

/* Increase GPC_EPSILON to encourage merging of near coincident edges    */

/*#define GPC_EPSILON (DBL_EPSILON)*/
#define GPC_EPSILON (1e-3)

#define GPC_VERSION "2.31"


/*
   ===========================================================================
   Public Data Types
   ===========================================================================
 */

typedef enum                        /* Set operation type                */
{
        GPC_DIFF,                         /* Difference                  */
        GPC_INT,                          /* Intersection                */
        GPC_XOR,                          /* Exclusive or                */
        GPC_UNION                         /* Union                       */
} gpc_op;

struct gpc_vertex                      /* Polygon vertex structure          */
{
        double              x;            /* Vertex x component          */
        double              y;            /* vertex y component          */

		gpc_vertex()
		{
			x = 0.0;
			y = 0.0;
		}

		gpc_vertex(double X, double Y)
		{
			x = X;
			y = Y;
		}

		/*
		operator list()
		{
			list l;
			l.append(x);
			l.append(y);
			return l;
		}
		*/

        gpc_vertex operator-(const gpc_vertex &v)
        {
            return gpc_vertex(x-v.x,y-v.y);
        }
        
		bool operator==(const gpc_vertex &v)
		{
			return (x == v.x) && (y == v.y);
		}

		void set_x(double X) { x = X; }
		double get_x() { return x; }
		void set_y(double Y) { y = Y; }
		double get_y() { return y; }
};

struct gpc_vertex_list : public std::vector<gpc_vertex>                    /* Vertex list structure             */
{
		bool flag;
        //gpc_vertex         *vertex;       /* Vertex array pointer        */
		int hole;

		void init(int size)
		{
			clear();
			for(; size; size--)
				push_back(gpc_vertex());
		}

		gpc_vertex_list()
		{
			flag = false;
			hole = 0;
		}

		gpc_vertex_list(bool bhole)
		{
			flag = false;
			hole = bhole ? 1 : 0;
		}

		gpc_vertex_list(std::vector<gpc_vertex>::iterator it1, std::vector<gpc_vertex>::iterator it2)
		{
			flag = false;
		}

		/*
		static gpc_vertex_list *from_list(list l)
		{
			gpc_vertex_list *vl = new gpc_vertex_list();
			int n = boost::python::len(l);
			for(int i = 0; i < n; i++) {
				vl->push_back(gpc_vertex(extract<double>(l[i][0]), extract<double>(l[i][1])));
			}
			return vl;
		}

		*/

		bool operator==(const gpc_vertex_list &list)
		{
			if(list.size() != size())
				return false;
			for(unsigned int i = 0; i < size(); i++) {
				if(!(at(i) == list[i]))
					return false;
			}
			return true;
		}
};

typedef gpc_vertex_list *p_vertex_list;

class gpc_contours : public std::vector<gpc_vertex_list>
{
public:
	//gpc_vertex_list *contour;      /* Contour array pointer          */
	gpc_contours()
	{
	}

	gpc_contours(std::vector<gpc_vertex_list>::iterator it1, std::vector<gpc_vertex_list>::iterator it2)
	{
	}

	void init(int size)
	{
		clear();
		for(; size; size--)
			push_back(gpc_vertex_list());
	}

	void init(gpc_vertex_list *l)
	{
		//std::cout << "IIIIIIIIIIINNNNNNNNNNNNNNNNIIIIIIIIIIIIIIIIIITTTTTTTTTTTTTT" << std::endl;
		//
		//for(int i = 0; i < l->num_vertices; i++)
		//	push_back(l[i]);
	}

	gpc_vertex_list &get_item(int index) {
		return at(index);
	}

	void set_item(int index, gpc_vertex_list *l)
	{
	}

	
	gpc_vertex_list &operator[](int index) {
		return at(index);
	}

	/*
	gpc_vertex_list *operator[](int index) {
		return at(index);
	}
	*/

	int get_length()
	{
		return (int) size();
	}

	void remove_item(int index)
	{
		//std::list<CGlumolSprite*>::iterator index;
		/*
		i = index;
		for( = children.begin(); i != children.end(); i++) {
			if( (*i)->IsDead() ) 

				children.remove( (*
		for(int i = index + 1; i < contour.size(); i++) {
			contour[i - 1] = contour[i];
		}
		contour->num_vertices--;
		*/
	}

	void append_item(gpc_vertex_list *vl)
	{
	}

};

class gpc_polygon                     /* Polygon set structure             */
{
public:
	gpc_contours		contour;
	
	gpc_polygon()
	{
	}

	~gpc_polygon()
	{
	}
};

typedef struct                      /* Tristrip set structure            */
{
        int                 num_strips;   /* Number of tristrips               */
        gpc_vertex_list    *strip;        /* Tristrip array pointer            */
} gpc_tristrip;


/*
   ===========================================================================
   Public Function Prototypes
   ===========================================================================
 */

void gpc_init_polygon       (gpc_polygon     * polygon);


void gpc_copy_vertex        (gpc_vertex       * source,
                gpc_vertex       * dest);

void gpc_copy_vertex_list   (gpc_vertex_list  * source,
                gpc_vertex_list  * dest);

void gpc_copy_polygon       (gpc_polygon      * source,
                gpc_polygon      * dest);

void gpc_create_rectangle   (double x,double y,double width,double height,gpc_polygon *p);

void gpc_pie_polygon        (double             x_center,
                double             y_center,
                double             radius,
                double             theta_min,
                double             theta_max,
                int                num_edges, 
                gpc_polygon      * p);


void gpc_create_polygon     (double * points,int size, gpc_polygon *p);


void gpc_read_polygon        (FILE            *infile_ptr, 
                int              read_hole_flags,
                gpc_polygon     *polygon);

void gpc_write_polygon       (FILE            *outfile_ptr,
                int              write_hole_flags,
                gpc_polygon     *polygon);

void gpc_write_tristrip(FILE *fp, gpc_polygon *p);

void gpc_add_contour         (gpc_polygon     *polygon,
                gpc_vertex_list *contour,
                int              hole);

int gpc_test_intersection    (gpc_polygon     *p1,
                gpc_polygon     *p2);


unsigned int gpc_get_clip_number();
void gpc_polygon_clip        (gpc_op           set_operation,
                gpc_polygon     *subject_polygon,
                gpc_polygon     *clip_polygon,
                gpc_polygon     *result_polygon);

void gpc_tristrip_clip       (gpc_op           set_operation,
                gpc_polygon     *subject_polygon,
                gpc_polygon     *clip_polygon,
                gpc_tristrip    *result_tristrip);

void gpc_polygon_to_tristrip (gpc_polygon     *polygon,
                gpc_tristrip    *tristrip);



double gpc_poly_area         (gpc_polygon     *polygon);

double gpc_tristrip_area     (gpc_tristrip    *tristrip);


void gpc_free_polygon        (gpc_polygon     *polygon);

void gpc_free_tristrip       (gpc_tristrip    *tristrip);

void gpc_transform_polygon(gpc_polygon *source,
                double theta,double x, double y,gpc_polygon *dest);


void gpc_find_bounding_circle_radius(gpc_polygon *p,gpc_vertex *center,double *r);

void gpc_get_center(gpc_polygon *p,gpc_vertex *v);

void gpc_filter_polygon(gpc_polygon * poly);

gpc_vertex_list *  gpc_extract_vertex_list(gpc_polygon *p,
                int cont,int first, int last);

void gpc_cut_polygon(gpc_polygon *poly,gpc_polygon *result);

#endif

/*
   ===========================================================================
   End of file: gpc.h
   ===========================================================================
 */

