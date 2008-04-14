
#include "stdafx.h"

/*********************************************
 *
 * Cedric Pradalier
 * DEA 2000/2001 
 * INRIA Rhones Alpes
 * http://cedric.pradalier.free.fr/index.html
 * mail : cedric.pradalier at free.fr
 *
 * *******************************************/
#include "polygon.h"
#include <math.h>
#include "poujol.h"

#ifndef _WIN32_WCE
#include <iostream>
#endif

//#define EQ(X,Y) (fabs(X-Y)<1e-3)
//#undef EQ
//#define EQ(X,Y) (fabs(X-Y)<=1)
    
SPolygon::~SPolygon()
{
    //printf("*");
    gpc_free_polygon(poly);
    delete poly;
}

void SPolygon::Read(FILE * fp) 
{
    gpc_free_polygon(poly);
    delete poly;
    poly = new gpc_polygon;
    gpc_init_polygon(poly);
    gpc_read_polygon(fp,true,poly);
}


SPolygon * SPolygon::Union(SPolygon * p)
{
    SPolygon * r = new SPolygon();
    gpc_polygon_clip(GPC_UNION,poly,p->poly,r->poly);
    return r;
}

SPolygon * SPolygon::Intersection(SPolygon * p)
{
    SPolygon * r = new SPolygon();
    gpc_polygon_clip(GPC_INT,poly,p->poly,r->poly);
    return r;
}

SPolygon * SPolygon::Minus(SPolygon * p)
{
    SPolygon * r = new SPolygon();
    gpc_polygon_clip(GPC_DIFF,poly,p->poly,r->poly);
    return r;
}

SPolygon * SPolygon::Xor(SPolygon * p)
{
    SPolygon * r = new SPolygon();
    gpc_polygon_clip(GPC_XOR,poly,p->poly,r->poly);
    return r;
}


/*void SPolygon::getConnexList(Vector * list)
{
        int i;
        SPolygon * P;
        gpc_polygon * gpc_P;
        if (numPartConnex()==1)
        {
                list->addElement(new SPolygon(this));
                return;
        }

        for(i=0;i<poly->contour.size();i++)
        {
                if (!(poly->hole[i]))
                {
                        gpc_P = new gpc_polygon;
                        gpc_init_polygon(gpc_P);
                        gpc_add_contour(gpc_P,&(poly->contour[i]),poly->hole[i]);
                        P = new SPolygon(gpc_P);
                        list->addElement(Intersection(P));
                        delete P;
                }
        }
}
*/

void SPolygon::getEdges(Vector &list)
{
    int i,j;
    for(i=0;i<poly->contour.size();i++)
    {
        int n = poly->contour[i].size();
        for(j=0;j<n-1;j++)
            list.push_back(Segment(
                        Vector2(poly->contour[i][j].x,
                            poly->contour[i][j].y),
                        Vector2(poly->contour[i][j+1].x,
                            poly->contour[i][j+1].y)));
        list.push_back(Segment(
                    Vector2(poly->contour[i][n-1].x,
                        poly->contour[i][n-1].y),
                    Vector2(poly->contour[i][0].x,
                        poly->contour[i][0].y)));
    }
    // list.trimToSize();
}


    

bool SPolygon::isAVertex(double x,double y)
{
    for(unsigned int i=0;i<poly->contour.size();i++)
        for(unsigned int j=0;j<poly->contour[i].size();j++)
            if ((fabs(poly->contour[i][j].x - x) < 1e-6) &&
                    (fabs(poly->contour[i][j].y - y) < 1e-6))
        //  if ((poly->contour[i][j].x == x) &&
        //          (poly->contour[i][j].y == y))
                return true;
    return false;
}



void SPolygon::getCenter(double * x, double * y)
{
    gpc_vertex pt;
    gpc_get_center(poly,&pt);
    *x = pt.x;
    *y = pt.y;
}
    

void SPolygon::getBoundingCircle(double * x, double * y,double * r)
{
    gpc_vertex pt;
    gpc_find_bounding_circle_radius(poly,&pt,r);
    *x = pt.x;
    *y = pt.y;
}
    


double SPolygon::getArea()
{
    return gpc_poly_area(poly);
}


int SPolygon::numPartConnex()
{
    unsigned int i,n;
    n = 0;
    for (i=0;i<poly->contour.size();i++)
        if (poly->contour[i].hole == 0) n+=1;
    return n;
}
    

void SPolygon::cut()
{
    gpc_polygon * result = new gpc_polygon;
    gpc_cut_polygon(poly,result);
    gpc_free_polygon(poly);
    delete poly;
    poly = result;
}   



// #define DEBUG

SPolygon * SPolygon::computeVisibility(double x,double y)
{
/*    Vector edges,obsEdges;
    double triangle[6];
    double quadri[8];
    SPolygon *visibility = new SPolygon();
    SPolygon *T,*tmp;
    Vector2 balise(x,y);
    int i,j;
    double mu;

    triangle[0] = x;
    triangle[1] = y;
    getEdges(edges);

    for (i=0;i<edges.size();i++)
    {
        Segment s = edges[i];
        triangle[2]= s.first.x;
        triangle[3]= s.first.y;
        triangle[4]= s.second.x;
        triangle[5]= s.second.y;
        T = new SPolygon(triangle,6);
        SPolygon * V = Intersection(T);// workspace inter T
        if (V->numPartConnex() > 1) {
            delete T;delete V;continue;
        }
        if (!V->isAVertex(balise.x,balise.y))  {
            delete T;delete V;continue;
        }
        SPolygon * O = T->Minus(V);
        O->getEdges(obsEdges); 
        delete T;
        if (obsEdges.size() == 0)
        {
            tmp = visibility;
            visibility = visibility->Union(V);
            delete tmp;
            delete V;
            continue;
        }

        
        
        
        for (j=0;j<obsEdges.size();j++)
        {
            Segment o = obsEdges[j];
            quadri[0]=o.first.x; quadri[1]=o.first.y;
            quadri[2]=o.second.x; quadri[3]=o.second.y;
            Vector2 dirA = (o.first-balise);
            if (EQ(dirA.norme(),0)) continue;
            double lambdaA=s->intersect(balise,dirA,&mu);
            if (lambdaA < 0)
            {
                if (EQ(mu,1))
                {
                    quadri[6]=s.second.x;
                    quadri[7]=s.second.y;
                }
                if EQ(mu,0)
                {
                    quadri[6]=s.first.x;
                    quadri[7]=s.first.y;
                }
            }
            else
            {
                quadri[6]=balise.x+lambdaA*dirA.x;
                quadri[7]=balise.y+lambdaA*dirA.y;
            }
            Vector2 dirB = (o->B-balise);
            if (EQ(dirB.norme(),0)) continue;
            double lambdaB=s->intersect(balise,dirB,&mu);
            if (lambdaB < 0)
            {
                if (EQ(mu,1))
                {
                    quadri[4]=s.second.x;
                    quadri[5]=s.second.y;
                }
                if EQ(mu,0)
                {
                    quadri[4]=s.first.x;
                    quadri[5]=s.first.y;
                }
            }
            else
            {
                quadri[4]=balise.x+lambdaB*dirB.x;
                quadri[5]=balise.y+lambdaB*dirB.y;
            }
            SPolygon * suppr = new SPolygon(quadri,8);
            T = V->Minus(suppr);
            delete suppr;
            delete V;
            V = T;
        }
        obsEdges.clear();
        delete O;
        T = visibility->Union(V);
        delete V;
        delete visibility;
        visibility = T;
    }
    edges.clear();

    return visibility;
*/
    return NULL;
}


void SPolygon::Print2(std::ostream &f, bool write_hole_flags)
{
	gpc_polygon *p = poly;
    unsigned int c, v;

	f << p->contour.size() << std::endl;
    for (c= 0; c < p->contour.size(); c++)
    {
		f << p->contour[c].size() << std::endl;

        if (write_hole_flags)
			f << p->contour[c].hole << std::endl;

        for (v= 0; v < p->contour[c].size(); v++)
            //fprintf(fp, "% .*lf % .*lf\n",
            //        DBL_DIG, p->contour[c][v].x,
            //        DBL_DIG, p->contour[c][v].y);
			f << float(p->contour[c][v].x )<< " " << 
				float(p->contour[c][v].y) << std::endl;
    }
}

#include "gpc.h"
#include <stdlib.h>
#include <float.h>
#include <math.h>

static unsigned int callNumber = 0;

unsigned int gpc_get_clip_number() {return callNumber;}

/*
   ===========================================================================
   Constants
   ===========================================================================
 */

#ifndef TRUE
#define FALSE              0
#define TRUE               1
#endif

#define LEFT               0
#define RIGHT              1

#define ABOVE              0
#define BELOW              1

#define CLIP               0
#define SUBJ               1

#define INVERT_TRISTRIPS   FALSE


/*
   ===========================================================================
   Macros 
   ===========================================================================
 */

#undef EQ
#define EQ(a, b)           (fabs((a) - (b)) <= GPC_EPSILON)

#define PREV_INDEX(i, n)   ((i - 1 + n) % n)
#define NEXT_INDEX(i, n)   ((i + 1    ) % n)

#define OPTIMAL(v, i, n)   ((v[PREV_INDEX(i, n)].y != v[i].y) || \
        (v[NEXT_INDEX(i, n)].y != v[i].y))

#define FWD_MIN(v, i, n)   ((v[PREV_INDEX(i, n)].vertex.y >= v[i].vertex.y) \
        && (v[NEXT_INDEX(i, n)].vertex.y > v[i].vertex.y))

#define NOT_FMAX(v, i, n)   (v[NEXT_INDEX(i, n)].vertex.y > v[i].vertex.y)

#define REV_MIN(v, i, n)   ((v[PREV_INDEX(i, n)].vertex.y > v[i].vertex.y) \
        && (v[NEXT_INDEX(i, n)].vertex.y >= v[i].vertex.y))

#define NOT_RMAX(v, i, n)   (v[PREV_INDEX(i, n)].vertex.y > v[i].vertex.y)

#define VERTEX(e,p,s,x,y)  {add_vertex(&((e)->outp[(p)]->v[(s)]), x, y); \
    (e)->outp[(p)]->active++;}

#define P_EDGE(d,e,p,i,j)  {(d)= (e); \
    do {(d)= (d)->prev;} while (!(d)->outp[(p)]); \
        (i)= (d)->bot.x + (d)->dx * ((j)-(d)->bot.y);}

#define N_EDGE(d,e,p,i,j)  {(d)= (e); \
    do {(d)= (d)->next;} while (!(d)->outp[(p)]); \
        (i)= (d)->bot.x + (d)->dx * ((j)-(d)->bot.y);}

#define MALLOC(p, b, s)    {if ((b) > 0) { \
    p= malloc(b); if (!(p)) { \
        fprintf(stderr, "gpc malloc failure: %s\n", s); \
            exit(0);}} else p= NULL;}

#define FREE(p)            {if (p) {free(p); (p)= NULL;}}


            /*
               ===========================================================================
               Private Data Types
               ===========================================================================
             */

            typedef enum                        /* Edge intersection classes         */
{
    NUL,                              /* Empty non-intersection            */
    EMX,                              /* External maximum                  */
    ELI,                              /* External left intermediate        */
    TED,                              /* Top edge                          */
    ERI,                              /* External right intermediate       */
    RED,                              /* Right edge                        */
    IMM,                              /* Internal maximum and minimum      */
    IMN,                              /* Internal minimum                  */
    EMN,                              /* External minimum                  */
    EMM,                              /* External maximum and minimum      */
    LED,                              /* Left edge                         */
    ILI,                              /* Internal left intermediate        */
    BED,                              /* Bottom edge                       */
    IRI,                              /* Internal right intermediate       */
    IMX,                              /* Internal maximum                  */
    FUL                               /* Full non-intersection             */
} vertex_type;



typedef enum                        /* Horizontal edge states            */
{
    NH,                               /* No horizontal edge                */
    BH,                               /* Bottom horizontal edge            */
    TH                                /* Top horizontal edge               */
} h_state;

typedef enum                        /* Edge bundle state                 */
{
    UNBUNDLED,                        /* Isolated edge not within a bundle */
    BUNDLE_HEAD,                      /* Bundle head node                  */
    BUNDLE_TAIL                       /* Passive bundle tail node          */
} bundle_state;

typedef struct v_shape              /* Internal vertex list datatype     */
{
    double              x;            /* X coordinate component            */
    double              y;            /* Y coordinate component            */
    struct v_shape     *next;         /* Pointer to next vertex in list    */
} vertex_node;

typedef struct p_shape              /* Internal contour / tristrip type  */
{
    int                 active;       /* Active flag / vertex count        */
    int                 hole;         /* Hole / external contour flag      */
    vertex_node        *v[2];         /* Left and right vertex list ptrs   */
    struct p_shape     *next;         /* Pointer to next polygon contour   */
    struct p_shape     *proxy;        /* Pointer to actual structure used  */
} polygon_node;

typedef struct edge_shape
{
    gpc_vertex          vertex;       /* Piggy-backed contour vertex data  */
    gpc_vertex          bot;          /* Edge lower (x, y) coordinate      */
    gpc_vertex          top;          /* Edge upper (x, y) coordinate      */
    double              xb;           /* Scanbeam bottom x coordinate      */
    double              xt;           /* Scanbeam top x coordinate         */
    double              dx;           /* Change in x for a unit y increase */
    int                 type;         /* Clip / subject edge flag          */
    int                 bundle[2][2]; /* Bundle edge flags                 */
    int                 bside[2];     /* Bundle left / right indicators    */
    bundle_state        bstate[2];    /* Edge bundle state                 */
    polygon_node       *outp[2];      /* Output polygon / tristrip pointer */
    struct edge_shape  *prev;         /* Previous edge in the AET          */
    struct edge_shape  *next;         /* Next edge in the AET              */
    struct edge_shape  *pred;         /* Edge connected at the lower end   */
    struct edge_shape  *succ;         /* Edge connected at the upper end   */
    struct edge_shape  *next_bound;   /* Pointer to next bound in LMT      */
} edge_node;

typedef struct lmt_shape            /* Local minima table                */
{
    double              y;            /* Y coordinate at local minimum     */
    edge_node          *first_bound;  /* Pointer to bound list             */
    struct lmt_shape   *next;         /* Pointer to next local minimum     */
} lmt_node;

typedef struct sbt_t_shape          /* Scanbeam tree                     */
{
    double              y;            /* Scanbeam node y value             */
    struct sbt_t_shape *less;         /* Pointer to nodes with lower y     */
    struct sbt_t_shape *more;         /* Pointer to nodes with higher y    */
} sb_tree;

typedef struct it_shape             /* Intersection table                */
{
    edge_node          *ie[2];        /* Intersecting edge (bundle) pair   */
    gpc_vertex          point;        /* Point of intersection             */
    struct it_shape    *next;         /* The next intersection table node  */
} it_node;

typedef struct st_shape             /* Sorted edge table                 */
{
    edge_node          *edge;         /* Pointer to AET edge               */
    double              xb;           /* Scanbeam bottom x coordinate      */
    double              xt;           /* Scanbeam top x coordinate         */
    double              dx;           /* Change in x for a unit y increase */
    struct st_shape    *prev;         /* Previous edge in sorted list      */
} st_node;

typedef struct bbox_shape           /* Contour axis-aligned bounding box */
{
    double             xmin;          /* Minimum x coordinate              */
    double             ymin;          /* Minimum y coordinate              */
    double             xmax;          /* Maximum x coordinate              */
    double             ymax;          /* Maximum y coordinate              */
} bbox;


/*
   ===========================================================================
   Global Data
   ===========================================================================
 */

/* Horizontal edge state transitions within scanbeam boundary */
const h_state next_h_state[3][6]=
{
    /*        ABOVE     BELOW     CROSS */
    /*        L   R     L   R     L   R */  
    /* NH */ {BH, TH,   TH, BH,   NH, NH},
    /* BH */ {NH, NH,   NH, NH,   TH, TH},
    /* TH */ {NH, NH,   NH, NH,   BH, BH}
};


/*
   ==========================================================================
   Fast garbage collector
   ==========================================================================
 */

typedef struct gc_item
{
    struct gc_item * next;
    void * mem;
} * gc_list;

gc_list GLOBAL_GC=NULL;


void gc_push(void * obj)
{
    gc_list h=NULL;
    h = (gc_list)(malloc(sizeof(struct gc_item)));
    if (h == NULL) 
    {
        fprintf(stderr,"Couldn't allocate memory for garbage collect\n");
        exit(1);
    }
    h->next = GLOBAL_GC;
    h->mem = obj;
    GLOBAL_GC = h;
}

void gc_free()
{
    gc_list g = GLOBAL_GC,t=NULL;
    while (g != NULL)
    {
        t = g;
        g = g->next;
        free(t->mem);
        free(t);
    }
    GLOBAL_GC = NULL;
}






/*
   ===========================================================================
   Private Functions
   ===========================================================================
 */

/*static*/ void reset_it(it_node **it)
{
    it_node *itn;

    while (*it)
    {
        itn= (*it)->next;
        FREE(*it);
        *it= itn;
    }
}


/*static*/ void reset_lmt(lmt_node **lmt)
{
    lmt_node *lmtn;

    while (*lmt)
    {
        lmtn= (*lmt)->next;
        FREE(*lmt);
        *lmt= lmtn;
    }
}


#define SQR(x) ((x)*(x))

double gpc_vertex_norme(gpc_vertex *v)
{
    return sqrt(SQR(v->x)+SQR(v->y));
}

double gpc_vertex_dist(gpc_vertex *v1,gpc_vertex *v2)
{
    return sqrt(SQR(v1->x-v2->x)+SQR(v1->y-v2->y));
}


/*static*/ void insert_bound(edge_node **b, edge_node *e)
{
    edge_node *existing_bound;

    if (!*b)
    {
        /* Link node e to the tail of the list */
        *b= e;
    }
    else
    {
        /* Do primary sort on the x field */
        if (e[0].bot.x < (*b)[0].bot.x)
        {
            /* Insert a new node mid-list */
            existing_bound= *b;
            *b= e;
            (*b)->next_bound= existing_bound;
        }
        else
        {
            if (e[0].bot.x == (*b)[0].bot.x)
            {
                /* Do secondary sort on the dx field */
                if (e[0].dx < (*b)[0].dx)
                {
                    /* Insert a new node mid-list */
                    existing_bound= *b;
                    *b= e;
                    (*b)->next_bound= existing_bound;
                }
                else
                {
                    /* Head further down the list */
                    insert_bound(&((*b)->next_bound), e);
                }
            }
            else
            {
                /* Head further down the list */
                insert_bound(&((*b)->next_bound), e);
            }
        }
    }
}


/*static*/ edge_node **bound_list(lmt_node **lmt, double y)
{
    lmt_node *existing_node;

    if (!*lmt)
    {
        /* Add node onto the tail end of the LMT */
        *lmt = (lmt_node*) malloc(sizeof(lmt_node));
        (*lmt)->y= y;
        (*lmt)->first_bound= NULL;
        (*lmt)->next= NULL;
        return &((*lmt)->first_bound);
    }
    else
        if (y < (*lmt)->y)
        {
            /* Insert a new LMT node before the current node */
            existing_node= *lmt;
            *lmt = (lmt_node*) malloc(sizeof(lmt_node));
            (*lmt)->y= y;
            (*lmt)->first_bound= NULL;
            (*lmt)->next= existing_node;
            return &((*lmt)->first_bound);
        }
        else
            if (y > (*lmt)->y)
                /* Head further up the LMT */
                return bound_list(&((*lmt)->next), y);
            else
                /* Use this existing LMT node */
                return &((*lmt)->first_bound);
}


/*static*/ void add_to_sbtree(int *entries, sb_tree **sbtree, double y)
{
    if (!*sbtree)
    {
        /* Add a new tree node here */
        *sbtree = (sb_tree*) malloc(sizeof(sb_tree));
        (*sbtree)->y= y;
        (*sbtree)->less= NULL;
        (*sbtree)->more= NULL;
        (*entries)++;
    }
    else
    {
        if ((*sbtree)->y > y)
        {
            /* Head into the 'less' sub-tree */
            add_to_sbtree(entries, &((*sbtree)->less), y);
        }
        else
        {
            if ((*sbtree)->y < y)
            {
                /* Head into the 'more' sub-tree */
                add_to_sbtree(entries, &((*sbtree)->more), y);
            }
        }
    }
}


/*static*/ void build_sbt(int *entries, double *sbt, sb_tree *sbtree)
{
    if (sbtree->less)
        build_sbt(entries, sbt, sbtree->less);
    sbt[*entries]= sbtree->y;
    (*entries)++;
    if (sbtree->more)
        build_sbt(entries, sbt, sbtree->more);
}


/*static*/ void free_sbtree(sb_tree **sbtree)
{
    if (*sbtree)
    {
        free_sbtree(&((*sbtree)->less));
        free_sbtree(&((*sbtree)->more));
        FREE(*sbtree);
    }
}


/*static*/ int count_optimal_vertices(gpc_vertex_list c)
{
    int result= 0;
    unsigned int i;

    /* Ignore non-contributing contours */
    if (c.size() > 0)
    {
        for (i= 0; i < c.size(); i++)
            /* Ignore superfluous vertices embedded in horizontal edges */
            if (OPTIMAL(c, i, c.size()))
                result++;
    }
    return result;
}


/*static*/ edge_node *build_lmt(lmt_node **lmt, sb_tree **sbtree,
        int *sbt_entries, gpc_polygon *p, int type,
        gpc_op op)
{
    int          min, max, num_edges, v, num_vertices;
    int          total_vertices= 0, e_index=0;
    int c, i;
    edge_node    *e, *edge_table;

    for (c= 0; c < (int) p->contour.size(); c++)
        total_vertices+= count_optimal_vertices(p->contour[c]);

    /* Create the entire input polygon edge table in one go */
    edge_table = (edge_node*) malloc(total_vertices * sizeof(edge_node));

    for (c= 0; c < (int) p->contour.size(); c++)
    {
        if (p->contour[c].flag)
        {
			p->contour[c].flag = false;
            /* Ignore the non-contributing contour and repair the vertex count */
            //p->contour[c].size()= -p->contour[c].size();/////////////////
        }
        else
        {
            /* Perform contour optimisation */
            num_vertices= 0;
            for (i= 0; i < (int) p->contour[c].size(); i++)
                if (OPTIMAL(p->contour[c], i, p->contour[c].size()))
                {
                    edge_table[num_vertices].vertex.x= p->contour[c][i].x;
                    edge_table[num_vertices].vertex.y= p->contour[c][i].y;

                    /* Record vertex in the scanbeam table */
                    add_to_sbtree(sbt_entries, sbtree,
                            edge_table[num_vertices].vertex.y);

                    num_vertices++;
                }

            /* Do the contour forward pass */
            for (min= 0; min < num_vertices; min++)
            {
                /* If a forward local minimum... */
                if (FWD_MIN(edge_table, min, num_vertices))
                {
                    /* Search for the next local maximum... */
                    num_edges= 1;
                    max= NEXT_INDEX(min, num_vertices);
                    while (NOT_FMAX(edge_table, max, num_vertices))
                    {
                        num_edges++;
                        max= NEXT_INDEX(max, num_vertices);
                    }

                    /* Build the next edge list */
                    e= &edge_table[e_index];
                    e_index+= num_edges;
                    v= min;
                    e[0].bstate[BELOW]= UNBUNDLED;
                    e[0].bundle[BELOW][CLIP]= FALSE;
                    e[0].bundle[BELOW][SUBJ]= FALSE;
                    for (i= 0; i < num_edges; i++)
                    {
                        e[i].xb= edge_table[v].vertex.x;
                        e[i].bot.x= edge_table[v].vertex.x;
                        e[i].bot.y= edge_table[v].vertex.y;

                        v= NEXT_INDEX(v, num_vertices);

                        e[i].top.x= edge_table[v].vertex.x;
                        e[i].top.y= edge_table[v].vertex.y;
                        e[i].dx= (edge_table[v].vertex.x - e[i].bot.x) /
                            (e[i].top.y - e[i].bot.y);
                        e[i].type= type;
                        e[i].outp[ABOVE]= NULL;
                        e[i].outp[BELOW]= NULL;
                        e[i].next= NULL;
                        e[i].prev= NULL;
                        e[i].succ= ((num_edges > 1) && (i < (num_edges - 1))) ?
                            &(e[i + 1]) : NULL;
                        e[i].pred= ((num_edges > 1) && (i > 0)) ? &(e[i - 1]) : NULL;
                        e[i].next_bound= NULL;
                        e[i].bside[CLIP]= (op == GPC_DIFF) ? RIGHT : LEFT;
                        e[i].bside[SUBJ]= LEFT;
                    }
                    insert_bound(bound_list(lmt, edge_table[min].vertex.y), e);
                }
            }

            /* Do the contour reverse pass */
            for (min= 0; min < num_vertices; min++)
            {
                /* If a reverse local minimum... */
                if (REV_MIN(edge_table, min, num_vertices))
                {
                    /* Search for the previous local maximum... */
                    num_edges= 1;
                    max= PREV_INDEX(min, num_vertices);
                    while (NOT_RMAX(edge_table, max, num_vertices))
                    {
                        num_edges++;
                        max= PREV_INDEX(max, num_vertices);
                    }

                    /* Build the previous edge list */
                    e= &edge_table[e_index];
                    e_index+= num_edges;
                    v= min;
                    e[0].bstate[BELOW]= UNBUNDLED;
                    e[0].bundle[BELOW][CLIP]= FALSE;
                    e[0].bundle[BELOW][SUBJ]= FALSE;
                    for (i= 0; i < num_edges; i++)
                    {
                        e[i].xb= edge_table[v].vertex.x;
                        e[i].bot.x= edge_table[v].vertex.x;
                        e[i].bot.y= edge_table[v].vertex.y;

                        v= PREV_INDEX(v, num_vertices);

                        e[i].top.x= edge_table[v].vertex.x;
                        e[i].top.y= edge_table[v].vertex.y;
                        e[i].dx= (edge_table[v].vertex.x - e[i].bot.x) /
                            (e[i].top.y - e[i].bot.y);
                        e[i].type= type;
                        e[i].outp[ABOVE]= NULL;
                        e[i].outp[BELOW]= NULL;
                        e[i].next= NULL;
                        e[i].prev= NULL;
                        e[i].succ= ((num_edges > 1) && (i < (num_edges - 1))) ?
                            &(e[i + 1]) : NULL;
                        e[i].pred= ((num_edges > 1) && (i > 0)) ? &(e[i - 1]) : NULL;
                        e[i].next_bound= NULL;
                        e[i].bside[CLIP]= (op == GPC_DIFF) ? RIGHT : LEFT;
                        e[i].bside[SUBJ]= LEFT;
                    }
                    insert_bound(bound_list(lmt, edge_table[min].vertex.y), e);
                }
            }
        }
    }
    return edge_table;
}


/*static*/ void add_edge_to_aet(edge_node **aet, edge_node *edge, edge_node *prev)
{
    if (!*aet)
    {
        /* Append edge onto the tail end of the AET */
        *aet= edge;
        edge->prev= prev;
        edge->next= NULL;
    }
    else
    {
        /* Do primary sort on the xb field */
        if (edge->xb < (*aet)->xb)
        {
            /* Insert edge here (before the AET edge) */
            edge->prev= prev;
            edge->next= *aet;
            (*aet)->prev= edge;
            *aet= edge;
        }
        else
        {
            if (edge->xb == (*aet)->xb)
            {
                /* Do secondary sort on the dx field */
                if (edge->dx < (*aet)->dx)
                {
                    /* Insert edge here (before the AET edge) */
                    edge->prev= prev;
                    edge->next= *aet;
                    (*aet)->prev= edge;
                    *aet= edge;
                }
                else
                {
                    /* Head further into the AET */
                    add_edge_to_aet(&((*aet)->next), edge, *aet);
                }
            }
            else
            {
                /* Head further into the AET */
                add_edge_to_aet(&((*aet)->next), edge, *aet);
            }
        }
    }
}


/*static*/ void add_intersection(it_node **it, edge_node *edge0, edge_node *edge1,
        double x, double y)
{
    it_node *existing_node;

    if (!*it)
    {
        /* Append a new node to the tail of the list */
        *it = (it_node*) malloc(sizeof(it_node));
        (*it)->ie[0]= edge0;
        (*it)->ie[1]= edge1;
        (*it)->point.x= x;
        (*it)->point.y= y;
        (*it)->next= NULL;
    }
    else
    {
        if ((*it)->point.y > y)
        {
            /* Insert a new node mid-list */
            existing_node= *it;
            *it = (it_node*) malloc(sizeof(it_node));
            (*it)->ie[0]= edge0;
            (*it)->ie[1]= edge1;
            (*it)->point.x= x;
            (*it)->point.y= y;
            (*it)->next= existing_node;
        }
        else
            /* Head further down the list */
            add_intersection(&((*it)->next), edge0, edge1, x, y);
    }
}


/*static*/ void add_st_edge(st_node **st, it_node **it, edge_node *edge,
        double dy)
{
    st_node *existing_node;
    double   den, r, x, y;

    if (!*st)
    {
        /* Append edge onto the tail end of the ST */
        *st = (st_node*) malloc(sizeof(st_node));
        (*st)->edge= edge;
        (*st)->xb= edge->xb;
        (*st)->xt= edge->xt;
        (*st)->dx= edge->dx;
        (*st)->prev= NULL;
    }
    else
    {
        den= ((*st)->xt - (*st)->xb) - (edge->xt - edge->xb);

        /* If new edge and ST edge don't cross */
        if ((edge->xt >= (*st)->xt) || (edge->dx == (*st)->dx) || 
                (fabs(den) <= DBL_EPSILON))
        {
            /* No intersection - insert edge here (before the ST edge) */
            existing_node= *st;
            *st = (st_node*) malloc(sizeof(st_node));
            (*st)->edge= edge;
            (*st)->xb= edge->xb;
            (*st)->xt= edge->xt;
            (*st)->dx= edge->dx;
            (*st)->prev= existing_node;
        }
        else
        {
            /* Compute intersection between new edge and ST edge */
            r= (edge->xb - (*st)->xb) / den;
            x= (*st)->xb + r * ((*st)->xt - (*st)->xb);
            y= r * dy;

            /* Insert the edge pointers and the intersection point in the IT */
            add_intersection(it, (*st)->edge, edge, x, y);

            /* Head further into the ST */
            add_st_edge(&((*st)->prev), it, edge, dy);
        }
    }
}


/*static*/ void build_intersection_table(it_node **it, edge_node *aet, double dy)
{
    st_node   *st, *stp;
    edge_node *edge;

    /* Build intersection table for the current scanbeam */
    reset_it(it);
    st= NULL;

    /* Process each AET edge */
    for (edge= aet; edge; edge= edge->next)
    {
        if ((edge->bstate[ABOVE] == BUNDLE_HEAD) ||
                edge->bundle[ABOVE][CLIP] || edge->bundle[ABOVE][SUBJ])
            add_st_edge(&st, it, edge, dy);
    }

    /* Free the sorted edge table */
    while (st)
    {
        stp= st->prev;
        FREE(st);
        st= stp;
    }
}

/*static*/ int count_contours(polygon_node *polygon)
{
    int          nc, nv;
    vertex_node *v, *nextv;

    for (nc= 0; polygon; polygon= polygon->next)
        if (polygon->active)
        {
            /* Count the vertices in the current contour */
            nv= 0;
            for (v= polygon->proxy->v[LEFT]; v; v= v->next)
                nv++;

            /* Record valid vertex counts in the active field */
            if (nv > 2)
            {
                polygon->active= nv;
                nc++;
            }
            else
            {
                /* Invalid contour: just free the heap */
                for (v= polygon->proxy->v[LEFT]; v; v= nextv)
                {
                    nextv= v->next;
                    FREE(v);
                }
                polygon->active= 0;
            }
        }
    return nc;
}


/*static*/ void add_left(polygon_node *p, double x, double y)
{
    vertex_node *nv;
    if (p==NULL) return;

    /* Create a new vertex node and set its fields */
    nv = (vertex_node*) malloc(sizeof(vertex_node));
    nv->x= x;
    nv->y= y;

    /* Add vertex nv to the left end of the polygon's vertex list */
    nv->next= p->proxy->v[LEFT];

    /* Update proxy->[LEFT] to point to nv */
    p->proxy->v[LEFT]= nv;
}


/*static*/ void merge_left(polygon_node *p, polygon_node *q, polygon_node *list)
{
    polygon_node *target;

    if ((p==NULL)||(q==NULL)) return;
    /* Label contour as a hole */
    q->proxy->hole= TRUE;

    if (p->proxy != q->proxy)
    {
        /* Assign p's vertex list to the left end of q's list */
        p->proxy->v[RIGHT]->next= q->proxy->v[LEFT];
        q->proxy->v[LEFT]= p->proxy->v[LEFT];

        /* Redirect any p->proxy references to q->proxy */

        for (target= p->proxy; list; list= list->next)
        {
            if (list->proxy == target)
            {
                list->active= FALSE;
                list->proxy= q->proxy;
            }
        }
    }
}


/*static*/ void add_right(polygon_node *p, double x, double y)
{
    vertex_node *nv;
    if (p==NULL) return;

    /* Create a new vertex node and set its fields */
    nv = (vertex_node*) malloc(sizeof(vertex_node));
    nv->x= x;
    nv->y= y;
    nv->next= NULL;

    /* Add vertex nv to the right end of the polygon's vertex list */
    p->proxy->v[RIGHT]->next= nv;

    /* Update proxy->v[RIGHT] to point to nv */
    p->proxy->v[RIGHT]= nv;
}


/*static*/ void merge_right(polygon_node *p, polygon_node *q, polygon_node *list)
{
    polygon_node *target;
    if ((p==NULL)||(q==NULL)) return;

    /* Label contour as external */
    q->proxy->hole= FALSE;

    if (p->proxy != q->proxy)
    {
        /* Assign p's vertex list to the right end of q's list */
        q->proxy->v[RIGHT]->next= p->proxy->v[LEFT];
        q->proxy->v[RIGHT]= p->proxy->v[RIGHT];

        /* Redirect any p->proxy references to q->proxy */
        for (target= p->proxy; list; list= list->next)
        {
            if (list->proxy == target)
            {
                list->active= FALSE;
                list->proxy= q->proxy;
            }
        }
    }
}


/*static*/ void add_local_min(polygon_node **p, edge_node *edge,
        double x, double y)
{
    polygon_node *existing_min;
    vertex_node  *nv;

    existing_min= *p;

    *p = (polygon_node*) malloc(sizeof(polygon_node));
    gc_push(*p);

    /* Create a new vertex node and set its fields */
    nv = (vertex_node*) malloc(sizeof(vertex_node));
    nv->x= x;
    nv->y= y;
    nv->next= NULL;

    /* Initialise proxy to point to p itself */
    (*p)->proxy= (*p);
    (*p)->active= TRUE;
    (*p)->next= existing_min;

    /* Make v[LEFT] and v[RIGHT] point to new vertex nv */
    (*p)->v[LEFT]= nv;
    (*p)->v[RIGHT]= nv;

    /* Assign polygon p to the edge */
    edge->outp[ABOVE]= *p;
}


/*static*/ int count_tristrips(polygon_node *tn)
{
    int total;

    for (total= 0; tn; tn= tn->next)
        if (tn->active > 2)
            total++;
    return total;
}


/*static*/ void add_vertex(vertex_node **t, double x, double y)
{
    if (!(*t))
    {
        *t = (vertex_node*) malloc(sizeof(vertex_node));
        (*t)->x= x;
        (*t)->y= y;
        (*t)->next= NULL;
    }
    else
        /* Head further down the list */
        add_vertex(&((*t)->next), x, y);
}


/*static*/ void new_tristrip(polygon_node **tn, edge_node *edge,
        double x, double y)
{
    if (!(*tn))
    {
        *tn = (polygon_node*) malloc(sizeof(polygon_node));
        (*tn)->next= NULL;
        (*tn)->v[LEFT]= NULL;
        (*tn)->v[RIGHT]= NULL;
        (*tn)->active= 1;
        add_vertex(&((*tn)->v[LEFT]), x, y); 
        edge->outp[ABOVE]= *tn;
    }
    else
        /* Head further down the list */
        new_tristrip(&((*tn)->next), edge, x, y);
}


/*static*/ bbox *create_contour_bboxes(gpc_polygon *p)
{
    bbox *box;
    unsigned int   c, v;

    box = (bbox*) malloc(p->contour.size() * sizeof(bbox));

    /* Construct contour bounding boxes */
    for (c= 0; c < p->contour.size(); c++)
    {
        /* Initialise bounding box extent */
        box[c].xmin= DBL_MAX;
        box[c].ymin= DBL_MAX;
        box[c].xmax= -DBL_MAX;
        box[c].ymax= -DBL_MAX;

        for (v= 0; v < p->contour[c].size(); v++)
        {
            /* Adjust bounding box */
            if (p->contour[c][v].x < box[c].xmin)
                box[c].xmin= p->contour[c][v].x;
            if (p->contour[c][v].y < box[c].ymin)
                box[c].ymin= p->contour[c][v].y;
            if (p->contour[c][v].x > box[c].xmax)
                box[c].xmax= p->contour[c][v].x;
            if (p->contour[c][v].y > box[c].ymax)
                box[c].ymax= p->contour[c][v].y;
        }
    }
    return box;  
}


/*static*/ void minimax_test(gpc_polygon *subj, gpc_polygon *clip, gpc_op op)
{
    bbox *s_bbox, *c_bbox;
    unsigned int   s, c;
    int *o_table, overlap;

    s_bbox= create_contour_bboxes(subj);
    c_bbox= create_contour_bboxes(clip);

    o_table = (int*) malloc(subj->contour.size() * clip->contour.size() * sizeof(int));

    /* Check all subject contour bounding boxes against clip boxes */
    for (s= 0; s < subj->contour.size(); s++)
        for (c= 0; c < clip->contour.size(); c++)
            o_table[c * subj->contour.size() + s]=
                (!((s_bbox[s].xmax < c_bbox[c].xmin) ||
                   (s_bbox[s].xmin > c_bbox[c].xmax))) &&
                (!((s_bbox[s].ymax < c_bbox[c].ymin) ||
                   (s_bbox[s].ymin > c_bbox[c].ymax)));

    /* For each clip contour, search for any subject contour overlaps */
    for (c= 0; c < clip->contour.size(); c++)
    {
        overlap= 0;
        for (s= 0; (!overlap) && (s < subj->contour.size()); s++)
            overlap= o_table[c * subj->contour.size() + s];

        if (!overlap)
            /* Flag non contributing status by negating vertex count */
            clip->contour[c].flag = true;
			//clip->contour[c].size() = -clip->contour[c].size();///////////////
    }  

    if (op == GPC_INT)
    {  
        /* For each subject contour, search for any clip contour overlaps */
        for (s= 0; s < subj->contour.size(); s++)
        {
            overlap= 0;
            for (c= 0; (!overlap) && (c < clip->contour.size()); c++)
                overlap= o_table[c * subj->contour.size() + s];

            if (!overlap)
                /* Flag non contributing status by negating vertex count */
			    clip->contour[c].flag = true;
               //subj->contour[s].size() = -subj->contour[s].size();/////////////////////
        }  
    }

    FREE(s_bbox);
    FREE(c_bbox);
    FREE(o_table);
}


/*
   ===========================================================================
   Public Functions
   ===========================================================================
 */

void gpc_init_polygon(gpc_polygon * p)
{
}


void gpc_copy_vertex(gpc_vertex * source,gpc_vertex * dest)
{
    dest->x = source->x;
    dest->y = source->y;
}

void gpc_copy_vertex_list(gpc_vertex_list * source,gpc_vertex_list * dest)
{
    unsigned int i;
	dest->init(source->size()); // = (gpc_vertex*) malloc(source->size()*sizeof(gpc_vertex));
    for(i=0;i<source->size();i++)
        gpc_copy_vertex(&(source->at(i)),&(dest->at(i)));
}

void gpc_copy_polygon(gpc_polygon * source,gpc_polygon * dest)
{
    unsigned int i;
	//dest->hole = (int*) malloc(source->contour.size()*sizeof(int));
    dest->contour.init(source->contour.size()); // = (gpc_vertex_list*) malloc(*sizeof(gpc_vertex_list));
	for(i=0;i<source->contour.size();i++)
    {
        //dest->hole[i]=source->hole[i];
		dest->contour[i].hole=source->contour[i].hole; // Ajout�par Bob
		gpc_copy_vertex_list(&(source->contour[i]),&(dest->contour[i]));
    }
}



void gpc_free_polygon(gpc_polygon *p)
{
}

void gpc_create_polygon (double * points,int size, gpc_polygon *p)
{
    int v;
	p->contour.init(1); //p->contour.size()); // = (gpc_vertex_list*) malloc(p->contour.size() * sizeof(gpc_vertex_list));
    //p->contour[0].size() = size / 2;
    p->contour[0].hole = FALSE;
	//p->hole[0] = FALSE;
	p->contour[0].init(size / 2); //vertex = (gpc_vertex*) malloc(p->contour[0].size()
            //* sizeof(gpc_vertex));
    for (v= 0; v < size/2 ; v++)
    {
        p->contour[0][v].x = points[2*v];
        p->contour[0][v].y = points[2*v+1];
    }
}

void gpc_pie_polygon(double x_center,double y_center,
        double radius,double theta_min,double theta_max,
        int num_edges, gpc_polygon *p)
{
    int v;
    double theta_step;
    if (num_edges<2)
    {
        gpc_init_polygon(p);
        return;
    }

    theta_step = (theta_max-theta_min)/num_edges;

    //p->hole = (int*) malloc(p->contour.size() * sizeof(int));
    p->contour.init(1); // = (gpc_vertex_list*) malloc(p->contour.size()
            //* sizeof(gpc_vertex_list));
    p->contour[0].hole = FALSE;
	//p->hole[0] = FALSE;
    p->contour[0].init(num_edges + 1); //vertex = (gpc_vertex*) malloc(p->contour[0].size()
            //* sizeof(gpc_vertex));
    p->contour[0][0].x = x_center;
    p->contour[0][0].y = y_center;
    for (v= 0; v <= num_edges ; v++)
    {
        p->contour[0][v+1].x = x_center+
            radius*cos(theta_min+v*theta_step);
        p->contour[0][v+1].y = y_center+
            radius*sin(theta_min+v*theta_step);
    }
}

void gpc_create_rectangle(double x,double y,double width,double height,gpc_polygon *p)
{
    //p->hole = (int*) malloc(1 * sizeof(int));
    p->contour.init(1); // = (gpc_vertex_list*) malloc(1 * sizeof(gpc_vertex_list));
    p->contour[0].hole = FALSE;
	//p->hole[0] = FALSE;
	p->contour[0].init(4); //vertex = (gpc_vertex*) malloc(4 * sizeof(gpc_vertex));
//    p->contour[0].size() = 4;
    p->contour[0][0].x = x;
    p->contour[0][0].y = y;
    p->contour[0][1].x = x;
    p->contour[0][1].y = y+height;
    p->contour[0][2].x = x+width;
    p->contour[0][2].y = y+height;
    p->contour[0][3].x = x+width;
    p->contour[0][3].y = y;
}


void gpc_read_polygon(FILE *fp, int read_hole_flags, gpc_polygon *p)
{
    unsigned int c, v, s;
    
    fscanf(fp, "%d", &s);
    //p->hole = (int*) malloc(p->contour.size() * sizeof(int));
	p->contour.init(s); // = (gpc_vertex_list*) malloc(p->contour.size()
            // * sizeof(gpc_vertex_list));
    for (c= 0; c < p->contour.size(); c++)
    {
	unsigned int size;
        fscanf(fp, "%d", &size);

        if (read_hole_flags)
            fscanf(fp, "%d", &(p->contour[c].hole));
		else {
            p->contour[c].hole= FALSE; /* Assume all contours to be external */
			p->contour[c].hole = FALSE;
		}
        p->contour[c].init(size);// = (gpc_vertex*) malloc(p->contour[c].size()
		//* sizeof(gpc_vertex));
        for (v= 0; v < size; v++)
            fscanf(fp, "%lf %lf", &(p->contour[c][v].x),
                    &(p->contour[c][v].y));
    }
}


void gpc_write_tristrip(FILE *fp, gpc_polygon *p)
{
    int c, v;
    gpc_tristrip tris;
    gpc_polygon_to_tristrip(p,&tris);

    fprintf(fp, "%d\n", tris.num_strips);
    for (c= 0; c < tris.num_strips; c++)
    {
        fprintf(fp, "%d\n", tris.strip[c].size());
        for (v= 0; v < (int) tris.strip[c].size(); v++)
            fprintf(fp, "%f %f\n",
                    tris.strip[c][v].x,
                    tris.strip[c][v].y);
    }
    gpc_free_tristrip(&tris);
}

void gpc_write_polygon(FILE *fp, int write_hole_flags, gpc_polygon *p)
{
    unsigned int c, v;

    fprintf(fp, "%d\n", p->contour.size());
    for (c= 0; c < p->contour.size(); c++)
    {
        fprintf(fp, "%d\n", p->contour[c].size());

        if (write_hole_flags)
            fprintf(fp, "%d\n", p->contour[c].hole);

        for (v= 0; v < p->contour[c].size(); v++)
            //fprintf(fp, "% .*lf % .*lf\n",
            //        DBL_DIG, p->contour[c][v].x,
            //        DBL_DIG, p->contour[c][v].y);
            fprintf(fp, "%f %f\n",
                    p->contour[c][v].x,
                    p->contour[c][v].y);
    }
}


gpc_vertex_list *  gpc_extract_vertex_list(gpc_polygon *p,
        int cont,int first, int last)
{
    int i;
    gpc_vertex_list * contour;
    contour = (gpc_vertex_list*) malloc(sizeof(gpc_vertex_list));
    //contour->size() = last - first + 1;
    contour->init(last - first + 1); // = (gpc_vertex*) malloc((last - first + 1) * sizeof(gpc_vertex));
	for (i=first;i<=last;i++)
        contour->at(i-first) = p->contour[cont][i];

    return contour;
}



void gpc_add_contour(gpc_polygon *p, gpc_vertex_list *new_contour, int hole)
{
    //new_contour->hole = hole;
    //p->contour.push_back(new_contour);
    
    //return;
    std::cout << "gpc_add_contour" << std::endl;
    
    int             *extended_hole;
    unsigned int c, v;
    gpc_vertex_list *extended_contour;

    /* Create an extended hole array */
    extended_hole = (int*) malloc((p->contour.size() + 1)
            * sizeof(int));

    /* Create an extended contour array */
    extended_contour = new gpc_vertex_list(); //[p->contour.size() + 1]; //(gpc_vertex_list*) malloc((p->contour.size() + 1)
            //* sizeof(gpc_vertex_list));

    // extended_contour->init(p->contour.size() + 1);
    
    
    /* Copy the old contour and hole data into the extended arrays */
    for (c= 0; c < p->contour.size(); c++)
    {
        // extended_contour[c].hole= p->contour[c].hole;
        // extended_contour->push_back(p->contour[c]);
        extended_contour[c]= p->contour[c];
	extended_contour[c].hole = p->contour[c].hole;
    }

    /* Copy the new contour and hole onto the end of the extended arrays */
    c= p->contour.size();
    extended_contour[c].hole= hole;
//	extended_contour[c].hole = hole;
//    extended_contour[c].size()= new_contour->size();
        std::cout << "extended_contour[c].init(new_contour->size())" << c << " " << new_contour->size() << " " << p->contour.size() << std::endl;
	//extended_contour[c]->init(new_contour->size()); //vertex = (gpc_vertex*) malloc(new_contour->size()
        std::cout << "done" << std::endl;
            //* sizeof(gpc_vertex));
    for (v= 0; v < new_contour->size(); v++)
        extended_contour[c][v]= new_contour->at(v);

    /* Dispose of the old contour */
    //FREE(p->contour);
    //FREE(p->hole);

    /* Update the polygon information */
    p->contour.push_back(extended_contour);
    //p->hole= extended_hole;
    //p->hole = hole;
    //p->contour.init(2); //extended_contour);
}

void gpc_cut_polygon(gpc_polygon *poly,gpc_polygon *result)
{
    int i,j,k,last;
    gpc_init_polygon(result);
    std::cout << "RESULT " << result->contour.size() << std::endl;
    for (i=0;i< (int) poly->contour.size();i++)
    {
        last = 0;
        for (j=0;j< (int) poly->contour[i].size();j++) {
            for (k=0;k<j;k++) {
                std::cout << "gpc_vertex_dist " << gpc_vertex_dist(&(poly->contour[i][j]),
                                &(poly->contour[i][k])) << std::endl;
                                
                if(
                    EQ(gpc_vertex_dist(&(poly->contour[i][j]),
                                       &(poly->contour[i][k]))
                        ,1) ) {
                    
                    gpc_vertex_list l; // = new gpc_vertex_list();
                    l.hole = poly->contour[i].hole;
                    
                    std::cout << "EQ " << poly->contour[i][j].x << " " <<
                        poly->contour[i][j].y << " " <<
                        poly->contour[i][k].x << " " <<
                        poly->contour[i][k].y << std::endl;
                    /*
                    gpc_vertex_list l;
                        l.num_vertices = j-last;
                        l.vertex = &(poly->contour[i].vertex[last]);
                        gpc_add_contour(result,&l,poly->hole[i]);
    
                        last = j;
                    */
                    
                    int size = j - last;
                        
                    for(int w = last; w < size; w++) {
                        l.push_back(poly->contour[i][w]);
                    }
                                            
                    // gpc_add_contour(result,l,poly->contour[i].hole);
                    result->contour.push_back(l);
                    
                    last = j;
                }
            }
        }
        if (last < (int) poly->contour[i].size() - 1)
        {
            gpc_vertex_list l; // *l = new gpc_vertex_list();
            l.hole = poly->contour[i].hole;
            //(gpc_vertex_list*) malloc(
            //    (poly->contour.size() + 1) * sizeof(gpc_vertex_list));
            int size = j - last;
            //int i = 3;
            for(int w = last; w < size; w++) {
                l.push_back(poly->contour[i][w]);
            }
            // l.vertex = &(poly->contour[i][last]);
            // gpc_add_contour(result,l,poly->contour[i].hole);
            result->contour.push_back(l);
        }
    }
    std::cout << "RESULT " << result->contour.size() << std::endl;
}


//#define DEBUGCLIP
void gpc_polygon_clip(gpc_op op, gpc_polygon *subj, gpc_polygon *clip,
        gpc_polygon *result)
{
    sb_tree       *sbtree= NULL;
    it_node       *it= NULL, *intersect;
    edge_node     *edge, *prev_edge, *next_edge, *succ_edge, *e0, *e1;
    edge_node     *aet= NULL, *c_heap= NULL, *s_heap= NULL;
    lmt_node      *lmt= NULL, *local_min;
    polygon_node  *out_poly= NULL, *p, *q, *poly, *npoly, *cf= NULL;
    vertex_node   *vtx, *nv;
    h_state        horiz[2];
    int            in[2], exists[2], parity[2]= {LEFT, LEFT};
    int            c, v, contributing=0, search, scanbeam= 0, sbt_entries= 0;
    int            vclass, bl=0, br=0, tl=0, tr=0;
    double        *sbt= NULL, xb=0, px=0, yb=0, yt=0, dy=0, ix=0, iy;
    gpc_polygon   clean;


#ifdef DEBUGCLIP
    printf("Operation : %d\n",op);
    printf("Polygon subj\n");gpc_write_polygon(stdout,1,subj);
    printf("Polygon clip\n");gpc_write_polygon(stdout,1,clip);
    fflush(stdout);
#endif

    callNumber += 1;

    /* Test for trivial NULL result cases */
    if (((subj->contour.size() == 0) && (clip->contour.size() == 0))
            || ((subj->contour.size() == 0) && ((op == GPC_INT) || (op == GPC_DIFF)))
            || ((clip->contour.size() == 0) &&  (op == GPC_INT)))
    {
        result->contour.init(0);//= NULL;
#ifdef DEBUGCLIP
        printf("Result trivial\n");
#endif
        return;
    }

    /* Identify potentialy contributing contours */
    if (((op == GPC_INT) || (op == GPC_DIFF))
            && (subj->contour.size() > 0) && (clip->contour.size() > 0))
        minimax_test(subj, clip, op);

    /* Build LMT */
    if (subj->contour.size() > 0)
        s_heap= build_lmt(&lmt, &sbtree, &sbt_entries, subj, SUBJ, op);
    if (clip->contour.size() > 0)
        c_heap= build_lmt(&lmt, &sbtree, &sbt_entries, clip, CLIP, op);

    /* Return a NULL result if no contours contribute */
    if (lmt == NULL)
    {
        result->contour.init(0);//= NULL;
        reset_lmt(&lmt);
        FREE(s_heap);
        FREE(c_heap);
#ifdef DEBUGCLIP
        printf("Result trivial\n");
#endif

        return;
    }

    /* Build scanbeam table from scanbeam tree */
    sbt = (double*) malloc(sbt_entries * sizeof(double));
    build_sbt(&scanbeam, sbt, sbtree);
    scanbeam= 0;
    free_sbtree(&sbtree);

    /* Allow pointer re-use without causing memory leak */
    if (subj == result)
        gpc_free_polygon(subj);
    if (clip == result)
        gpc_free_polygon(clip);

    /* Invert clip polygon for difference operation */
    if (op == GPC_DIFF)
        parity[CLIP]= RIGHT;

    local_min= lmt;

    /* Process each scanbeam */
    while (scanbeam < sbt_entries)
    {
        /* Set yb and yt to the bottom and top of the scanbeam */
        yb= sbt[scanbeam++];
        if (scanbeam < sbt_entries)
        {
            yt= sbt[scanbeam];
            dy= yt - yb;
        }

        /* === SCANBEAM BOUNDARY PROCESSING ================================ */

        /* If LMT node corresponding to yb exists */
        if (local_min)
        {
            if (local_min->y == yb)
            {
                /* Add edges starting at this local minimum to the AET */
                for (edge= local_min->first_bound; edge; edge= edge->next_bound)
                    add_edge_to_aet(&aet, edge, NULL);

                local_min= local_min->next;
            }
        }

        /* Set dummy previous x value */
        px= -DBL_MAX;

        /* Create bundles within AET */
        e0= aet;
        e1= aet;

        /* Set up bundle fields of first edge */
        aet->bundle[ABOVE][ aet->type]= (aet->top.y != yb);
        aet->bundle[ABOVE][!aet->type]= FALSE;
        aet->bstate[ABOVE]= UNBUNDLED;

        for (next_edge= aet->next; next_edge; next_edge= next_edge->next)
        {
            /* Set up bundle fields of next edge */
            next_edge->bundle[ABOVE][ next_edge->type]= (next_edge->top.y != yb);
            next_edge->bundle[ABOVE][!next_edge->type]= FALSE;
            next_edge->bstate[ABOVE]= UNBUNDLED;

            /* Bundle edges above the scanbeam boundary if they coincide */
            if (next_edge->bundle[ABOVE][next_edge->type])
            {
                if (EQ(e0->xb, next_edge->xb) && EQ(e0->dx, next_edge->dx)
                        && (e0->top.y != yb))
                {
                    next_edge->bundle[ABOVE][ next_edge->type]^= 
                        e0->bundle[ABOVE][ next_edge->type];
                    next_edge->bundle[ABOVE][!next_edge->type]= 
                        e0->bundle[ABOVE][!next_edge->type];
                    next_edge->bstate[ABOVE]= BUNDLE_HEAD;
                    e0->bundle[ABOVE][CLIP]= FALSE;
                    e0->bundle[ABOVE][SUBJ]= FALSE;
                    e0->bstate[ABOVE]= BUNDLE_TAIL;
                }
                e0= next_edge;
            }
        }

        horiz[CLIP]= NH;
        horiz[SUBJ]= NH;

        /* Process each edge at this scanbeam boundary */
        for (edge= aet; edge; edge= edge->next)
        {
            exists[CLIP]= edge->bundle[ABOVE][CLIP] + 
                (edge->bundle[BELOW][CLIP] << 1);
            exists[SUBJ]= edge->bundle[ABOVE][SUBJ] + 
                (edge->bundle[BELOW][SUBJ] << 1);

            if (exists[CLIP] || exists[SUBJ])
            {
                /* Set bundle side */
                edge->bside[CLIP]= parity[CLIP];
                edge->bside[SUBJ]= parity[SUBJ];

                /* Determine contributing status and quadrant occupancies */
                switch (op)
                {
                    case GPC_DIFF:
                    case GPC_INT:
                        contributing= (exists[CLIP] && (parity[SUBJ] || horiz[SUBJ]))
                            || (exists[SUBJ] && (parity[CLIP] || horiz[CLIP]))
                            || (exists[CLIP] && exists[SUBJ]
                                    && (parity[CLIP] == parity[SUBJ]));
                        br= (parity[CLIP])
                            && (parity[SUBJ]);
                        bl= (parity[CLIP] ^ edge->bundle[ABOVE][CLIP])
                            && (parity[SUBJ] ^ edge->bundle[ABOVE][SUBJ]);
                        tr= (parity[CLIP] ^ (horiz[CLIP]!=NH))
                            && (parity[SUBJ] ^ (horiz[SUBJ]!=NH));
                        tl= (parity[CLIP] ^ (horiz[CLIP]!=NH) ^ edge->bundle[BELOW][CLIP]) 
                            && (parity[SUBJ] ^ (horiz[SUBJ]!=NH) ^ edge->bundle[BELOW][SUBJ]);
                        break;
                    case GPC_XOR:
                        contributing= exists[CLIP] || exists[SUBJ];
                        br= (parity[CLIP])
                            ^ (parity[SUBJ]);
                        bl= (parity[CLIP] ^ edge->bundle[ABOVE][CLIP])
                            ^ (parity[SUBJ] ^ edge->bundle[ABOVE][SUBJ]);
                        tr= (parity[CLIP] ^ (horiz[CLIP]!=NH))
                            ^ (parity[SUBJ] ^ (horiz[SUBJ]!=NH));
                        tl= (parity[CLIP] ^ (horiz[CLIP]!=NH) ^ edge->bundle[BELOW][CLIP]) 
                            ^ (parity[SUBJ] ^ (horiz[SUBJ]!=NH) ^ edge->bundle[BELOW][SUBJ]);
                        break;
                    case GPC_UNION:
                        contributing= (exists[CLIP] && (!parity[SUBJ] || horiz[SUBJ]))
                            || (exists[SUBJ] && (!parity[CLIP] || horiz[CLIP]))
                            || (exists[CLIP] && exists[SUBJ]
                                    && (parity[CLIP] == parity[SUBJ]));
                        br= (parity[CLIP])
                            || (parity[SUBJ]);
                        bl= (parity[CLIP] ^ edge->bundle[ABOVE][CLIP])
                            || (parity[SUBJ] ^ edge->bundle[ABOVE][SUBJ]);
                        tr= (parity[CLIP] ^ (horiz[CLIP]!=NH))
                            || (parity[SUBJ] ^ (horiz[SUBJ]!=NH));
                        tl= (parity[CLIP] ^ (horiz[CLIP]!=NH) ^ edge->bundle[BELOW][CLIP]) 
                            || (parity[SUBJ] ^ (horiz[SUBJ]!=NH) ^ edge->bundle[BELOW][SUBJ]);
                        break;
                }

                /* Update parity */
                parity[CLIP]^= edge->bundle[ABOVE][CLIP];
                parity[SUBJ]^= edge->bundle[ABOVE][SUBJ];

                /* Update horizontal state */
                if (exists[CLIP])         
                    horiz[CLIP]=
                        next_h_state[horiz[CLIP]]
                        [((exists[CLIP] - 1) << 1) + parity[CLIP]];
                if (exists[SUBJ])         
                    horiz[SUBJ]=
                        next_h_state[horiz[SUBJ]]
                        [((exists[SUBJ] - 1) << 1) + parity[SUBJ]];

                vclass= tr + (tl << 1) + (br << 2) + (bl << 3);

                if (contributing)
                {
                    xb= edge->xb;

#ifdef DEBUGCLIP
                    printf("Vclass %d\n",vclass);
#endif
                    switch (vclass)
                    {
                        case EMN:
                        case IMN:
                            add_local_min(&out_poly, edge, xb, yb);
                            px= xb;
                            cf= edge->outp[ABOVE];
                            break;
                        case ERI:
                            if (xb != px)
                            {
                                add_right(cf, xb, yb);
                                px= xb;
                            }
                            edge->outp[ABOVE]= cf;
                            cf= NULL;
                            break;
                        case ELI:
                            add_left(edge->outp[BELOW], xb, yb);
                            px= xb;
                            cf= edge->outp[BELOW];
                            break;
                        case EMX:
                            if (xb != px)
                            {
                                add_left(cf, xb, yb);
                                px= xb;
                            }
                            
                            
                            merge_right(cf, edge->outp[BELOW], out_poly);
                            cf= NULL;
                            break;
                        case ILI:
                            if (xb != px)
                            {
                                add_left(cf, xb, yb);
                                px= xb;
                            }
                            edge->outp[ABOVE]= cf;
                            cf= NULL;
                            break;
                        case IRI:
                            add_right(edge->outp[BELOW], xb, yb);
                            px= xb;
                            cf= edge->outp[BELOW];
                            edge->outp[BELOW]= NULL;
                            break;
                        case IMX:
                            if (xb != px)
                            {
                                add_right(cf, xb, yb);
                                px= xb;
                            }
                            merge_left(cf, edge->outp[BELOW], out_poly);
                            cf= NULL;
                            edge->outp[BELOW]= NULL;
                            break;
                        case IMM:
                            if (xb != px)
                            {
                                add_right(cf, xb, yb);
                                px= xb;
                            }
                            merge_left(cf, edge->outp[BELOW], out_poly);
                            edge->outp[BELOW]= NULL;
                            add_local_min(&out_poly, edge, xb, yb);
                            cf= edge->outp[ABOVE];
                            break;
                        case EMM:
                            if (xb != px)
                            {
                                add_left(cf, xb, yb);
                                px= xb;
                            }
                            merge_right(cf, edge->outp[BELOW], out_poly);
                            edge->outp[BELOW]= NULL;
                            add_local_min(&out_poly, edge, xb, yb);
                            cf= edge->outp[ABOVE];
                            break;
                        case LED:
                            if (edge->bot.y == yb)
                                add_left(edge->outp[BELOW], xb, yb);
                            edge->outp[ABOVE]= edge->outp[BELOW];
                            px= xb;
                            break;
                        case RED:
                            if (edge->bot.y == yb)
                                add_right(edge->outp[BELOW], xb, yb);
                            edge->outp[ABOVE]= edge->outp[BELOW];
                            px= xb;
                            break;
                        default:
                            break;
                    } /* End of switch */
                } /* End of contributing conditional */
            } /* End of edge exists conditional */
        } /* End of AET loop */

        /* Delete terminating edges from the AET, otherwise compute xt */
        for (edge= aet; edge; edge= edge->next)
        {
            if (edge->top.y == yb)
            {
                prev_edge= edge->prev;
                next_edge= edge->next;
                if (prev_edge)
                    prev_edge->next= next_edge;
                else
                    aet= next_edge;
                if (next_edge)
                    next_edge->prev= prev_edge;

                /* Copy bundle head state to the adjacent tail edge if required */
                if ((edge->bstate[BELOW] == BUNDLE_HEAD) && prev_edge)
                {
                    if (prev_edge->bstate[BELOW] == BUNDLE_TAIL)
                    {
                        prev_edge->outp[BELOW]= edge->outp[BELOW];
                        prev_edge->bstate[BELOW]= UNBUNDLED;
                        if (prev_edge->prev)
                            if (prev_edge->prev->bstate[BELOW] == BUNDLE_TAIL)
                                prev_edge->bstate[BELOW]= BUNDLE_HEAD;
                    }
                }
            }
            else
            {
                if (edge->top.y == yt)
                    edge->xt= edge->top.x;
                else
                    edge->xt= edge->bot.x + edge->dx * (yt - edge->bot.y);
            }
        }

        if (scanbeam < sbt_entries)
        {
            /* === SCANBEAM INTERIOR PROCESSING ============================== */

            build_intersection_table(&it, aet, dy);

            /* Process each node in the intersection table */
            for (intersect= it; intersect; intersect= intersect->next)
            {
                e0= intersect->ie[0];
                e1= intersect->ie[1];

                /* Only generate output for contributing intersections */
                if ((e0->bundle[ABOVE][CLIP] || e0->bundle[ABOVE][SUBJ])
                        && (e1->bundle[ABOVE][CLIP] || e1->bundle[ABOVE][SUBJ]))
                {
                    p= e0->outp[ABOVE];
                    q= e1->outp[ABOVE];
                    ix= intersect->point.x;
                    iy= intersect->point.y + yb;

                    in[CLIP]= ( e0->bundle[ABOVE][CLIP] && !e0->bside[CLIP])
                        || ( e1->bundle[ABOVE][CLIP] &&  e1->bside[CLIP])
                        || (!e0->bundle[ABOVE][CLIP] && !e1->bundle[ABOVE][CLIP]
                                && e0->bside[CLIP] && e1->bside[CLIP]);
                    in[SUBJ]= ( e0->bundle[ABOVE][SUBJ] && !e0->bside[SUBJ])
                        || ( e1->bundle[ABOVE][SUBJ] &&  e1->bside[SUBJ])
                        || (!e0->bundle[ABOVE][SUBJ] && !e1->bundle[ABOVE][SUBJ]
                                && e0->bside[SUBJ] && e1->bside[SUBJ]);

                    /* Determine quadrant occupancies */
                    switch (op)
                    {
                        case GPC_DIFF:
                        case GPC_INT:
                            tr= (in[CLIP])
                                && (in[SUBJ]);
                            tl= (in[CLIP] ^ e1->bundle[ABOVE][CLIP])
                                && (in[SUBJ] ^ e1->bundle[ABOVE][SUBJ]);
                            br= (in[CLIP] ^ e0->bundle[ABOVE][CLIP])
                                && (in[SUBJ] ^ e0->bundle[ABOVE][SUBJ]);
                            bl= (in[CLIP] ^ e1->bundle[ABOVE][CLIP] ^ e0->bundle[ABOVE][CLIP])
                                && (in[SUBJ] ^ e1->bundle[ABOVE][SUBJ] ^ e0->bundle[ABOVE][SUBJ]);
                            break;
                        case GPC_XOR:
                            tr= (in[CLIP])
                                ^ (in[SUBJ]);
                            tl= (in[CLIP] ^ e1->bundle[ABOVE][CLIP])
                                ^ (in[SUBJ] ^ e1->bundle[ABOVE][SUBJ]);
                            br= (in[CLIP] ^ e0->bundle[ABOVE][CLIP])
                                ^ (in[SUBJ] ^ e0->bundle[ABOVE][SUBJ]);
                            bl= (in[CLIP] ^ e1->bundle[ABOVE][CLIP] ^ e0->bundle[ABOVE][CLIP])
                                ^ (in[SUBJ] ^ e1->bundle[ABOVE][SUBJ] ^ e0->bundle[ABOVE][SUBJ]);
                            break;
                        case GPC_UNION:
                            tr= (in[CLIP])
                                || (in[SUBJ]);
                            tl= (in[CLIP] ^ e1->bundle[ABOVE][CLIP])
                                || (in[SUBJ] ^ e1->bundle[ABOVE][SUBJ]);
                            br= (in[CLIP] ^ e0->bundle[ABOVE][CLIP])
                                || (in[SUBJ] ^ e0->bundle[ABOVE][SUBJ]);
                            bl= (in[CLIP] ^ e1->bundle[ABOVE][CLIP] ^ e0->bundle[ABOVE][CLIP])
                                || (in[SUBJ] ^ e1->bundle[ABOVE][SUBJ] ^ e0->bundle[ABOVE][SUBJ]);
                            break;
                    }

                    vclass= tr + (tl << 1) + (br << 2) + (bl << 3);

                    switch (vclass)
                    {
                        case EMN:
                            add_local_min(&out_poly, e0, ix, iy);
                            e1->outp[ABOVE]= e0->outp[ABOVE];
                            break;
                        case ERI:
                            if (p)
                            {
                                add_right(p, ix, iy);
                                e1->outp[ABOVE]= p;
                                e0->outp[ABOVE]= NULL;
                            }
                            break;
                        case ELI:
                            if (q)
                            {
                                add_left(q, ix, iy);
                                e0->outp[ABOVE]= q;
                                e1->outp[ABOVE]= NULL;
                            }
                            break;
                        case EMX:
                            if (p && q)
                            {
                                add_left(p, ix, iy);
                                merge_right(p, q, out_poly);
                                e0->outp[ABOVE]= NULL;
                                e1->outp[ABOVE]= NULL;
                            }
                            break;
                        case IMN:
                            add_local_min(&out_poly, e0, ix, iy);
                            e1->outp[ABOVE]= e0->outp[ABOVE];
                            break;
                        case ILI:
                            if (p)
                            {
                                add_left(p, ix, iy);
                                e1->outp[ABOVE]= p;
                                e0->outp[ABOVE]= NULL;
                            }
                            break;
                        case IRI:
                            if (q)
                            {
                                add_right(q, ix, iy);
                                e0->outp[ABOVE]= q;
                                e1->outp[ABOVE]= NULL;
                            }
                            break;
                        case IMX:
                            if (p && q)
                            {
                                add_right(p, ix, iy);
                                merge_left(p, q, out_poly);
                                e0->outp[ABOVE]= NULL;
                                e1->outp[ABOVE]= NULL;
                            }
                            break;
                        case IMM:
                            if (p && q)
                            {
                                add_right(p, ix, iy);
                                merge_left(p, q, out_poly);
                                add_local_min(&out_poly, e0, ix, iy);
                                e1->outp[ABOVE]= e0->outp[ABOVE];
                            }
                            break;
                        case EMM:
                            if (p && q)
                            {
                                add_left(p, ix, iy);
                                merge_right(p, q, out_poly);
                                add_local_min(&out_poly, e0, ix, iy);
                                e1->outp[ABOVE]= e0->outp[ABOVE];
                            }
                            break;
                        default:
                            break;
                    } /* End of switch */
                } /* End of contributing intersection conditional */

                /* Swap bundle sides in response to edge crossing */
                if (e0->bundle[ABOVE][CLIP])
                    e1->bside[CLIP]= !e1->bside[CLIP];
                if (e1->bundle[ABOVE][CLIP])
                    e0->bside[CLIP]= !e0->bside[CLIP];
                if (e0->bundle[ABOVE][SUBJ])
                    e1->bside[SUBJ]= !e1->bside[SUBJ];
                if (e1->bundle[ABOVE][SUBJ])
                    e0->bside[SUBJ]= !e0->bside[SUBJ];

                /* Swap e0 and e1 bundles in the AET */
                prev_edge= e0->prev;
                next_edge= e1->next;
                if (next_edge)
                    next_edge->prev= e0;

                if (e0->bstate[ABOVE] == BUNDLE_HEAD)
                {
                    search= TRUE;
                    while (search)
                    {
                        prev_edge= prev_edge->prev;
                        if (prev_edge)
                        {
                            if (prev_edge->bstate[ABOVE] != BUNDLE_TAIL)
                                search= FALSE;
                        }
                        else
                            search= FALSE;
                    }
                }
                if (!prev_edge)
                {
                    aet->prev= e1;
                    e1->next= aet;
                    aet= e0->next;
                }
                else
                {
                    prev_edge->next->prev= e1;
                    e1->next= prev_edge->next;
                    prev_edge->next= e0->next;
                }
                if (e0->next != NULL)
                {
                    e0->next->prev= prev_edge;
                    e0->next= next_edge;
                }
                if (e1->next != NULL)
                {
                    e1->next->prev= e1;
                }
            } /* End of IT loop*/

            /* Prepare for next scanbeam */
            for (edge= aet; edge; edge= next_edge)
            {
                next_edge= edge->next;
                succ_edge= edge->succ;

                if ((edge->top.y == yt) && succ_edge)
                {
                    /* Replace AET edge by its successor */
                    succ_edge->outp[BELOW]= edge->outp[ABOVE];
                    succ_edge->bstate[BELOW]= edge->bstate[ABOVE];
                    succ_edge->bundle[BELOW][CLIP]= edge->bundle[ABOVE][CLIP];
                    succ_edge->bundle[BELOW][SUBJ]= edge->bundle[ABOVE][SUBJ];
                    prev_edge= edge->prev;
                    if (prev_edge)
                        prev_edge->next= succ_edge;
                    else
                        aet= succ_edge;
                    if (next_edge)
                        next_edge->prev= succ_edge;
                    succ_edge->prev= prev_edge;
                    succ_edge->next= next_edge;
                }
                else
                {
                    /* Update this edge */
                    edge->outp[BELOW]= edge->outp[ABOVE];
                    edge->bstate[BELOW]= edge->bstate[ABOVE];
                    edge->bundle[BELOW][CLIP]= edge->bundle[ABOVE][CLIP];
                    edge->bundle[BELOW][SUBJ]= edge->bundle[ABOVE][SUBJ];
                    edge->xb= edge->xt;
                }
                edge->outp[ABOVE]= NULL;
            }
        }
    } /* === END OF SCANBEAM PROCESSING ================================== */

    /* Generate result polygon from out_poly */
    gpc_init_polygon(&clean);
    clean.contour.init(0);//= NULL;
    //clean.hole= NULL;
    int num_contours= count_contours(out_poly);
	//clean.contour.num_contours = clean.num_contours;
    if (num_contours > 0)
    {
        //clean.hole = (int*) malloc(clean.num_contours
        //        * sizeof(int));
        clean.contour.init(num_contours); // = (gpc_vertex_list*) malloc(clean.num_contours
                //* sizeof(gpc_vertex_list));

        c= 0;
        for (poly= out_poly; poly; poly= npoly)
        {
            npoly= poly->next;
            if (poly->active)
            {
                clean.contour[c].hole= poly->proxy->hole;
				//clean.contour[c].size()= poly->active;
                clean.contour[c].init(poly->active);// = (gpc_vertex*) malloc(
                        //clean.contour[c].size() * sizeof(gpc_vertex));

                v= clean.contour[c].size() - 1;
                for (vtx= poly->proxy->v[LEFT]; vtx; vtx= nv)
                {
                    nv= vtx->next;
                    clean.contour[c][v].x= vtx->x;
                    clean.contour[c][v].y= vtx->y;
                    FREE(vtx);
                    v--;
                }
                c++;
            }
            // FREE(poly);
        }
    }

    gpc_filter_polygon(&clean);
    /*
       gpc_cut_polygon(&clean,result);
       gpc_free_polygon(&clean);
     */
    *result = clean;

#ifdef DEBUGCLIP
    printf("Result result : \n");
    gpc_write_polygon(stdout,1,result);
    fflush(stdout);
#endif
    gc_free();
    /* Tidy up */
    reset_it(&it);
    reset_lmt(&lmt);
    FREE(c_heap);
    FREE(s_heap);
    FREE(sbt);
}


void gpc_free_tristrip(gpc_tristrip *t)
{

//    for (s= 0; s < t->num_strips; s++)
//        FREE(t->strip[s].vertex);
    FREE(t->strip);
    t->num_strips= 0;
}


void gpc_polygon_to_tristrip(gpc_polygon *s, gpc_tristrip *t)
{
    gpc_polygon c;

    c.contour.init(0); //= NULL;
    gpc_tristrip_clip(GPC_DIFF, s, &c, t);
}


void gpc_tristrip_clip(gpc_op op, gpc_polygon *subj, gpc_polygon *clip,
        gpc_tristrip *result)
{
    sb_tree       *sbtree= NULL;
    it_node       *it= NULL, *intersect;
    edge_node     *edge, *prev_edge, *next_edge, *succ_edge, *e0, *e1;
    edge_node     *aet= NULL, *c_heap= NULL, *s_heap= NULL, *cf=NULL;
    lmt_node      *lmt= NULL, *local_min;
    polygon_node  *tlist= NULL, *tn, *tnn, *p, *q;
    vertex_node   *lt, *ltn, *rt, *rtn;
    h_state        horiz[2];
    vertex_type    cft=NUL;
    int            in[2], exists[2], parity[2]= {LEFT, LEFT};
    int            s, v, contributing=0, search, scanbeam= 0, sbt_entries= 0;
    int            vclass, bl=0, br=0, tl=0, tr=0;
    double        *sbt= NULL, xb=0, px=0, nx=0, yb=0, yt=0, dy=0, ix=0, iy;

    /* Test for trivial NULL result cases */
    if (((subj->contour.size() == 0) && (clip->contour.size() == 0))
            || ((subj->contour.size() == 0) && ((op == GPC_INT) || (op == GPC_DIFF)))
            || ((clip->contour.size() == 0) &&  (op == GPC_INT)))
    {
        result->num_strips= 0;
        result->strip= NULL;
        return;
    }

    /* Identify potentialy contributing contours */
    if (((op == GPC_INT) || (op == GPC_DIFF))
            && (subj->contour.size() > 0) && (clip->contour.size() > 0))
        minimax_test(subj, clip, op);

    /* Build LMT */
    if (subj->contour.size() > 0)
        s_heap= build_lmt(&lmt, &sbtree, &sbt_entries, subj, SUBJ, op);
    if (clip->contour.size() > 0)
        c_heap= build_lmt(&lmt, &sbtree, &sbt_entries, clip, CLIP, op);

    /* Return a NULL result if no contours contribute */
    if (lmt == NULL)
    {
        result->num_strips= 0;
        result->strip= NULL;
        reset_lmt(&lmt);
        FREE(s_heap);
        FREE(c_heap);
        return;
    }

    /* Build scanbeam table from scanbeam tree */
    sbt = (double*) malloc(sbt_entries * sizeof(double));
    build_sbt(&scanbeam, sbt, sbtree);
    scanbeam= 0;
    free_sbtree(&sbtree);

    /* Invert clip polygon for difference operation */
    if (op == GPC_DIFF)
        parity[CLIP]= RIGHT;

    local_min= lmt;

    /* Process each scanbeam */
    while (scanbeam < sbt_entries)
    {
        /* Set yb and yt to the bottom and top of the scanbeam */
        yb= sbt[scanbeam++];
        if (scanbeam < sbt_entries)
        {
            yt= sbt[scanbeam];
            dy= yt - yb;
        }

        /* === SCANBEAM BOUNDARY PROCESSING ================================ */

        /* If LMT node corresponding to yb exists */
        if (local_min)
        {
            if (local_min->y == yb)
            {
                /* Add edges starting at this local minimum to the AET */
                for (edge= local_min->first_bound; edge; edge= edge->next_bound)
                    add_edge_to_aet(&aet, edge, NULL);

                local_min= local_min->next;
            }
        }

        /* Set dummy previous x value */
        px= -DBL_MAX;

        /* Create bundles within AET */
        e0= aet;
        e1= aet;

        /* Set up bundle fields of first edge */
        aet->bundle[ABOVE][ aet->type]= (aet->top.y != yb);
        aet->bundle[ABOVE][!aet->type]= FALSE;
        aet->bstate[ABOVE]= UNBUNDLED;

        for (next_edge= aet->next; next_edge; next_edge= next_edge->next)
        {
            /* Set up bundle fields of next edge */
            next_edge->bundle[ABOVE][ next_edge->type]= (next_edge->top.y != yb);
            next_edge->bundle[ABOVE][!next_edge->type]= FALSE;
            next_edge->bstate[ABOVE]= UNBUNDLED;

            /* Bundle edges above the scanbeam boundary if they coincide */
            if (next_edge->bundle[ABOVE][next_edge->type])
            {
                if (EQ(e0->xb, next_edge->xb) && EQ(e0->dx, next_edge->dx)
                        && (e0->top.y != yb))
                {
                    next_edge->bundle[ABOVE][ next_edge->type]^= 
                        e0->bundle[ABOVE][ next_edge->type];
                    next_edge->bundle[ABOVE][!next_edge->type]= 
                        e0->bundle[ABOVE][!next_edge->type]; 
                    next_edge->bstate[ABOVE]= BUNDLE_HEAD;
                    e0->bundle[ABOVE][CLIP]= FALSE;
                    e0->bundle[ABOVE][SUBJ]= FALSE;
                    e0->bstate[ABOVE]= BUNDLE_TAIL;
                }
                e0= next_edge;
            }
        }

        horiz[CLIP]= NH;
        horiz[SUBJ]= NH;

        /* Process each edge at this scanbeam boundary */
        for (edge= aet; edge; edge= edge->next)
        {
            exists[CLIP]= edge->bundle[ABOVE][CLIP] + 
                (edge->bundle[BELOW][CLIP] << 1);
            exists[SUBJ]= edge->bundle[ABOVE][SUBJ] + 
                (edge->bundle[BELOW][SUBJ] << 1);

            if (exists[CLIP] || exists[SUBJ])
            {
                /* Set bundle side */
                edge->bside[CLIP]= parity[CLIP];
                edge->bside[SUBJ]= parity[SUBJ];

                /* Determine contributing status and quadrant occupancies */
                switch (op)
                {
                    case GPC_DIFF:
                    case GPC_INT:
                        contributing= (exists[CLIP] && (parity[SUBJ] || horiz[SUBJ]))
                            || (exists[SUBJ] && (parity[CLIP] || horiz[CLIP]))
                            || (exists[CLIP] && exists[SUBJ]
                                    && (parity[CLIP] == parity[SUBJ]));
                        br= (parity[CLIP])
                            && (parity[SUBJ]);
                        bl= (parity[CLIP] ^ edge->bundle[ABOVE][CLIP])
                            && (parity[SUBJ] ^ edge->bundle[ABOVE][SUBJ]);
                        tr= (parity[CLIP] ^ (horiz[CLIP]!=NH))
                            && (parity[SUBJ] ^ (horiz[SUBJ]!=NH));
                        tl= (parity[CLIP] ^ (horiz[CLIP]!=NH) ^ edge->bundle[BELOW][CLIP]) 
                            && (parity[SUBJ] ^ (horiz[SUBJ]!=NH) ^ edge->bundle[BELOW][SUBJ]);
                        break;
                    case GPC_XOR:
                        contributing= exists[CLIP] || exists[SUBJ];
                        br= (parity[CLIP])
                            ^ (parity[SUBJ]);
                        bl= (parity[CLIP] ^ edge->bundle[ABOVE][CLIP])
                            ^ (parity[SUBJ] ^ edge->bundle[ABOVE][SUBJ]);
                        tr= (parity[CLIP] ^ (horiz[CLIP]!=NH))
                            ^ (parity[SUBJ] ^ (horiz[SUBJ]!=NH));
                        tl= (parity[CLIP] ^ (horiz[CLIP]!=NH) ^ edge->bundle[BELOW][CLIP])
                            ^ (parity[SUBJ] ^ (horiz[SUBJ]!=NH) ^ edge->bundle[BELOW][SUBJ]);
                        break;
                    case GPC_UNION:
                        contributing= (exists[CLIP] && (!parity[SUBJ] || horiz[SUBJ]))
                            || (exists[SUBJ] && (!parity[CLIP] || horiz[CLIP]))
                            || (exists[CLIP] && exists[SUBJ]
                                    && (parity[CLIP] == parity[SUBJ]));
                        br= (parity[CLIP])
                            || (parity[SUBJ]);
                        bl= (parity[CLIP] ^ edge->bundle[ABOVE][CLIP])
                            || (parity[SUBJ] ^ edge->bundle[ABOVE][SUBJ]);
                        tr= (parity[CLIP] ^ (horiz[CLIP]!=NH))
                            || (parity[SUBJ] ^ (horiz[SUBJ]!=NH));
                        tl= (parity[CLIP] ^ (horiz[CLIP]!=NH) ^ edge->bundle[BELOW][CLIP])
                            || (parity[SUBJ] ^ (horiz[SUBJ]!=NH) ^ edge->bundle[BELOW][SUBJ]);
                        break;
                }

                /* Update parity */
                parity[CLIP]^= edge->bundle[ABOVE][CLIP];
                parity[SUBJ]^= edge->bundle[ABOVE][SUBJ];

                /* Update horizontal state */
                if (exists[CLIP])         
                    horiz[CLIP]=
                        next_h_state[horiz[CLIP]]
                        [((exists[CLIP] - 1) << 1) + parity[CLIP]];
                if (exists[SUBJ])         
                    horiz[SUBJ]=
                        next_h_state[horiz[SUBJ]]
                        [((exists[SUBJ] - 1) << 1) + parity[SUBJ]];

                vclass= tr + (tl << 1) + (br << 2) + (bl << 3);

                if (contributing)
                {
                    xb= edge->xb;

                    switch (vclass)
                    {
                        case EMN:
                            new_tristrip(&tlist, edge, xb, yb);
                            cf= edge;
                            break;
                        case ERI:
                            edge->outp[ABOVE]= cf->outp[ABOVE];
                            if (xb != cf->xb)
                                VERTEX(edge, ABOVE, RIGHT, xb, yb);
                            cf= NULL;
                            break;
                        case ELI:
                            VERTEX(edge, BELOW, LEFT, xb, yb);
                            edge->outp[ABOVE]= NULL;
                            cf= edge;
                            break;
                        case EMX:
                            if (xb != cf->xb)
                                VERTEX(edge, BELOW, RIGHT, xb, yb);
                            edge->outp[ABOVE]= NULL;
                            cf= NULL;
                            break;
                        case IMN:
                            if (cft == LED)
                            {
                                if (cf->bot.y != yb)
                                    VERTEX(cf, BELOW, LEFT, cf->xb, yb);
                                new_tristrip(&tlist, cf, cf->xb, yb);
                            }
                            edge->outp[ABOVE]= cf->outp[ABOVE];
                            VERTEX(edge, ABOVE, RIGHT, xb, yb);
                            break;
                        case ILI:
                            new_tristrip(&tlist, edge, xb, yb);
                            cf= edge;
                            cft= ILI;
                            break;
                        case IRI:
                            if (cft == LED)
                            {
                                if (cf->bot.y != yb)
                                    VERTEX(cf, BELOW, LEFT, cf->xb, yb);
                                new_tristrip(&tlist, cf, cf->xb, yb);
                            }
                            VERTEX(edge, BELOW, RIGHT, xb, yb);
                            edge->outp[ABOVE]= NULL;
                            break;
                        case IMX:
                            VERTEX(edge, BELOW, LEFT, xb, yb);
                            edge->outp[ABOVE]= NULL;
                            cft= IMX;
                            break;
                        case IMM:
                            VERTEX(edge, BELOW, LEFT, xb, yb);
                            edge->outp[ABOVE]= cf->outp[ABOVE];
                            if (xb != cf->xb)
                                VERTEX(cf, ABOVE, RIGHT, xb, yb);
                            cf= edge;
                            break;
                        case EMM:
                            VERTEX(edge, BELOW, RIGHT, xb, yb);
                            edge->outp[ABOVE]= NULL;
                            new_tristrip(&tlist, edge, xb, yb);
                            cf= edge;
                            break;
                        case LED:
                            if (edge->bot.y == yb)
                                VERTEX(edge, BELOW, LEFT, xb, yb);
                            edge->outp[ABOVE]= edge->outp[BELOW];
                            cf= edge;
                            cft= LED;
                            break;
                        case RED:
                            edge->outp[ABOVE]= cf->outp[ABOVE];
                            if (cft == LED)
                            {
                                if (cf->bot.y == yb)
                                {
                                    VERTEX(edge, BELOW, RIGHT, xb, yb);
                                }
                                else
                                {
                                    if (edge->bot.y == yb)
                                    {
                                        VERTEX(cf, BELOW, LEFT, cf->xb, yb);
                                        VERTEX(edge, BELOW, RIGHT, xb, yb);
                                    }
                                }
                            }
                            else
                            {
                                VERTEX(edge, BELOW, RIGHT, xb, yb);
                                VERTEX(edge, ABOVE, RIGHT, xb, yb);
                            }
                            cf= NULL;
                            break;
                        default:
                            break;
                    } /* End of switch */
                } /* End of contributing conditional */
            } /* End of edge exists conditional */
        } /* End of AET loop */

        /* Delete terminating edges from the AET, otherwise compute xt */
        for (edge= aet; edge; edge= edge->next)
        {
            if (edge->top.y == yb)
            {
                prev_edge= edge->prev;
                next_edge= edge->next;
                if (prev_edge)
                    prev_edge->next= next_edge;
                else
                    aet= next_edge;
                if (next_edge)
                    next_edge->prev= prev_edge;

                /* Copy bundle head state to the adjacent tail edge if required */
                if ((edge->bstate[BELOW] == BUNDLE_HEAD) && prev_edge)
                {
                    if (prev_edge->bstate[BELOW] == BUNDLE_TAIL)
                    {
                        prev_edge->outp[BELOW]= edge->outp[BELOW];
                        prev_edge->bstate[BELOW]= UNBUNDLED;
                        if (prev_edge->prev)
                            if (prev_edge->prev->bstate[BELOW] == BUNDLE_TAIL)
                                prev_edge->bstate[BELOW]= BUNDLE_HEAD;
                    }
                }
            }
            else
            {
                if (edge->top.y == yt)
                    edge->xt= edge->top.x;
                else
                    edge->xt= edge->bot.x + edge->dx * (yt - edge->bot.y);
            }
        }

        if (scanbeam < sbt_entries)
        {
            /* === SCANBEAM INTERIOR PROCESSING ============================== */

            build_intersection_table(&it, aet, dy);

            /* Process each node in the intersection table */
            for (intersect= it; intersect; intersect= intersect->next)
            {
                e0= intersect->ie[0];
                e1= intersect->ie[1];

                /* Only generate output for contributing intersections */
                if ((e0->bundle[ABOVE][CLIP] || e0->bundle[ABOVE][SUBJ])
                        && (e1->bundle[ABOVE][CLIP] || e1->bundle[ABOVE][SUBJ]))
                {
                    p= e0->outp[ABOVE];
                    q= e1->outp[ABOVE];
                    ix= intersect->point.x;
                    iy= intersect->point.y + yb;

                    in[CLIP]= ( e0->bundle[ABOVE][CLIP] && !e0->bside[CLIP])
                        || ( e1->bundle[ABOVE][CLIP] &&  e1->bside[CLIP])
                        || (!e0->bundle[ABOVE][CLIP] && !e1->bundle[ABOVE][CLIP]
                                && e0->bside[CLIP] && e1->bside[CLIP]);
                    in[SUBJ]= ( e0->bundle[ABOVE][SUBJ] && !e0->bside[SUBJ])
                        || ( e1->bundle[ABOVE][SUBJ] &&  e1->bside[SUBJ])
                        || (!e0->bundle[ABOVE][SUBJ] && !e1->bundle[ABOVE][SUBJ]
                                && e0->bside[SUBJ] && e1->bside[SUBJ]);

                    /* Determine quadrant occupancies */
                    switch (op)
                    {
                        case GPC_DIFF:
                        case GPC_INT:
                            tr= (in[CLIP])
                                && (in[SUBJ]);
                            tl= (in[CLIP] ^ e1->bundle[ABOVE][CLIP])
                                && (in[SUBJ] ^ e1->bundle[ABOVE][SUBJ]);
                            br= (in[CLIP] ^ e0->bundle[ABOVE][CLIP])
                                && (in[SUBJ] ^ e0->bundle[ABOVE][SUBJ]);
                            bl= (in[CLIP] ^ e1->bundle[ABOVE][CLIP] ^ e0->bundle[ABOVE][CLIP])
                                && (in[SUBJ] ^ e1->bundle[ABOVE][SUBJ] ^ e0->bundle[ABOVE][SUBJ]);
                            break;
                        case GPC_XOR:
                            tr= (in[CLIP])
                                ^ (in[SUBJ]);
                            tl= (in[CLIP] ^ e1->bundle[ABOVE][CLIP])
                                ^ (in[SUBJ] ^ e1->bundle[ABOVE][SUBJ]);
                            br= (in[CLIP] ^ e0->bundle[ABOVE][CLIP])
                                ^ (in[SUBJ] ^ e0->bundle[ABOVE][SUBJ]);
                            bl= (in[CLIP] ^ e1->bundle[ABOVE][CLIP] ^ e0->bundle[ABOVE][CLIP])
                                ^ (in[SUBJ] ^ e1->bundle[ABOVE][SUBJ] ^ e0->bundle[ABOVE][SUBJ]);
                            break;
                        case GPC_UNION:
                            tr= (in[CLIP])
                                || (in[SUBJ]);
                            tl= (in[CLIP] ^ e1->bundle[ABOVE][CLIP])
                                || (in[SUBJ] ^ e1->bundle[ABOVE][SUBJ]);
                            br= (in[CLIP] ^ e0->bundle[ABOVE][CLIP])
                                || (in[SUBJ] ^ e0->bundle[ABOVE][SUBJ]);
                            bl= (in[CLIP] ^ e1->bundle[ABOVE][CLIP] ^ e0->bundle[ABOVE][CLIP])
                                || (in[SUBJ] ^ e1->bundle[ABOVE][SUBJ] ^ e0->bundle[ABOVE][SUBJ]);
                            break;
                    }

                    vclass= tr + (tl << 1) + (br << 2) + (bl << 3);

                    switch (vclass)
                    {
                        case EMN:
                            new_tristrip(&tlist, e1, ix, iy);
                            e0->outp[ABOVE]= e1->outp[ABOVE];
                            break;
                        case ERI:
                            if (p)
                            {
                                P_EDGE(prev_edge, e0, ABOVE, px, iy);
                                VERTEX(prev_edge, ABOVE, LEFT, px, iy);
                                VERTEX(e0, ABOVE, RIGHT, ix, iy);
                                e1->outp[ABOVE]= e0->outp[ABOVE];
                                e0->outp[ABOVE]= NULL;
                            }
                            break;
                        case ELI:
                            if (q)
                            {
                                N_EDGE(next_edge, e1, ABOVE, nx, iy);
                                VERTEX(e1, ABOVE, LEFT, ix, iy);
                                VERTEX(next_edge, ABOVE, RIGHT, nx, iy);
                                e0->outp[ABOVE]= e1->outp[ABOVE];
                                e1->outp[ABOVE]= NULL;
                            }
                            break;
                        case EMX:
                            if (p && q)
                            {
                                VERTEX(e0, ABOVE, LEFT, ix, iy);
                                e0->outp[ABOVE]= NULL;
                                e1->outp[ABOVE]= NULL;
                            }
                            break;
                        case IMN:
                            P_EDGE(prev_edge, e0, ABOVE, px, iy);
                            VERTEX(prev_edge, ABOVE, LEFT, px, iy);
                            N_EDGE(next_edge, e1, ABOVE, nx, iy);
                            VERTEX(next_edge, ABOVE, RIGHT, nx, iy);
                            new_tristrip(&tlist, prev_edge, px, iy); 
                            e1->outp[ABOVE]= prev_edge->outp[ABOVE];
                            VERTEX(e1, ABOVE, RIGHT, ix, iy);
                            new_tristrip(&tlist, e0, ix, iy);
                            next_edge->outp[ABOVE]= e0->outp[ABOVE];
                            VERTEX(next_edge, ABOVE, RIGHT, nx, iy);
                            break;
                        case ILI:
                            if (p)
                            {
                                VERTEX(e0, ABOVE, LEFT, ix, iy);
                                N_EDGE(next_edge, e1, ABOVE, nx, iy);
                                VERTEX(next_edge, ABOVE, RIGHT, nx, iy);
                                e1->outp[ABOVE]= e0->outp[ABOVE];
                                e0->outp[ABOVE]= NULL;
                            }
                            break;
                        case IRI:
                            if (q)
                            {
                                VERTEX(e1, ABOVE, RIGHT, ix, iy);
                                P_EDGE(prev_edge, e0, ABOVE, px, iy);
                                VERTEX(prev_edge, ABOVE, LEFT, px, iy);
                                e0->outp[ABOVE]= e1->outp[ABOVE];
                                e1->outp[ABOVE]= NULL;
                            }
                            break;
                        case IMX:
                            if (p && q)
                            {
                                VERTEX(e0, ABOVE, RIGHT, ix, iy);
                                VERTEX(e1, ABOVE, LEFT, ix, iy);
                                e0->outp[ABOVE]= NULL;
                                e1->outp[ABOVE]= NULL;
                                P_EDGE(prev_edge, e0, ABOVE, px, iy);
                                VERTEX(prev_edge, ABOVE, LEFT, px, iy);
                                new_tristrip(&tlist, prev_edge, px, iy);
                                N_EDGE(next_edge, e1, ABOVE, nx, iy);
                                VERTEX(next_edge, ABOVE, RIGHT, nx, iy);
                                next_edge->outp[ABOVE]= prev_edge->outp[ABOVE];
                                VERTEX(next_edge, ABOVE, RIGHT, nx, iy);
                            }
                            break;
                        case IMM:
                            if (p && q)
                            {
                                VERTEX(e0, ABOVE, RIGHT, ix, iy);
                                VERTEX(e1, ABOVE, LEFT, ix, iy);
                                P_EDGE(prev_edge, e0, ABOVE, px, iy);
                                VERTEX(prev_edge, ABOVE, LEFT, px, iy);
                                new_tristrip(&tlist, prev_edge, px, iy);
                                N_EDGE(next_edge, e1, ABOVE, nx, iy);
                                VERTEX(next_edge, ABOVE, RIGHT, nx, iy);
                                e1->outp[ABOVE]= prev_edge->outp[ABOVE];
                                VERTEX(e1, ABOVE, RIGHT, ix, iy);
                                new_tristrip(&tlist, e0, ix, iy);
                                next_edge->outp[ABOVE]= e0->outp[ABOVE];
                                VERTEX(next_edge, ABOVE, RIGHT, nx, iy);
                            }
                            break;
                        case EMM:
                            if (p && q)
                            {
                                VERTEX(e0, ABOVE, LEFT, ix, iy);
                                new_tristrip(&tlist, e1, ix, iy);
                                e0->outp[ABOVE]= e1->outp[ABOVE];
                            }
                            break;
                        default:
                            break;
                    } /* End of switch */
                } /* End of contributing intersection conditional */

                /* Swap bundle sides in response to edge crossing */
                if (e0->bundle[ABOVE][CLIP])
                    e1->bside[CLIP]= !e1->bside[CLIP];
                if (e1->bundle[ABOVE][CLIP])
                    e0->bside[CLIP]= !e0->bside[CLIP];
                if (e0->bundle[ABOVE][SUBJ])
                    e1->bside[SUBJ]= !e1->bside[SUBJ];
                if (e1->bundle[ABOVE][SUBJ])
                    e0->bside[SUBJ]= !e0->bside[SUBJ];

                /* Swap e0 and e1 bundles in the AET */
                prev_edge= e0->prev;
                next_edge= e1->next;
                if (e1->next)
                    e1->next->prev= e0;

                if (e0->bstate[ABOVE] == BUNDLE_HEAD)
                {
                    search= TRUE;
                    while (search)
                    {
                        prev_edge= prev_edge->prev;
                        if (prev_edge)
                        {
                            if (prev_edge->bundle[ABOVE][CLIP]
                                    || prev_edge->bundle[ABOVE][SUBJ]
                                    || (prev_edge->bstate[ABOVE] == BUNDLE_HEAD))
                                search= FALSE;
                        }
                        else
                            search= FALSE;
                    }
                }
                if (!prev_edge)
                {
                    e1->next= aet;
                    aet= e0->next;
                }
                else
                {
                    e1->next= prev_edge->next;
                    prev_edge->next= e0->next;
                }
                e0->next->prev= prev_edge;
                e1->next->prev= e1;
                e0->next= next_edge;
            } /* End of IT loop*/

            /* Prepare for next scanbeam */
            for (edge= aet; edge; edge= next_edge)
            {
                next_edge= edge->next;
                succ_edge= edge->succ;

                if ((edge->top.y == yt) && succ_edge)
                {
                    /* Replace AET edge by its successor */
                    succ_edge->outp[BELOW]= edge->outp[ABOVE];
                    succ_edge->bstate[BELOW]= edge->bstate[ABOVE];
                    succ_edge->bundle[BELOW][CLIP]= edge->bundle[ABOVE][CLIP];
                    succ_edge->bundle[BELOW][SUBJ]= edge->bundle[ABOVE][SUBJ];
                    prev_edge= edge->prev;
                    if (prev_edge)
                        prev_edge->next= succ_edge;
                    else
                        aet= succ_edge;
                    if (next_edge)
                        next_edge->prev= succ_edge;
                    succ_edge->prev= prev_edge;
                    succ_edge->next= next_edge;
                }
                else
                {
                    /* Update this edge */
                    edge->outp[BELOW]= edge->outp[ABOVE];
                    edge->bstate[BELOW]= edge->bstate[ABOVE];
                    edge->bundle[BELOW][CLIP]= edge->bundle[ABOVE][CLIP];
                    edge->bundle[BELOW][SUBJ]= edge->bundle[ABOVE][SUBJ];
                    edge->xb= edge->xt;
                }
                edge->outp[ABOVE]= NULL;
            }
        }
    } /* === END OF SCANBEAM PROCESSING ================================== */

    /* Generate result tristrip from tlist */
    result->strip= NULL;
    result->num_strips= count_tristrips(tlist);
    if (result->num_strips > 0)
    {
        result->strip = new gpc_vertex_list[result->num_strips];
        // (gpc_vertex_list*) malloc(result->num_strips * sizeof(gpc_vertex_list));

        s= 0;
        for (tn= tlist; tn; tn= tnn)
        {
            tnn= tn->next;

            if (tn->active > 2)
            {
                /* Valid tristrip: copy the vertices and free the heap */
//                result->strip[s].size()= tn->active;
				//result->strip[s].init(tn->active); // = (gpc_vertex*) malloc(tn->active * sizeof(gpc_vertex));
                v= 0;
                if (INVERT_TRISTRIPS)
                {
                    lt= tn->v[RIGHT];
                    rt= tn->v[LEFT];
                }
                else
                {
                    lt= tn->v[LEFT];
                    rt= tn->v[RIGHT];
                }
                while (lt || rt)
                {
                    if (lt)
                    {
                        ltn= lt->next;
                        result->strip[s].push_back(gpc_vertex(lt->x, lt->y));
                        //result->strip[s][v].x= lt->x;
                        //result->strip[s][v].y= lt->y;
                        v++;
                        FREE(lt);
                        lt= ltn;
                    }
                    if (rt)
                    {
                        rtn= rt->next;
                        result->strip[s].push_back(gpc_vertex(rt->x, rt->y));
                        // result->strip[s][v].x= rt->x;
                        // result->strip[s][v].y= rt->y;
                        v++;
                        FREE(rt);
                        rt= rtn;
                    }
                }
                s++;
            }
            else
            {
                /* Invalid tristrip: just free the heap */
                for (lt= tn->v[LEFT]; lt; lt= ltn)
                {
                    ltn= lt->next;
                    FREE(lt);
                }
                for (rt= tn->v[RIGHT]; rt; rt=rtn)
                {
                    rtn= rt->next;
                    FREE(rt);
                }
            }
            FREE(tn);
        }
    }

    /* Tidy up */
    reset_it(&it);
    reset_lmt(&lmt);
    FREE(c_heap);
    FREE(s_heap);
    FREE(sbt);
}


double gpc_triangle_area(gpc_vertex *v1,gpc_vertex *v2,gpc_vertex *v3)
{
    gpc_vertex u,v;
    double area;
    u.x = v2->x-v1->x;
    u.y = v2->y-v1->y;
    v.x = v3->x-v1->x;
    v.y = v3->y-v1->y;
    area = 0.5 * fabs(u.x*v.y - u.y*v.x); 
    /*
       printf("Triangle (%f,%f) (%f,%f) (%f,%f) -> %f\n",
       v1->x,v1->y,v2->x,v2->y,v3->x,v3->y,area);
     */
    return area;
}



double gpc_tristrip_area(gpc_tristrip * t)
{
    double somme = 0.0;
    int i;
    unsigned int j;
    for(i=0;i<t->num_strips;i++)
    {
        /*
           printf("strip %d\n",i);
         */
        for(j=0;j<t->strip[i].size()-2;j++)
            somme += gpc_triangle_area(
                    &(t->strip[i][j]),
                    &(t->strip[i][j+1]),
                    &(t->strip[i][j+2]));
    }
    return somme;
}   



double gpc_poly_area(gpc_polygon * p)
{
    double area;
    gpc_tristrip t;
    gpc_polygon_to_tristrip(p,&t);
    area = gpc_tristrip_area(&t);
    gpc_free_tristrip(&t);
    return area;
}



int gpc_test_intersection(gpc_polygon *p1,gpc_polygon *p2)
{
    double res;
    gpc_polygon result;
    gpc_polygon_clip(GPC_INT,p1,p2,&result);
    res = gpc_poly_area(&result);
    gpc_free_polygon(&result);
    if (res > GPC_EPSILON)
        return 1;
    else 
        return 0;
}


void gpc_get_center(gpc_polygon *p,gpc_vertex *v)
{
    unsigned int i,j;
    int n = 0;
    v->x = 0.0;
    v->y = 0.0;
    for(i=0;i<p->contour.size();i++)
        for(j=0;j<p->contour[i].size();j++)
        {
            v->x += p->contour[i][j].x;
            v->y += p->contour[i][j].y;
            n += 1;
        }
    if (n>0)
    {
        v->x /= n;
        v->y /= n;
    }
}

void gpc_transform_polygon(gpc_polygon *source,
        double theta,double x, double y,gpc_polygon *dest)
{
    unsigned int i,j;
    double xnew,ynew,xold,yold;
    double c,s;
    gpc_vertex center;
    c = cos(theta);
    s = sin(theta);
    gpc_copy_polygon(source,dest);
    gpc_get_center(dest,&center);
    for(i=0;i<source->contour.size();i++)
        for(j=0;j<source->contour[i].size();j++)
        {
            xold = source->contour[i][j].x - center.x;
            yold = source->contour[i][j].y - center.y;
            xnew = center.x + xold * c - yold * s;
            ynew = center.y + xold * s + yold * c;
            dest->contour[i][j].x = x + xnew;
            dest->contour[i][j].y = y + ynew;
        }
}


void gpc_find_bounding_circle_radius(gpc_polygon *p,gpc_vertex *center,double *radius)
{
    unsigned int i,j;
    double r,rmax;
    gpc_get_center(p,center);

    rmax = 0.0;
    for(i=0;i<p->contour.size();i++)
        for(j=0;j<p->contour[i].size();j++)
        {
            r = gpc_vertex_dist(center,&(p->contour[i][j]));
            rmax = (r>rmax)?r:rmax;
        }
    *radius = rmax;
}


void gpc_filter_polygon(gpc_polygon * poly)
{
    double x,y;
    int i,j;
    int k,lastIn,insPos,svnContours;
    gpc_polygon poly1;
    gpc_copy_polygon(poly,&poly1);
    svnContours = poly->contour.size();
    for (i=0;i< (int) poly->contour.size();i++)
    {
        insPos = 0;
        lastIn = -1;
        j = 1;
        while (j < (int) poly->contour[i].size())
        {

            if (gpc_vertex_dist(&(poly->contour[i][j-1]),
                        &(poly->contour[i][j]))>GPC_EPSILON)
            {
                // a new real point
                x = y = 0;
                for (k=lastIn+1;k<(int) j;k++)
                {
                    x+= poly->contour[i][k].x;
                    y+= poly->contour[i][k].y;
                }
                poly1.contour[i][insPos].x = x/(j-lastIn-1); 
                poly1.contour[i][insPos].y = y/(j-lastIn-1); 
                insPos += 1;
                lastIn = j-1;
            }


            j+=1;
        }
        // last segment
        x = y = 0;
        for (k=lastIn+1;k<j;k++)
        {
            x+= poly->contour[i][k].x;
            y+= poly->contour[i][k].y;
        }
        poly1.contour[i][insPos].x = x/(j-lastIn-1); 
        poly1.contour[i][insPos].y = y/(j-lastIn-1); 
 //       poly1.contour[i].size() = insPos+1;////////////////////
    }
    gpc_free_polygon(poly);
    gpc_copy_polygon(&poly1,poly);
  //  poly1.num_contours = svnContours;//////////////////////////////////////
//	poly1.contour.num_contours = svnContours;//////////////////////////////////
    gpc_free_polygon(&poly1);
}


/*
   ===========================================================================
   End of file: gpc.c
   ===========================================================================
 */
