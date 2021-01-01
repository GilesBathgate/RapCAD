/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2021 Giles Bathgate
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
#include "cgalsanitizer.h"


CGALSanitizer::CGALSanitizer(CGAL::Polyhedron3& p) :
	polyhedron(p)
{}

void CGALSanitizer::sanitize()
{
	fixZeroEdges();
	fixZeroTriangles();
}

void CGALSanitizer::fixZeroEdges()
{
	while(removeShortEdges());
}

void CGALSanitizer::fixZeroTriangles()
{
	using FacetIterator = CGAL::Polyhedron3::Facet_iterator;
	for(FacetIterator f=polyhedron.facets_begin(); f!=polyhedron.facets_end(); ++f) {
		if(f->facet_degree()>3) continue;
		CGAL::HalfedgeHandle h1=f->halfedge();
		CGAL::HalfedgeHandle h2=h1->next();
		CGAL::HalfedgeHandle h3=h2->next();
		const CGAL::Point3& p1=h1->vertex()->point();
		const CGAL::Point3& p2=h2->vertex()->point();
		const CGAL::Point3& p3=h3->vertex()->point();
		if(CGAL::collinear(p1,p2,p3)) {
			removeLongestEdge(h1,h2,h3);
		}
	}
}

void CGALSanitizer::removeLongestEdge(const CGAL::HalfedgeHandle& h1, const CGAL::HalfedgeHandle& h2, const CGAL::HalfedgeHandle& h3)
{
	CGAL::Scalar l1=getLength(h1);
	CGAL::Scalar l2=getLength(h2);
	CGAL::Scalar l3=getLength(h3);

	if(l1>l2&&l1>l3)
		polyhedron.join_facet(h1);
	if(l2>l1&&l2>l3)
		polyhedron.join_facet(h2);
	if(l3>l1&&l3>l2)
		polyhedron.join_facet(h3);
}

CGAL::Scalar CGALSanitizer::getLength(const CGAL::HalfedgeConstHandle& h)
{
	return CGAL::squared_distance(h->vertex()->point(),h->opposite()->vertex()->point());
}

bool CGALSanitizer::hasLength(const CGAL::HalfedgeConstHandle& h)
{
	return getLength(h)!=0.0;
}

void CGALSanitizer::removeShortEdge(const CGAL::HalfedgeHandle& h1)
{
	// Determine the number of edges surrounding the vertex. e.g. \|/ or |/
	auto edges=h1->vertex_degree();
	if(edges<3) {
		polyhedron.erase_facet(h1);
	} else if(edges==3) {
		CGAL::HalfedgeHandle h2(h1->next());
		if(hasLength(h2))
			polyhedron.join_facet(h2);
		polyhedron.join_vertex(h1);
	} else {
		CGAL::HalfedgeHandle h2(h1->next());
		CGAL::HalfedgeHandle h3(h1->opposite()->next());
		if(hasLength(h2))
			polyhedron.join_facet(h2);
		if(hasLength(h3))
			polyhedron.join_facet(h3);
		polyhedron.join_vertex(h1);
	}
}

bool CGALSanitizer::removeShortEdges()
{
	using HalfedgeIterator = CGAL::Polyhedron3::Halfedge_iterator;
	for(HalfedgeIterator h=polyhedron.halfedges_begin(); h!=polyhedron.halfedges_end(); ++h) {
		if(!hasLength(h)) {
			removeShortEdge(h);
			return true;
		}
	}
	return false;
}
