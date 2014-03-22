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
#include <CGAL/Triangulation_3.h>
#include <CGAL/centroid.h>
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

	bool isBase(CGAL::Vector3 v)
	{
		return (v.x()==0&&v.y()==0)&&direction?v.z()<0:v.z()>0;
	}

	QList<CGAL::Point3> points;
	CGALPrimitive* primitive;
	bool direction;
	QMap<HalfEdgeHandle,int> periMap;
	QList<CGALPolygon*> basePolygons;
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
			if(isBase(v))
				basePolygons.append(pg);
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

	void clearPoints()
	{
		points.clear();
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

	QList<CGALPolygon*> getBase()
	{
		return basePolygons;
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
	OnceOnly first_v;
	CGAL_forall_volumes(vi,poly) {
		ShellEntryIterator si;
		CGAL_forall_shells_of(si,vi) {
			/* In the list of shells of a volume, the first one is always the
			 * enclosing shell. In case of the outer volume, there is no outer
			 * shell. */
			poly.visit_shell_objects(SFaceHandle(si),se);
		}

		QList<CGAL::Point3> points=se.getPoints();
		allPoints.append(points);
		se.clearPoints();

		/* The first volume, i.e. N.volumes_begin(), is always the outer
		 * volume. Sometimes a facet will belong to the outer volume with
		 * the normals reversed so, flip the direction of all the facets
		 * in subsequent volumes. */
		if(first_v()) {
			se.setDirection();
		} else {
			volumePoints.append(points);
		}
	}

	primitive=se.getPrimitive();
	basePolygons=se.getBase();
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

QList<CGALPolygon*> CGALExplorer::getBase()
{
	if(!evaluated) evaluate();
	return basePolygons;
}

CGALVolume CGALExplorer::getVolume(bool calcMass)
{
	if(!evaluated) evaluate();

	typedef CGAL::Triangulation_3<CGAL::Kernel3> Triangulation;
	typedef Triangulation::Finite_cells_iterator CellIterator;
	typedef Triangulation::Cell_handle CellHandle;
	typedef Triangulation::Tetrahedron Tetrahedron;

	CGAL::FT total=0;
	QList<Tetrahedron> volumes;
	foreach(Points pts, volumePoints) {
		Triangulation tr(pts.begin(),pts.end());
		CellIterator ci;
		for(ci=tr.finite_cells_begin(); ci!=tr.finite_cells_end(); ++ci) {
			Tetrahedron t=tr.tetrahedron(CellHandle(ci));
			volumes.append(t);
			total+=t.volume();
		}
	}
	if(!calcMass) {
		CGAL::Cuboid3 b=getBounds();
		CGAL::FT cx=0.0,cy=0.0,cz=0.0;
		cx=(b.xmin()+b.xmax())/2;
		cy=(b.ymin()+b.ymax())/2;
		cz=(b.zmin()+b.zmax())/2;

		return CGALVolume(b,total,CGAL::Point3(cx,cy,cz));
	}

	CGAL::Point3 cm=CGAL::centroid(volumes.begin(),volumes.end());
	return CGALVolume(getBounds(),total,cm);
}
#endif
