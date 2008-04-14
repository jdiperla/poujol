
#ifndef POLYGON_H
#define POLYGON_H
/*********************************************
 *
 * Cedric Pradalier
 * DEA 2000/2001 
 * INRIA Rhones Alpes
 * http://cedric.pradalier.free.fr/index.html
 * mail : cedric.pradalier at free.fr
 *
 * *******************************************/


#include <stdlib.h>
//extern "C" {
#include "gpc.h"
//}

#include <utility>
#include "poujol.h"

// Classe impl�entant un wrapper autour des polygones
// de gpc.h
class SPolygon
{
    public :
    
    typedef gpc_vertex Vector2;
    typedef std::pair<Vector2, Vector2> Segment;
    typedef std::vector<Segment> Vector;

    gpc_polygon * poly;
        
        SPolygon() {poly = new gpc_polygon;gpc_init_polygon(poly);}
        
        // Approximation polygonale d'un arc de cercle ferm�
        // (en part de tarte). Plus num_edges est grand, plus l'approximation
        // est valide
        SPolygon(double x_center,double y_center,
                double radius,double theta_min,
                double theta_max,int num_edges) 
        {
            poly = new gpc_polygon;
            gpc_init_polygon(poly);
            gpc_pie_polygon(x_center,y_center,
                    radius,theta_min,theta_max,
                    num_edges,poly);
        }

        // SPolygone rectangulaire
        SPolygon (double x,double y,double w,double h)
        {
            poly = new gpc_polygon;
            gpc_init_polygon(poly);
            gpc_create_rectangle(x,y,w,h,poly);
        }

        // Construction a partir d'une liste de point
        // [x1,y1,x2,y2...xn,yn]. size == n
        SPolygon(double * points,int size)
        {
            poly = new gpc_polygon;
            gpc_init_polygon(poly);
            gpc_create_polygon(points,size,poly);
        }
            

        // Construction d'un polygone a partir d'un gpc_polygon*.
        // le pointeur est affect�directement, sans copie
        SPolygon(gpc_polygon *p) {poly = p;}
        
        // Constructeur de copie
        SPolygon(SPolygon *p) 
        {
            poly = new gpc_polygon;
            gpc_init_polygon(poly);
            gpc_copy_polygon(p->poly,poly);
        }
        
        // Constructeur de copie
        SPolygon(const SPolygon &p) 
        {
            poly = new gpc_polygon;
            gpc_init_polygon(poly);
            gpc_copy_polygon(p.poly,poly);
        }

        virtual ~SPolygon(); 

        // Afectation
        SPolygon & operator=(const SPolygon & P) 
        {
                copySPolygon(P);
                return (*this);
        }

        // copie explicite
        void copySPolygon(const SPolygon & P) 
        {
                delete poly;
                poly = new gpc_polygon;
                gpc_init_polygon(poly);
                gpc_copy_polygon(P.poly,poly);
        }

        
        virtual void Print(FILE*fp=stdout) 
        {
            gpc_write_polygon(fp,true,poly);
        }

		virtual void Print2(std::ostream &f, bool write_hole_flags = true);

        void PrintAsTriStrip(FILE*fp=stdout)
        {
            gpc_write_tristrip(fp,poly);
        }
            
        gpc_polygon* getGpcSPolygon() {return poly;}

        // Lecture dans fp. la syntaxe doit etre correcte.
        void Read(FILE * fp); 
        
        bool isEmpty() {return (poly->contour.size() == 0);}

        // D�omposition du polygone en partie connexe
		// void getConnexList(std::list * list);

        // Operateurs ensemblistes
        SPolygon * Union(SPolygon * p);
        SPolygon * Intersection(SPolygon * p);
        SPolygon * Minus(SPolygon * p);
        SPolygon * Xor(SPolygon * p);

        
        int getNbContours() {return poly->contour.size();}
        
        // teste si (x,y) est un sommet du poly
        bool isAVertex(double x,double y);
        
        // barycentre
        void getCenter(double * x, double * y);

        // cercle circonscrit
        void getBoundingCircle(double * x, double * y,double * r);

        // surface
        double getArea();

        // liste des segments formant les cotes
        void getEdges(Vector &list);

        // fusions des points trop pres l'un de l'autre
        void filter() {gpc_filter_polygon(poly);}
        
        // decoupage au points d'�ranglement :
        //
        // +-----------+
        // |           |                                 
        // |           |                                 
        // |           | point d'etranglement            
        // |           |/                                
        // +-----------+----------+                      
        //             |          |                      
        //             |          |                      
        //             |          |                      
        //             +----------+                      
        //                                                   
        void cut();        
            
        // Nombre de parties connexes
        int numPartConnex();

        // Calcule l'ensemble des points visibles �partir de (x,y).
        // Algorithme TRES TRES inefficace. TODO : utiliser un meilleur algo
        SPolygon * computeVisibility(double x,double y);

        // Collect les points du polygone pour les exporter
        // en PolyData pour Vtk (Visualization Tool Kit)
        // void collectVtkPoints(VtkPoints & points,VtkSPolygons & polygons);
};

#endif // POLYGON_H

