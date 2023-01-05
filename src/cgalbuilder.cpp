/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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
#include "cgalbuilder.h"

#include "cgal.h"
#include "cgalexplorer.h"
#include "cgalprojection.h"
#include "onceonly.h"
#include <CGAL/Constrained_triangulation_2.h>
#ifdef USE_OFFSET
#include <CGAL/create_offset_polygons_2.h>
#endif
#include <CGAL/Polygon_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <QList>

namespace CGAL
{
using Polygon2 = Polygon_2<Kernel3>;
}

CGALBuilder::CGALBuilder(CGALPrimitive& p) : primitive(p)
{
}

void CGALBuilder::operator()(CGAL::HalfedgeDS& hds)
{
	const QList<CGAL::Point3> points=primitive.getPoints();
	const QList<CGALPolygon*> polygons=primitive.getCGALPolygons();

	CGAL::Polyhedron_incremental_builder_3<CGAL::HalfedgeDS> builder(hds,true);
	builder.begin_surface(points.size(), polygons.size());

	for(const auto& p: points)
		builder.add_vertex(p);

	bool sanitized=primitive.getSanitized();
	for(CGALPolygon* pg: polygons) {
		if(!sanitized) {
			auto indexes=pg->getIndexes();
			const auto& begin=indexes.begin();
			const auto& end=std::unique(begin,indexes.end());
			if(builder.test_facet(begin,end))
				builder.add_facet(begin,end);
		} else {
			const auto& indexes=pg->getIndexes();
			builder.add_facet(indexes.begin(),indexes.end());
		}
	}

	builder.end_surface();

	if(!sanitized) {
		builder.remove_unconnected_vertices();
	}
}

struct FaceInfo {

	FaceInfo() : nestingLevel(-1) {}

	bool inDomain() const
	{
		return nestingLevel%2;
	}

	bool isNested() const
	{
		return nestingLevel != -1;
	}

	int nestingLevel;
};

struct VertexInfo {
	VertexInfo() : index(-1) {}

	bool isValid() const
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
	using FaceHandle = typename CT::Face_handle;
	using Edge = typename CT::Edge;

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
	using VertexHandle = typename CT::Vertex_handle;
	using Point = typename CT::Geom_traits::Point_2 ;

	if(first == last) {
		return;
	}
	const Point& p0 = *first;
	Point p = p0;
	VertexHandle v0 = ct.insert(p0), v(v0), w(v0);
	++first;
	for(; first!=last; ++first) {
		const Point& q = *first;
		if(p != q) {
			w = ct.insert(q);
			ct.insert_constraint(v,w);
			v = w;
			p = q;
		}
	}
	if(close && (p != p0)) {
		ct.insert(w,v0);
	}
}
#endif

bool CGALBuilder::triangulate()
{
	using VertexBase = CGAL::Triangulation_vertex_base_with_info_2<VertexInfo, CGAL::Kernel3>;
	using Info = CGAL::Triangulation_face_base_with_info_2<FaceInfo, CGAL::Kernel3>;
	using FaceBase = CGAL::Constrained_triangulation_face_base_2<CGAL::Kernel3, Info>;
	using TDS = CGAL::Triangulation_data_structure_2<VertexBase, FaceBase>;
	using Tag = CGAL::Exact_predicates_tag;
	using CT = CGAL::Constrained_triangulation_2<CGAL::Kernel3, TDS, Tag>;
	using VertexHandle = CT::Vertex_handle;
	using FaceIterator = CT::Face_iterator;


	QList<CGAL::Point3> points3=primitive.getPoints();
	int total=points3.size();
	if(total<3) return false;
	if(total==3) return true;

	CT ct;
	TDS::size_type count=0;
	for(CGALPolygon* pg: primitive.getCGALPolygons()) {
		const QList<int> indexes=pg->getIndexes();
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
			CGALPolygon& pg=primitive.createPolygon();
			for(auto i=0; i<3; ++i) {
				VertexInfo info=f->vertex(i)->info();
				if(info.isValid())
					pg.append(info.index);
			}
			pg.calculatePlane();
		}
	}
	return true;
}

#ifndef USE_OFFSET
CGALPrimitive* CGALBuilder::buildOffset(const CGAL::Scalar&) { return &primitive; }
#else
CGALPrimitive* CGALBuilder::buildOffset(const CGAL::Scalar& amount)
{
	CGAL::Polygon2 polygon;
	CGALExplorer e(&primitive);
	CGALPrimitive* original=e.getPrimitive();

	CGAL::Scalar z=0.0;
	for(CGALPolygon* pg: original->getCGALPolygons()) {
		for(const auto& pt: pg->getPoints()) {
			CGAL::Point2 p2(pt.x(),pt.y());
			polygon.push_back(p2);
			z=pt.z();
		}
		if(pg->getNormal().z()<0.0)
			polygon.reverse_orientation();
	}
	delete original;

	bool interior=amount<0.0;
	const auto& offsetPolys {
		interior?
#if CGAL_VERSION_NR < CGAL_VERSION_NUMBER(5,2,0)
		CGAL::create_interior_skeleton_and_offset_polygons_2(-amount,polygon):
		CGAL::create_exterior_skeleton_and_offset_polygons_2(amount,polygon)
#else
		CGAL::create_interior_skeleton_and_offset_polygons_2(to_double(-amount),polygon):
		CGAL::create_exterior_skeleton_and_offset_polygons_2(to_double(amount),polygon)
#endif
	};

	OnceOnly first;
	if(interior)
		first();

	auto* offset=new CGALPrimitive();
	for(const auto& ptr: offsetPolys) {
		if(!first()) {
			auto& np=offset->createPolygon();
			for(auto vi=ptr->vertices_begin(); vi!=ptr->vertices_end(); ++vi) {
				CGAL::Point3 p3(vi->x(),vi->y(),z);
				np.appendVertex(p3);
			}
		}
	}
	return offset;
}
#endif
#endif
