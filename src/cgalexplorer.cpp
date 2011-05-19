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

#include "cgalexplorer.h"
#include "float.h"

CGALExplorer::CGALExplorer(const CGAL::NefPolyhedron3& p) : poly(p)
{
}

void CGALExplorer::evaluate()
{
	typedef CGAL::NefPolyhedron3::SNC_structure SNC;
	typedef SNC::Halffacet_const_iterator HalfFacetIterator;
	typedef SNC::Halffacet_cycle_const_iterator HalfFacetCycleIterator;
	typedef SNC::SHalfedge_const_handle SHalfEdgeHandle;
	typedef SNC::SHalfedge_around_facet_const_circulator SHalfEdgeCirculator;
	typedef SNC::Vector_3 Vector3;

	primitive = new CGALPrimitive();
	HalfFacetIterator f;
	CGAL_forall_facets(f,*poly.sncp()) {
		CGALPolygon* pg=primitive->createPolygon();

		Vector3 v = f->plane().orthogonal_vector();
		pg->setNormal(v);

		HalfFacetCycleIterator fc;
		CGAL_forall_facet_cycles_of(fc,f) {
			SHalfEdgeHandle h = fc;
			SHalfEdgeCirculator hc(h), he(hc);
			CGAL_For_all(hc,he) {
				CGAL::Point3 sp = hc->source()->source()->point();
				primitive->appendVertex(sp);
			}
		}
	}
}

CGALPrimitive* CGALExplorer::getPrimitive()
{
	evaluate();
	return primitive;
}

QList<CGAL::Point3> CGALExplorer::getPoints()
{
	evaluate();
	return primitive->getPoints();
}

CGAL::Bbox_3 CGALExplorer::getBounds()
{
	double minX=DBL_MAX,maxX=-DBL_MAX;
	double minY=DBL_MAX,maxY=-DBL_MAX;
	double minZ=DBL_MAX,maxZ=-DBL_MAX;
	foreach(CGAL::Point3 pt,getPoints()) {
		double x=to_double(pt.x());
		double y=to_double(pt.y());
		double z=to_double(pt.z());
		minX=fmin(x,minX);
		maxX=fmax(x,maxX);
		minY=fmin(y,minY);
		maxY=fmax(y,maxY);
		minZ=fmin(z,minZ);
		maxZ=fmax(z,maxZ);
	}

	return CGAL::Bbox_3(minX,minY,minZ,maxX,maxY,maxZ);
}
