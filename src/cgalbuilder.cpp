/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2017 Giles Bathgate
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
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#ifdef USE_OFFSET
#include <CGAL/create_offset_polygons_2.h>
#endif
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

	for(const auto& p: points) {
		builder.add_vertex(p);
	}

	for(CGALPolygon* pg: polygons) {
		builder.begin_facet();
		for(auto index: pg->getIndexes()) {
			builder.add_vertex_to_facet(index);
		}
		builder.end_facet();
	}

	builder.end_surface();
}

struct FaceInfo {

	FaceInfo() : nestingLevel(-1) {}

	bool inDomain()
	{
		return nestingLevel%2;
	}

	bool isNested()
	{
		return nestingLevel != -1;
	}

	int nestingLevel;
};

template <class CT, class FaceHandle, class Edge>
static void markDomain(CT& ct,FaceHandle start,int index,QList<Edge>& border)
{
	QList<FaceHandle> queue;
	queue.append(start);
	while(!queue.isEmpty()) {
		FaceHandle fh=queue.takeFirst();
		fh->info().nestingLevel=index;
		for(auto i=0; i<3; ++i) {
			FaceHandle n=fh->neighbor(i);
			if(!n->info().isNested()) {
				Edge e(fh,i);
				if(ct.is_constrained(e))
					border.append(e);
				else
					queue.append(n);
			}
		}
	}
}

template <class CT>
static void markDomains(CT& ct)
{
	typedef typename CT::Face_handle FaceHandle;
	typedef typename CT::Edge Edge;

	QList<Edge> border;
	markDomain(ct, ct.infinite_face(), 0, border);
	while(!border.isEmpty()) {
		Edge e=border.takeFirst();
		FaceHandle c=e.first;
		FaceHandle n=c->neighbor(e.second);
		if(!n->info().isNested()) {
			markDomain(ct, n, c->info().nestingLevel+1, border);
		}
	}
}

template <typename CT>
static void insertConstraint(CT& ct,const CGAL::Point3& p,const CGAL::Point3& np)
{
	typedef typename CT::Vertex_handle VertexHandle;

	CGAL::Point2 p2(p.x(),p.y());
	VertexHandle h=ct.insert(p2);
	h->info() = p.z();
	CGAL::Point2 np2(np.x(),np.y());
	ct.insert_constraint(p2,np2);
}

CGALPrimitive* CGALBuilder::triangulate()
{
	typedef CGAL::Triangulation_vertex_base_with_info_2<CGAL::Scalar,CGAL::Kernel3> VertexBase;
	typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo,CGAL::Kernel3> Info;
	typedef CGAL::Constrained_triangulation_face_base_2<CGAL::Kernel3,Info> FaceBase;
	typedef CGAL::Triangulation_data_structure_2<VertexBase,FaceBase> TDS;
	typedef CGAL::Exact_predicates_tag Tag;
	typedef CGAL::Constrained_triangulation_2<CGAL::Kernel3,TDS,Tag> CT;
	typedef CT::Vertex_handle VertexHandle;
	typedef CT::Face_iterator FaceIterator;

	CT ct;
	for(CGALPolygon* pg: primitive->getCGALPolygons()) {
		OnceOnly first;
		CGAL::Point3 np,fp;
		for(const auto& p: pg->getPoints()) {
			if(first())
				fp=p;
			else
				insertConstraint(ct,p,np);
			np=p;
		}
		insertConstraint(ct,fp,np);
	}

	markDomains(ct);

	primitive->clear();

	for(FaceIterator f=ct.finite_faces_begin(); f!=ct.finite_faces_end(); ++f) {
		if(f->info().inDomain()) {
			primitive->createPolygon();
			for(auto i=0; i<3; ++i) {
				VertexHandle h=f->vertex(i);
				CGAL::Point2 p2=h->point();
				CGAL::Point3 p(p2.x(),p2.y(),h->info());
				primitive->appendVertex(p);
			}
		}
	}
	return primitive;
}

#ifndef USE_OFFSET
CGALPrimitive* CGALBuilder::buildOffsetPolygons(const CGAL::Scalar)
{
	return primitive;
}
#else
CGALPrimitive* CGALBuilder::buildOffsetPolygons(const CGAL::Scalar amount)
{
	typedef boost::shared_ptr<CGAL::Polygon2> PolygonPtr;
	typedef std::vector<PolygonPtr> PolygonPtrVector;

	CGAL::Polygon2 poly;
	CGALExplorer e(primitive);
	CGALPrimitive* prim = e.getPrimitive();

	CGAL::Scalar z=0.0;
	for(CGALPolygon* pg: prim->getCGALPolygons()) {
		for(const auto& pt: pg->getPoints()) {
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

	primitive->clear();

	for(PolygonPtr ptr: offsetPolys) {
		if(!first()) {
			primitive->createPolygon();
			for(auto vi=ptr->vertices_begin(); vi!=ptr->vertices_end(); vi++) {
				CGAL::Point3 p3(vi->x(),vi->y(),z);
				primitive->appendVertex(p3);
			}
		}
	}

	return primitive;
}
#endif


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
