/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#if USE_CGAL

#include "cgal.h"

#include <QList>
#include <QMap>
#include <CGAL/Constrained_triangulation_2.h>
#include <CGAL/create_offset_polygons_2.h>
#include <CGAL/Polygon_2.h>
#include "cgalbuilder.h"
#include "cgalexplorer.h"
#include "onceonly.h"

namespace CGAL
{
typedef Polygon_2<Kernel3> Polygon2;
}

CGALBuilder::CGALBuilder(CGALPrimitive* p)
{
	primitive=p;
}

void CGALBuilder::operator()(CGAL::HalfedgeDS& hds)
{
	QList<CGAL::Point3> points=primitive->getCGALPoints();
	QList<CGALPolygon*> polygons=primitive->getCGALPolygons();

	CGAL::Polyhedron_incremental_builder_3<CGAL::HalfedgeDS> builder(hds,true);
	builder.begin_surface(points.size(), polygons.size());

	foreach(CGAL::Point3 p, points) {
		builder.add_vertex(p);
	}

	foreach(CGALPolygon* pg, polygons) {
		builder.begin_facet();
		foreach(int index, pg->getIndexes()) {
			builder.add_vertex_to_facet(index);
		}
		builder.end_facet();
	}

	builder.end_surface();
}

template <class CT, class FaceHandle>
static void descend(CT& ct,FaceHandle f,int cw,QMap<FaceHandle,bool>& visited)
{
	FaceHandle h=f->neighbor(cw);
	typename CT::Edge e(f,cw);
	if(!ct.is_constrained(e) && !visited[h]) {
		FaceHandle c(h);
		traverse(ct,c,c->index(f),visited);
	}
}

template <class CT, class FaceHandle>
static void traverse(CT& ct,FaceHandle f,int p,QMap<FaceHandle,bool>& visited)
{
	visited[f]=true;
	descend(ct,f,ct.cw(p),visited);
	descend(ct,f,ct.ccw(p),visited);
}

CGALPrimitive* CGALBuilder::triangulate()
{
	typedef CGAL::Triangulation_vertex_base_2<CGAL::Kernel3> VertexBase;
	typedef CGAL::Constrained_triangulation_face_base_2<CGAL::Kernel3> FaceBase;
	typedef CGAL::Triangulation_data_structure_2<VertexBase,FaceBase> TDS;
	typedef CGAL::Exact_predicates_tag Tag;
	typedef CGAL::Constrained_triangulation_2<CGAL::Kernel3,TDS,Tag> CT;
	typedef CT::Face_iterator FaceIterator;
	typedef CT::Face_handle FaceHandle;
	typedef CT::Vertex_handle VertexHandle;
	typedef CT::Face_circulator FaceCirculator;
	typedef CT::Edge Edge;

	CT ct;
	foreach(CGALPolygon* pg, primitive->getCGALPolygons()) {
		OnceOnly first;
		CGAL::Point2 np;
		CGAL::Point2 fp;
		foreach(CGAL::Point3 p3, pg->getPoints()) {
			CGAL::Point2 p(p3.x(),p3.y());
			if(first()) {
				fp=p;
			} else {
				ct.insert_constraint(p,np);
			}
			np=p;
		}
		ct.insert_constraint(np,fp);
	}

	FaceHandle infinite = ct.infinite_face();
	VertexHandle ctv = infinite->vertex(1);
	if(ct.is_infinite(ctv)) ctv = infinite->vertex(2);

	FaceHandle opposite;
	FaceCirculator vc(ctv,infinite);
	do {
		opposite = vc++;
	} while(!ct.is_constrained(Edge(vc,vc->index(opposite))));
	FaceHandle first = vc;

	QMap<FaceHandle,bool> visited;
	traverse(ct,first,first->index(opposite),visited);

	CGALPrimitive* result = new CGALPrimitive();
	for(FaceIterator f=ct.finite_faces_begin(); f!=ct.finite_faces_end(); ++f) {
		if(visited[f]) {
			result->createPolygon();
			for(int i=0; i<3; ++i) {
				CGAL::Point2 p2=f->vertex(i)->point();
				CGAL::Point3 p(p2.x(),p2.y(),0);
				result->appendVertex(p);
			}
		}
	}
	return result;
}

CGALPrimitive* CGALBuilder::buildOffsetPolygons(const CGAL::Scalar amount)
{
	typedef boost::shared_ptr<CGAL::Polygon2> PolygonPtr;
	typedef std::vector<PolygonPtr> PolygonPtrVector;
	typedef CGAL::Polygon2::Vertex_const_iterator VertexIterator;

	CGAL::Polygon2 poly;
	CGALExplorer e(primitive);
	CGALPrimitive* prim = e.getPrimitive();

	CGAL::Scalar z=0.0;
	foreach(CGALPolygon* pg,prim->getCGALPolygons()) {
		foreach(CGAL::Point3 pt,pg->getPoints()) {
			CGAL::Point2 p2(pt.x(),pt.y());
			poly.push_back(p2);
			z=pt.z();
		}
		if(pg->getNormal().z()<0)
			poly.reverse_orientation();
	}


	OnceOnly first;
	PolygonPtrVector offsetPolys;
	if(amount<0) {
		offsetPolys=CGAL::create_interior_skeleton_and_offset_polygons_2(-amount,poly);
		first();
	} else {
		offsetPolys=CGAL::create_exterior_skeleton_and_offset_polygons_2(amount,poly);
	}

	CGALPrimitive* offsetPrim = new CGALPrimitive();

	foreach(PolygonPtr ptr,offsetPolys) {
		if(!first()) {
			offsetPrim->createPolygon();
			for(VertexIterator vi=ptr->vertices_begin(); vi!=ptr->vertices_end(); vi++) {
				CGAL::Point3 p3(vi->x(),vi->y(),z);
				offsetPrim->appendVertex(p3);
			}
		}
	}

	return offsetPrim;
}

void CGALBuilder::makeSideZ(const CGAL::Scalar& x1,const CGAL::Scalar& x2,const CGAL::Scalar& y1,const CGAL::Scalar& y2,const CGAL::Scalar& z)
{
	primitive->createPolygon(); // sideZ
	primitive->appendVertex(CGAL::Point3(x1, y1, z));
	primitive->appendVertex(CGAL::Point3(x2, y1, z));
	primitive->appendVertex(CGAL::Point3(x2, y2, z));
	primitive->appendVertex(CGAL::Point3(x1, y2, z));
}

void CGALBuilder::makeSideY(const CGAL::Scalar& x1,const CGAL::Scalar& x2,const CGAL::Scalar& y,const CGAL::Scalar& z1,const CGAL::Scalar& z2)
{
	primitive->createPolygon(); // sideY
	primitive->appendVertex(CGAL::Point3(x1, y, z1));
	primitive->appendVertex(CGAL::Point3(x2, y, z1));
	primitive->appendVertex(CGAL::Point3(x2, y, z2));
	primitive->appendVertex(CGAL::Point3(x1, y, z2));
}

void CGALBuilder::makeSideX(const CGAL::Scalar& x,const CGAL::Scalar& y1,const CGAL::Scalar& y2,const CGAL::Scalar& z1,const CGAL::Scalar& z2)
{
	primitive->createPolygon(); // sideX
	primitive->appendVertex(CGAL::Point3(x, y1, z1));
	primitive->appendVertex(CGAL::Point3(x, y2, z1));
	primitive->appendVertex(CGAL::Point3(x, y2, z2));
	primitive->appendVertex(CGAL::Point3(x, y1, z2));
}
#endif
