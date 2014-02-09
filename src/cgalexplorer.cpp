/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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
#include <QMap>
#include <CGAL/config.h>
#include <CGAL/normal_vector_newell_3.h>
#include "onceonly.h"

CGALExplorer::CGALExplorer(Primitive* p)
{
	primitive=static_cast<CGALPrimitive*>(p);
	evaluated=false;
}

CGALExplorer::CGALExplorer(CGALPrimitive* p)
{
	primitive=p;
	evaluated=false;
}

typedef CGAL::NefPolyhedron3 Nef;
typedef Nef::Volume_const_iterator VolumeIterator;
typedef Nef::Shell_entry_const_iterator ShellEntryIterator;
typedef Nef::Halfedge_const_handle HalfEdgeHandle;
typedef Nef::SFace_const_handle SFaceHandle;

#if CGAL_VERSION_NR < CGAL_VERSION_NUMBER(3,7,0)
static bool operator<(HalfEdgeHandle h1,HalfEdgeHandle h2)
{
	return &(*h1) < &(*h2);
}
#endif

class ShellExplorer
{
	typedef Nef::Halffacet_const_handle HalfFacetHandle;
	typedef Nef::Halffacet_const_iterator HalfFacetIterator;
	typedef Nef::Halffacet_cycle_const_iterator HalfFacetCycleIterator;
	typedef Nef::SHalfedge_const_handle SHalfEdgeHandle;
	typedef Nef::SHalfloop_const_handle SHalfLoopHandle;
	typedef Nef::SHalfedge_around_facet_const_circulator SHalfEdgeCirculator;
	typedef Nef::Vertex_const_handle VertexHandle;
	typedef Nef::SVertex_const_handle SVertexHandle;

	HalfEdgeHandle getID(HalfEdgeHandle h)
	{
		HalfEdgeHandle t=h->twin();
		return h<t?h:t;
	}

	QList<CGAL::Point3> points;
	CGALPrimitive* primitive;
	bool direction;
	QMap<HalfEdgeHandle,int> periMap;
public:
	ShellExplorer() {
		primitive = new CGALPrimitive();
		direction=true;
	}

	void visit(VertexHandle v) {
		points.append(v->point());
	}

	void visit(HalfEdgeHandle) {}
	void visit(HalfFacetHandle f)
	{
		bool facet = !f->is_twin();
		if(facet) {
			CGALPolygon* pg=static_cast<CGALPolygon*>(primitive->createPolygon());
			CGAL::Vector3 v = f->plane().orthogonal_vector();
			pg->setNormal(v);
			HalfFacetCycleIterator fc;
			CGAL_forall_facet_cycles_of(fc,f) {
				if(fc.is_shalfedge()) {
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

	void visit(SHalfEdgeHandle hc) {
		HalfEdgeHandle h = getID(hc->source());
		periMap[h]++;
	}

	void visit(SHalfLoopHandle) {}
	void visit(SFaceHandle) {}

	QList<CGAL::Point3> getPoints()
	{
		return points;
	}

	void setDirection()
	{
		direction=false;
	}

	CGALPrimitive* getPrimitive()
	{
		return primitive;
	}

	QMap<HalfEdgeHandle,int> getPerimeterMap()
	{
		return periMap;
	}
};

static HalfEdgeHandle findNewEdge(QList<HalfEdgeHandle> visited,QList<HalfEdgeHandle> edges)
{
	foreach(HalfEdgeHandle h, edges)
		if(!visited.contains(h) && !visited.contains(h->twin()))
			return h;

	return NULL;
}

void CGALExplorer::evaluate()
{
	const CGAL::NefPolyhedron3& poly=primitive->getNefPolyhedron();
	ShellExplorer se;
	VolumeIterator vi;
	CGAL_forall_volumes(vi,poly) {
		ShellEntryIterator si;
		CGAL_forall_shells_of(si,vi) {
			poly.visit_shell_objects(SFaceHandle(si),se);
		}
		se.setDirection();
	}

	allPoints=se.getPoints();
	primitive=se.getPrimitive();
	QMap<HalfEdgeHandle,int> periMap=se.getPerimeterMap();


	/* A halfedge will belong the the facet twice, the upper
	 * halffacet and the lower halffacet. If this is not the
	 * case then the edge is not on the perimeter. */
	QList<HalfEdgeHandle> outEdges;
	QMap<HalfEdgeHandle, int>::const_iterator it;
	for(it = periMap.constBegin(); it!=periMap.constEnd(); ++it) {
		if(it.value()==2)
			outEdges.append(it.key());
	}

	if(outEdges.size()>0) {

		/* Now walk the perimeter moving from source to target along
		 * each halfedge so that the edges come out in the correct
		 * order. We check that we didnt reverse direction and if
		 * we did we walk along the twin edge. */
		CGALPolygon* poly=new CGALPolygon();
		HalfEdgeHandle f=outEdges.first();
		HalfEdgeHandle c=f;
		QList<HalfEdgeHandle> visited;
		CGAL::Point3 fp;
		bool twin=true;
		bool first=true;
		do {
			foreach(HalfEdgeHandle h,outEdges) {
				if(twin) h=h->twin();
				if(c!=NULL && h!=c && h!=c->twin()) {
					CGAL::Point3 cp=c->target()->point();
					CGAL::Point3 np=h->source()->point();
					if(cp==np) {

						if(first) {
							fp=np;
							first=false;
						}

						poly->append(np);
						visited.append(h);
						c=h;

						if(h==f) {
							poly->append(fp);

							//Calculate the normal of the perimeter polygon
							CGAL::Vector3 v;
							QList<CGAL::Point3> pts=poly->getPoints();
							CGAL::normal_vector_newell_3(pts.begin(),pts.end(),v);
							poly->setNormal(v);

							perimeters.append(poly);
							poly=new CGALPolygon();
							f=findNewEdge(visited,outEdges);
							c=f;
							first=true;
						}
					}
				}
			}
			twin=!twin;
		} while(visited.size()<outEdges.size());
	}

	evaluated=true;
}

QList<CGALPolygon*> CGALExplorer::getPerimeters()
{
	if(!evaluated) evaluate();
	return perimeters;
}

CGALPrimitive* CGALExplorer::getPrimitive()
{
	if(!evaluated) evaluate();
	return primitive;
}

QList<CGAL::Point3> CGALExplorer::getPoints()
{
	if(!evaluated) evaluate();
	return allPoints;
}

CGAL::Cuboid3 CGALExplorer::getBounds()
{
	QList<CGAL::Point3> pts=getPoints();
	return CGAL::bounding_box(pts.begin(),pts.end());
}
#endif
