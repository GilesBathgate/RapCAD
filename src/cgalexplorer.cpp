/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2013 Giles Bathgate
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
#include "cgalexplorer.h"
#include "float.h"
#include <QMap>
#include <CGAL/config.h>
#include <CGAL/normal_vector_newell_3.h>
#include <CGAL/bounding_box.h>

CGALExplorer::CGALExplorer(Primitive* p)
{
	primitive=static_cast<CGALPrimitive*>(p);
	evaluated=false;
	hasPerimeter=false;
}

CGALExplorer::CGALExplorer(CGALPrimitive* p)
{
	primitive=p;
	evaluated=false;
	hasPerimeter=false;
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

typedef CGAL::NefPolyhedron3 Nef;
typedef Nef::Volume_const_iterator VolumeIterator;
typedef Nef::Shell_entry_const_iterator ShellEntryIterator;
typedef Nef::Halffacet_const_handle HalfFacetHandle;
typedef Nef::Halffacet_const_iterator HalfFacetIterator;
typedef Nef::Halffacet_cycle_const_iterator HalfFacetCycleIterator;
typedef Nef::Halfedge_const_handle HalfEdgeHandle;
typedef Nef::SHalfedge_const_handle SHalfEdgeHandle;
typedef Nef::SHalfloop_const_handle SHalfLoopHandle;
typedef Nef::SFace_const_handle SFaceHandle;
typedef Nef::SHalfedge_around_facet_const_circulator SHalfEdgeCirculator;
typedef Nef::Vertex_const_handle VertexHandle;
typedef Nef::SVertex_const_handle SVertexHandle;
typedef Nef::Vector_3 Vector3;

class ShellExplorer
{
	QList<CGAL::Point3> points;
	CGALPrimitive* primitive;
	bool direction;
public:
	ShellExplorer() {
		primitive = new CGALPrimitive();
		direction=false;
	}

	void visit(VertexHandle v) {
		points.append(v->point());
	}

	void visit(HalfEdgeHandle) {}
	void visit(HalfFacetHandle f)
	{
		bool facet = !f->is_twin();
		if(facet) {
			HalfFacetCycleIterator fc;
			CGAL_forall_facet_cycles_of(fc,f) {
				if(fc.is_shalfedge()) {
					primitive->createPolygon();
					SHalfEdgeHandle h = fc;
					SHalfEdgeCirculator hc(h), he(hc);
					CGAL_For_all(hc,he) {
						SVertexHandle sv = hc->source();
						CGAL::Point3 sp = sv->source()->point();
						if(direction)
							primitive->appendVertex(sp);
						else
							primitive->prependVertex(sp);
					}
				}
			}
		}
	}

	void visit(SHalfEdgeHandle) {}
	void visit(SHalfLoopHandle) {}
	void visit(SFaceHandle) {}

	QList<CGAL::Point3> getPoints()
	{
		return points;
	}

	void setDirection()
	{
		direction=true;
	}

	CGALPrimitive* getPrimitive()
	{
		return primitive;
	}
};

void CGALExplorer::evaluate()
{
	const CGAL::NefPolyhedron3& poly=primitive->getNefPolyhedron();
	primitive = new CGALPrimitive();
	QMap<HalfEdgeHandle,int> periMap;
	HalfFacetIterator f;
	CGAL_forall_halffacets(f,poly) {
		bool facet = !f->is_twin();
		if(facet) {
			CGALPolygon* pg=static_cast<CGALPolygon*>(primitive->createPolygon());
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
					perimeterPoints.append(h->source()->point());
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

CGAL::Vector3 CGALExplorer::getPerimeterNormal()
{
	if(!evaluated) evaluate();
	if(!hasPerimeter) {
		CGAL::normal_vector_newell_3(perimeterPoints.begin(),perimeterPoints.end(),perimeterNormal);
		hasPerimeter=true;
	}
	return perimeterNormal;
}

CGALPrimitive* CGALExplorer::getPrimitive()
{
	if(!evaluated) evaluate();
	return primitive;
}

CGALPrimitive* CGALExplorer::getOrientedPrimitive()
{
	const CGAL::NefPolyhedron3& poly=primitive->getNefPolyhedron();
	VolumeIterator vi;
	ShellExplorer se;
	CGAL_forall_volumes(vi,poly) {
		ShellEntryIterator si;
		CGAL_forall_shells_of(si,vi) {
			poly.visit_shell_objects(SFaceHandle(si),se);
		}
		se.setDirection();
	}

	return se.getPrimitive();
}

QList<CGAL::Point3> CGALExplorer::getPoints()
{
	const CGAL::NefPolyhedron3& poly=primitive->getNefPolyhedron();
	VolumeIterator vi;
	ShellExplorer se;
	CGAL_forall_volumes(vi,poly) {
		ShellEntryIterator si;
		CGAL_forall_shells_of(si,vi) {
			poly.visit_shell_objects(SFaceHandle(si),se);
		}
	}
	return se.getPoints();
}

CGAL::Cuboid3 CGALExplorer::getBounds()
{
	QList<CGAL::Point3> pts=getPoints();
	return CGAL::bounding_box(pts.begin(),pts.end());
}
#endif
