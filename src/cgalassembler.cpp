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

#include<CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include<CGAL/Polygon_2.h>
#include<CGAL/create_offset_polygons_2.h>
#include "cgalassembler.h"
#include "cgalexplorer.h"

CGALAssembler::CGALAssembler()
{
}

CGALPrimitive* CGALAssembler::buildInsetPolygons(CGALPrimitive* result, double amount)
{
	typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
	typedef K::FT FT;
	typedef K::Point_2 Point2;
	typedef CGAL::Polygon_2<K> Polygon;
	typedef boost::shared_ptr<Polygon> PolygonPtr;
	typedef std::vector<PolygonPtr> PolygonPtrVector;
	typedef PolygonPtrVector::const_iterator PolygonIterator;
	typedef Polygon::Vertex_const_iterator VertexIterator;

	Polygon poly;
	CGALExplorer e(result->getPoly3());
	CGALPrimitive* prim = e.getPrimitive();

	foreach(CGALPolygon points,prim->getPolygons()) {
		foreach(CGAL::Point3 pt,points){
			poly.push_back(Point2(to_double(pt.x()),to_double(pt.y())));
		}
	}

	FT offset = amount;
	PolygonPtrVector polies = CGAL::create_interior_skeleton_and_offset_polygons_2(offset,poly);

	CGALPrimitive* insetPoly = new CGALPrimitive();
	FT z=0.0;
	for(PolygonIterator pi=polies.begin(); pi!=polies.end(); pi++) {
		insetPoly->createPolygon();
		Polygon pp=**pi;
		for(VertexIterator vi=pp.vertices_begin(); vi!=pp.vertices_end(); vi++) {
			Point2 p2 =*vi;
			insetPoly->appendVertex(CGAL::Point3(p2.x(),p2.y(),z));
		}
	}

	return insetPoly;
}
