/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2020 Giles Bathgate
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
#ifndef CGALSANITIZER_H
#define CGALSANITIZER_H

#include "cgal.h"
#include <CGAL/Polyhedron_3.h>

namespace CGAL
{
using Polyhedron3 = Polyhedron_3<Kernel3> ;
using HalfedgeDS = Polyhedron3::HalfedgeDS;
using VertexHandle = CGAL::Polyhedron3::Vertex_handle;
using HalfedgeHandle = CGAL::Polyhedron3::Halfedge_handle;
}

class CGALSanitizer :  public CGAL::Modifier_base<CGAL::HalfedgeDS>
{
public:
	CGALSanitizer(CGAL::Polyhedron3& p);
	void sanitize();
private:
	void erase(CGAL::VertexHandle);
	void fixIsolated();
	void fixZero();
	void fixZeroTriangles();
	void fixZeroEdges();
	static CGAL::Scalar getLength(CGAL::HalfedgeHandle);
	void removeShortEdge(CGAL::HalfedgeHandle);
	void removeShortestEdges(CGAL::HalfedgeHandle,CGAL::HalfedgeHandle,CGAL::HalfedgeHandle);
	bool removeShortEdges();
	void operator()(CGAL::HalfedgeDS&) override;

	CGAL::VertexHandle handle;
	CGAL::Polyhedron3& polyhedron;
};

#endif // CGALSANITIZER_H
