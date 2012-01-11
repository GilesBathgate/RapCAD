/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2012 Giles Bathgate
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
#include <QMap>
#include <CGAL/config.h>

CGALExplorer::CGALExplorer(const CGAL::NefPolyhedron3& p) : poly(p)
{
	evaluated=false;
}

#if CGAL_VERSION_NR < CGAL_VERSION_NUMBER(3,7,0)
static bool operator<(CGALExplorer::HalfEdgeHandle h1,CGALExplorer::HalfEdgeHandle h2)
{
	return &(*h1) < &(*h2);
}
#endif

static CGALExplorer::HalfEdgeHandle getID(CGALExplorer::HalfEdgeHandle h)
{
	return h<h->twin()?h:h->twin();
}

void CGALExplorer::evaluate()
{
	typedef Nef::Halffacet_const_iterator HalfFacetIterator;
	typedef Nef::Halffacet_cycle_const_iterator HalfFacetCycleIterator;
	typedef Nef::SHalfedge_const_handle SHalfEdgeHandle;
	typedef Nef::SHalfedge_around_facet_const_circulator SHalfEdgeCirculator;
	typedef Nef::SVertex_const_handle SVertexHandle;
	typedef Nef::Vector_3 Vector3;

	primitive = new CGALPrimitive();
	QMap<HalfEdgeHandle,int> periMap;
	HalfFacetIterator f;
	CGAL_forall_halffacets(f,poly) {
		bool facet = !f->is_twin();
		if(facet) {
			CGALPolygon* pg=primitive->createPolygon();
			Vector3 v = f->plane().orthogonal_vector();
			pg->setNormal(v);
		}
		HalfFacetCycleIterator fc;
		CGAL_forall_facet_cycles_of(fc,f) {
			if(fc.is_shalfedge()) {
				SHalfEdgeHandle h = fc;
				SHalfEdgeCirculator hc(h), he(hc);
				CGAL_For_all(hc,he) {
					if(facet) {
						SVertexHandle sv = hc->source();
						CGAL::Point3 sp = sv->source()->point();
						primitive->appendVertex(sp);
					}
					HalfEdgeHandle h=getID(hc->source());
					periMap[h]++;
				}
			}
		}
	}

	QList<HalfEdgeHandle> outEdges;
	for(QMap<HalfEdgeHandle,int>::iterator
			it=periMap.begin(); it!=periMap.end(); ++it)
		if(it.value()==2)
			outEdges.append(it.key());

	if(outEdges.size()>0) {
		HalfEdgeHandle current=outEdges.first();
		bool twin=true;
		do {
			foreach(HalfEdgeHandle h,outEdges) {
				if(twin) h=h->twin();
				if(h!=current && h!=current->twin() &&
				current->target()->point()==h->source()->point()) {
					current=h;
					perimeter.append(h);
					break;
				}
			}
			twin=!twin;
		} while(perimeter.size()<outEdges.size());
	}

	evaluated=true;
}

QList<CGALExplorer::HalfEdgeHandle> CGALExplorer::getPerimeter()
{
	if(!evaluated) evaluate();
	return perimeter;
}

CGALPrimitive* CGALExplorer::getPrimitive()
{
	if(!evaluated) evaluate();
	return primitive;
}

QList<CGAL::Point3> CGALExplorer::getPoints()
{
	if(!evaluated) evaluate();
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
