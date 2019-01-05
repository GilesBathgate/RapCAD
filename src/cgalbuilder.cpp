/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2019 Giles Bathgate
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
#ifdef USE_CGAL

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
#include "cgalprojection.h"
#include "onceonly.h"

namespace CGAL
{
typedef Polygon_2<Kernel3> Polygon2;
}

CGALBuilder::CGALBuilder(CGALPrimitive& p) : primitive(p)
{
}

void CGALBuilder::operator()(CGAL::HalfedgeDS& hds)
{
	QList<CGAL::Point3> points=primitive.getPoints();
	QList<CGALPolygon*> polygons=primitive.getCGALPolygons();

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

struct VertexInfo
{
	VertexInfo() : index(-1) {}

	bool isValid()
	{
		return index != -1;
	}

	int index;
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

#if CGAL_VERSION_NR < CGAL_VERSION_NUMBER(4,6,0)
template <class CT,class PointIterator>
void insert_constraint(CT& ct,PointIterator first, PointIterator last, bool close=false)
{
	typedef typename CT::Vertex_handle VertexHandle;
	typedef typename CT::Geom_traits::Point_2 Point;

	if(first == last){
		return;
	}
	const Point& p0 = *first;
	Point p = p0;
	VertexHandle v0 = ct.insert(p0), v(v0), w(v0);
	++first;
	for(; first!=last; ++first){
		const Point& q = *first;
		if(p != q){
			w = ct.insert(q);
			ct.insert_constraint(v,w);
			v = w;
			p = q;
		}
	}
	if(close && (p != p0)){
		ct.insert(w,v0);
	}
}
#endif

bool CGALBuilder::triangulate()
{
	typedef CGAL::Triangulation_vertex_base_with_info_2<VertexInfo,CGAL::Kernel3> VertexBase;
	typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo,CGAL::Kernel3> Info;
	typedef CGAL::Constrained_triangulation_face_base_2<CGAL::Kernel3,Info> FaceBase;
	typedef CGAL::Triangulation_data_structure_2<VertexBase,FaceBase> TDS;
	typedef CGAL::Exact_predicates_tag Tag;
	typedef CGAL::Constrained_triangulation_2<CGAL::Kernel3,TDS,Tag> CT;
	typedef CT::Vertex_handle VertexHandle;
	typedef CT::Face_iterator FaceIterator;


	QList<CGAL::Point3> points3=primitive.getPoints();
	int total=points3.size();
	if(total<3)
		return false;
	else if(total==3)
		return true;

	CT ct;
	TDS::size_type count=0;
	for(CGALPolygon* pg: primitive.getCGALPolygons()) {
		QList<int> indexes=pg->getIndexes();
		if(indexes.size()<3) continue;
		CGALProjection* pro=pg->getProjection();
		QList<CGAL::Point2> points2;
		for(auto i: indexes) {
			CGAL::Point2 p2=pro->project(points3.at(i));
			VertexHandle h=ct.insert(p2);
			h->info().index = i;
			points2.append(p2);
			++count;
		}

#if CGAL_VERSION_NR < CGAL_VERSION_NUMBER(4,6,0)
		insert_constraint(ct,points2.begin(),points2.end(),true);
#else
		ct.insert_constraint(points2.begin(),points2.end(),true);
#endif
	}

	if(count<3||ct.number_of_vertices()<count)
		return false;

	if(ct.number_of_faces()>3)
		markDomains(ct);

	primitive.clearPolygons();

	for(FaceIterator f=ct.finite_faces_begin(); f!=ct.finite_faces_end(); ++f) {
		if(f->info().inDomain()) {
			auto* pg=primitive.createCGALPolygon();
			for(auto i=0; i<3; ++i) {
				VertexInfo info=f->vertex(i)->info();
				if(info.isValid())
					pg->append(info.index);
			}
			pg->calculatePlane();
		}
	}
	return true;
}

#ifndef USE_OFFSET
void CGALBuilder::buildOffsetPolygons(const CGAL::Scalar&) {}
#else
void CGALBuilder::buildOffsetPolygons(const CGAL::Scalar& amount)
{
	typedef boost::shared_ptr<CGAL::Polygon2> PolygonPtr;
	typedef std::vector<PolygonPtr> PolygonPtrVector;

	CGAL::Polygon2 poly;
	CGALExplorer e(&primitive);
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

	primitive.clear();

	for(PolygonPtr ptr: offsetPolys) {
		if(!first()) {
			primitive.createPolygon();
			for(auto vi=ptr->vertices_begin(); vi!=ptr->vertices_end(); ++vi) {
				CGAL::Point3 p3(vi->x(),vi->y(),z);
				primitive.appendVertex(p3);
			}
		}
	}
}
#endif
#endif
