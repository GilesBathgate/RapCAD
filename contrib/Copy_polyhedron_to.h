// Copyright (c) 2009-2015  Max-Planck-Institute Saarbruecken (Germany).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef COPY_POLYHEDRON_TO_H
#define COPY_POLYHEDRON_TO_H

// convert polyhedra from exact to inexact and vice-versa
template <class Polyhedron_input,
		  class Polyhedron_output>
struct Copy_polyhedron_to
		: public CGAL::Modifier_base<typename Polyhedron_output::HalfedgeDS>
{
	Copy_polyhedron_to(const Polyhedron_input& in_poly)
		: in_poly(in_poly) {}

	void operator()(typename Polyhedron_output::HalfedgeDS& out_hds)
	{
		typedef typename Polyhedron_output::HalfedgeDS Output_HDS;

		CGAL::Polyhedron_incremental_builder_3<Output_HDS> builder(out_hds);

		typedef typename Polyhedron_input::Vertex_const_iterator Vertex_const_iterator;
		typedef typename Polyhedron_input::Facet_const_iterator  Facet_const_iterator;
		typedef typename Polyhedron_input::Halfedge_around_facet_const_circulator HFCC;

		builder.begin_surface(in_poly.size_of_vertices(),
							  in_poly.size_of_facets(),
							  in_poly.size_of_halfedges());

		for(Vertex_const_iterator
			vi = in_poly.vertices_begin(), end = in_poly.vertices_end();
			vi != end ; ++vi)
		{
			typename Polyhedron_output::Point_3 p(::CGAL::to_double( vi->point().x()),
												  ::CGAL::to_double( vi->point().y()),
												  ::CGAL::to_double( vi->point().z()));
			builder.add_vertex(p);
		}

		typedef CGAL::Inverse_index<Vertex_const_iterator> Index;
		Index index( in_poly.vertices_begin(), in_poly.vertices_end());

		for(Facet_const_iterator
			fi = in_poly.facets_begin(), end = in_poly.facets_end();
			fi != end; ++fi)
		{
			HFCC hc = fi->facet_begin();
			HFCC hc_end = hc;
			//     std::size_t n = circulator_size( hc);
			//     CGAL_assertion( n >= 3);
			builder.begin_facet ();
			do {
				builder.add_vertex_to_facet(index[hc->vertex()]);
				++hc;
			} while( hc != hc_end);
			builder.end_facet();
		}
		builder.end_surface();
	} // end operator()(..)
private:
	const Polyhedron_input& in_poly;
}; // end Copy_polyhedron_to<>

#endif // COPY_POLYHEDRON_TO_H
