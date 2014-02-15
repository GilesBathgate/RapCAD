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
#include <QList>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include "cgalbuilder.h"
#include "cgalexplorer.h"

CGALBuilder::CGALBuilder(CGALPrimitive* p)
{
	primitive=p;
}

void CGALBuilder::operator()(CGAL::HalfedgeDS& hds)
{
	QList<CGAL::Point3> points=primitive->getPoints();
	QList<CGALPolygon*> polygons=primitive->getPolygons();

	CGAL::Polyhedron_incremental_builder_3<CGAL::HalfedgeDS> builder(hds,true);
	builder.begin_surface(points.size(), polygons.size());

	foreach(CGAL::Point3 p, points) {
		builder.add_vertex(p);
	}

	foreach(CGALPolygon* pg, polygons) {
		builder.begin_facet();
		foreach(CGAL::Point3 p, pg->getPoints()) {
			int index = points.indexOf(p);
			builder.add_vertex_to_facet(index);
		}
		builder.end_facet();
	}

	builder.end_surface();
}

CGALPrimitive* CGALBuilder::buildOffsetPolygons(const CGAL::FT amount)
{
	typedef CGAL::FT FT;
	typedef boost::shared_ptr<CGAL::Polygon2> PolygonPtr;
	typedef std::vector<PolygonPtr> PolygonPtrVector;
	typedef CGAL::Polygon2::Vertex_const_iterator VertexIterator;

	CGAL::Polygon2 poly;
	CGALExplorer e(primitive);
	CGALPrimitive* prim = e.getPrimitive();

	FT z=0.0;
	foreach(CGALPolygon* pg,prim->getPolygons()) {
		foreach(CGAL::Point3 pt,pg->getPoints()) {
			CGAL::Point2 p2(pt.x(),pt.y());
			poly.push_back(p2);
			z=pt.z();
		}
		if(pg->getNormal().z()<0)
			poly.reverse_orientation();
	}


	PolygonPtrVector offsetPolys;
	if(amount<0) {
		offsetPolys=CGAL::create_interior_skeleton_and_offset_polygons_2(-amount,poly);
	} else {
		offsetPolys=CGAL::create_exterior_skeleton_and_offset_polygons_2(amount,poly);
	}

	CGALPrimitive* offsetPrim = new CGALPrimitive();

	foreach(PolygonPtr ptr,offsetPolys) {
		offsetPrim->createPolygon();
		for(VertexIterator vi=ptr->vertices_begin(); vi!=ptr->vertices_end(); vi++) {
			CGAL::Point3 p3(vi->x(),vi->y(),z);
			offsetPrim->appendVertex(p3);
		}
	}

	return offsetPrim;
}
#endif
