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
#include "cgalsanitizer.h"

#include <QtGlobal>

CGALSanitizer::CGALSanitizer(CGAL::Polyhedron3& p) :
	polyhedron(p)
{}

bool CGALSanitizer::sanitize()
{
	fixZeroEdges();
	fixZeroTriangles();
	return allEdgesSimple() && allFacetsSimple();
}

void CGALSanitizer::fixZeroEdges()
{
	while(removeShortEdges());
}

bool CGALSanitizer::allEdgesSimple()
{
	// Circulate around each of the vertices edges.
	// If a vertex has more than 2 edges then it is a facet fan,
	// which must have at most 2 border edges on the shared vertex
	for(auto v=polyhedron.vertices_begin(); v!=polyhedron.vertices_end(); ++v) {
		if(v->vertex_degree()<=2) continue;
		uint borderEdges=0;
		auto b=v->vertex_begin(),e(b);
		CGAL_For_all(e,b) {
			if(e->is_border_edge())
				++borderEdges;
		}
		if(borderEdges>2)
			return false;
	}

	return true;
}

static bool connected(const CGAL::HalfedgeHandle& h1,const CGAL::HalfedgeHandle& h2)
{
	return(h1==h2||h1->next()==h2||h2->next()==h1||h1->prev()==h2||h2->prev()==h1);
}

static bool do_intersect(const CGAL::HalfedgeHandle& h1, const CGAL::HalfedgeHandle& h2)
{
	using Segment3 = CGAL::Segment_3<CGAL::Kernel3>;
	Segment3 s1(h1->vertex()->point(),h1->opposite()->vertex()->point());
	Segment3 s2(h2->vertex()->point(),h2->opposite()->vertex()->point());
	return CGAL::do_intersect(s1,s2);
}

static bool facetIntersecting(const CGAL::FacetIterator& f)
{
	auto b=f->facet_begin(),e(b),o(b);
	CGAL_For_all(b,e) {
		CGAL_For_all(o,e) {
			if(connected(b,o)) continue;
			if(do_intersect(b,o))
				return true;
		}
	}
	return false;
}

static bool facetNoncoplanar(const CGAL::FacetIterator& f)
{
	auto b=f->facet_begin(),e(b);
	CGAL::Point3 p[4];
	for(auto i=0; i<3; ++i,++b)
		p[i]=b->vertex()->point();
	CGAL_For_all(b,e) {
		p[3]=b->vertex()->point();
		if(!CGAL::coplanar(p[0],p[1],p[2],p[3]))
			return true;
	}

	return false;
}

bool CGALSanitizer::allFacetsSimple()
{
	for(auto f=polyhedron.facets_begin(); f!=polyhedron.facets_end(); ++f) {
		if(f->facet_degree()<=3) continue;
		if(facetIntersecting(f))
			return false;
		if(facetNoncoplanar(f))
			return false;
	}
	return true;
}

void CGALSanitizer::fixZeroTriangles()
{
	for(auto f=polyhedron.facets_begin(); f!=polyhedron.facets_end(); ++f) {
		if(f->facet_degree()>3) continue;
		CGAL::HalfedgeHandle h1=f->halfedge();
		CGAL::HalfedgeHandle h2=h1->next();
		CGAL::HalfedgeHandle h3=h2->next();
		const CGAL::Point3& p1=h1->vertex()->point();
		const CGAL::Point3& p2=h2->vertex()->point();
		const CGAL::Point3& p3=h3->vertex()->point();
		if(CGAL::collinear(p1,p2,p3)) {
			if(h1->is_border_edge()&&h2->is_border_edge()&&h3->is_border_edge()) {
				// facet is isolated just erase it
				polyhedron.erase_facet(h1);
			} else {
				removeLongestEdge(h1,h2,h3);
			}
		}
	}
}

static CGAL::Scalar getLength(const CGAL::HalfedgeConstHandle& h)
{
	return CGAL::squared_distance(h->vertex()->point(),h->opposite()->vertex()->point());
}

static bool hasLength(const CGAL::HalfedgeConstHandle& h)
{
	return getLength(h)!=0.0;
}

void CGALSanitizer::removeLongestEdge(const CGAL::HalfedgeHandle& h1,const CGAL::HalfedgeHandle& h2,const CGAL::HalfedgeHandle& h3)
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
	for(auto h=polyhedron.halfedges_begin(); h!=polyhedron.halfedges_end(); ++h) {
		if(!hasLength(h)) {
			removeShortEdge(h);
			return true;
		}
	}
	return false;
}
#endif
