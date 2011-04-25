/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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

#include <CGAL/Polyhedron_incremental_builder_3.h>
#include "cgalbuilder.h"
typedef CGAL::HalfedgeDS::Vertex Vertex;

CGALBuilder::CGALBuilder()
{
}

void CGALBuilder::setPrimitive(PrimitiveNode* n)
{
	polyNode=n;
}

void CGALBuilder::operator()(CGAL::HalfedgeDS& hds)
{

	QList<Point> vertices;
	QList<Polygon> polygons=polyNode->getPolygons();
	foreach(Polygon pg, polygons) {
		foreach(Point p, pg) {
			if(!vertices.contains(p))
				vertices.append(p);
		}
	}

	CGAL::Polyhedron_incremental_builder_3<CGAL::HalfedgeDS> builder(hds,true);
	builder.begin_surface(vertices.size(), polygons.size());

	foreach(Point p, vertices) {
		double x,y,z;
		p.getXYZ(x,y,z);
		builder.add_vertex(Vertex::Point(x,y,z));
	}

	foreach(Polygon pg, polygons) {
		builder.begin_facet();
		foreach(Point p, pg) {
			int index = vertices.indexOf(p);
			builder.add_vertex_to_facet(index);
		}
		builder.end_facet();
	}

	builder.end_surface();
}
