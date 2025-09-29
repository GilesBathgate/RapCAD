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
#ifndef CGALSANITIZER_H
#define CGALSANITIZER_H

#include "cgal.h"
#include <CGAL/Polyhedron_3.h>

namespace CGAL
{
using Polyhedron3 = Polyhedron_3<Kernel3> ;
using VertexHandle = CGAL::Polyhedron3::Vertex_handle;
using HalfedgeHandle = CGAL::Polyhedron3::Halfedge_handle;
using HalfedgeConstHandle = CGAL::Polyhedron3::Halfedge_const_handle;
using FacetIterator = CGAL::Polyhedron3::Facet_iterator;
} // namespace CGAL

class CGALSanitizer
{
public:
	CGALSanitizer(CGAL::Polyhedron3&);
	bool sanitize();
private:
	void fixZeroTriangles();
	void fixZeroEdges();
	bool allEdgesSimple();
	bool allFacetsSimple();
	void removeLongestEdge(const CGAL::HalfedgeHandle&,const CGAL::HalfedgeHandle&,const CGAL::HalfedgeHandle&);
	void removeShortEdge(const CGAL::HalfedgeHandle&);
	bool removeShortEdges();

	CGAL::Polyhedron3& polyhedron;
};

#endif // CGALSANITIZER_H
#endif
