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
#ifdef USE_CGAL
#include "cgalexplorer.h"
#include <QHash>
#include <CGAL/config.h>
#include <CGAL/Triangulation_3.h>
#include <CGAL/centroid.h>
#include <CGAL/bounding_box.h>
#include "onceonly.h"

using Nef = CGAL::NefPolyhedron3;
using VolumeIterator = Nef::Volume_const_iterator;
using ShellEntryIterator = Nef::Shell_entry_const_iterator;
using SFaceHandle = Nef::SFace_const_handle;

#if CGAL_VERSION_NR < CGAL_VERSION_NUMBER(3,7,0)
static bool operator<(HalfEdgeHandle h1,HalfEdgeHandle h2)
{
	return &(*h1) < &(*h2);
}
#endif

class ShellExplorer
{
	using HalfFacetHandle = Nef::Halffacet_const_handle;
	using HalfFacetIterator = Nef::Halffacet_const_iterator;
	using HalfFacetCycleIterator = Nef::Halffacet_cycle_const_iterator;
	using SHalfEdgeHandle = Nef::SHalfedge_const_handle;
	using SHalfLoopHandle = Nef::SHalfloop_const_handle;
	using SHalfEdgeCirculator = Nef::SHalfedge_around_facet_const_circulator;
	using VertexHandle = Nef::Vertex_const_handle;
	using SVertexHandle = Nef::SVertex_const_handle;

public:
	explicit ShellExplorer(const CGAL::NefPolyhedron3&);
	void createPerimeters();
	void explore();
	void evaluate();

	void visit(VertexHandle);
	void visit(HalfEdgeHandle) {}
	void visit(HalfFacetHandle);
	void visit(SHalfEdgeHandle);
	void visit(SHalfLoopHandle) {}
	void visit(SFaceHandle) {}

	QList<CGALPolygon*> getBase();
	CGALPrimitive* getPrimitive() const;
	CGALVolume getVolume(bool) const;

private:
	static HalfEdgeHandle findNewEdge(const QList<HalfEdgeHandle>&,const QList<HalfEdgeHandle>&);
	static HalfEdgeHandle getID(HalfEdgeHandle);
	bool isBase(const CGALPolygon&) const;

	bool direction;
	const CGAL::NefPolyhedron3& nefPolyhedron;
	CGALPrimitive* primitive;
	QList<CGAL::Point3> points;
	QHash<HalfEdgeHandle,int> perimeterMap;
	QList<CGALPolygon*> basePolygons;
	using Points = QList<CGAL::Point3>;
	QList<Points> volumePoints;
};

ShellExplorer::ShellExplorer(const CGAL::NefPolyhedron3& n) :
	direction(true),
	nefPolyhedron(n),
	primitive(nullptr)
{
}

void ShellExplorer::createPerimeters()
{
	/* A halfedge will belong the the facet twice, the upper
		 * halffacet and the lower halffacet. If this is not the
		 * case then the edge is not on the perimeter. */
	QList<HalfEdgeHandle> outEdges;
	for(auto it=perimeterMap.constBegin(); it!=perimeterMap.constEnd(); ++it) {
		if(it.value()==2)
			outEdges.append(it.key());
	}

	if(!outEdges.empty()) {

		/* Now walk the perimeter moving from source to target along
			 * each halfedge so that the edges come out in the correct
			 * order. We check that we didnt reverse direction and if
			 * we did we walk along the twin edge. */
		CGALPolygon* perimeter=&primitive->createPerimeter();
		HalfEdgeHandle f=outEdges.first();
		HalfEdgeHandle c=f;
		QList<HalfEdgeHandle> visited;
		CGAL::Point3 fp;
		bool twin=true;
		bool first=true;
		do {
			for(HalfEdgeHandle h: outEdges) {
				if(twin) h=h->twin();
				if(c!=nullptr && h!=c && h!=c->twin()) {
					CGAL::Point3 cp=c->target()->point();
					CGAL::Point3 np=h->source()->point();
					if(cp==np) {

						if(first) {
							fp=np;
							first=false;
						}

						perimeter->appendVertex(np);
						visited.append(h);
						c=h;

						if(h==f) {
							perimeter->appendVertex(fp);
							perimeter->calculatePlane();

							f=findNewEdge(visited,outEdges);
							if(f==nullptr)
								return;

							perimeter=&primitive->createPerimeter();
							c=f;
							first=true;
						}
					}
				}
			}
			twin=!twin;
		} while(visited.size()<outEdges.size());
	}
}

void ShellExplorer::explore()
{
	primitive=new CGALPrimitive();
	VolumeIterator vi;
	OnceOnly first_v;
	CGAL_forall_volumes(vi,nefPolyhedron) {
		ShellEntryIterator si;
		CGAL_forall_shells_of(si,vi) {
			/* In the list of shells of a volume, the first one is always the
				 * enclosing shell. In case of the outer volume, there is no outer
				 * shell. */
			nefPolyhedron.visit_shell_objects(SFaceHandle(si),*this);
		}

		/* The first volume, i.e. N.volumes_begin(), is always the outer
			 * volume. Sometimes a facet will belong to the outer volume with
			 * the normals reversed so, flip the direction of all the facets
			 * in subsequent volumes. */
		if(first_v()) {
			direction=false;
		} else {
			volumePoints.append(points);
		}
		points.clear();
	}
}

void ShellExplorer::evaluate()
{
	createPerimeters();
	primitive->detectPerimeterHoles();
	if(!primitive->getSanitized()) {
		primitive->triangulate();
		primitive->setSanitized(true);
	}
}

void ShellExplorer::visit(ShellExplorer::VertexHandle v)
{
	points.append(v->point());
}

void ShellExplorer::visit(ShellExplorer::HalfFacetHandle f)
{
	bool facet = !f->is_twin();
	if(facet) {
		OnceOnly first;
		HalfFacetCycleIterator fc;
		CGAL_forall_facet_cycles_of(fc,f) {
			/* When there is more than one facet cycle we have holes */
			if(!first())
				primitive->setSanitized(false);

			if(fc.is_shalfedge()) {
				CGALPolygon& pg=primitive->createPolygon();
				pg.setPlane(f->plane());
				if(isBase(pg))
					basePolygons.append(&pg);

				SHalfEdgeHandle h = fc;
				SHalfEdgeCirculator hc(h);
				SHalfEdgeCirculator he(hc);
				CGAL_For_all(hc,he) {
					SVertexHandle sv = hc->source();
					CGAL::Point3 sp = sv->source()->point();
					pg.appendVertex(sp,direction);
				}
			}
		}
	}
}

void ShellExplorer::visit(ShellExplorer::SHalfEdgeHandle hc)
{
	HalfEdgeHandle h = getID(hc->source());
	perimeterMap[h]++;
}

QList<CGALPolygon*> ShellExplorer::getBase()
{
	return basePolygons;
}

CGALPrimitive* ShellExplorer::getPrimitive() const
{
	return primitive;
}

CGALVolume ShellExplorer::getVolume(bool calcMass) const
{
	using Triangulation = CGAL::Triangulation_3<CGAL::Kernel3>;
	using CellIterator = Triangulation::Finite_cells_iterator;
	using CellHandle = Triangulation::Cell_handle;
	using Tetrahedron = Triangulation::Tetrahedron;

	CGAL::Scalar total=0.0;
	QList<Tetrahedron> volumes;
	QList<CGAL::Point3> allPoints;
	for(const auto& pts: volumePoints) {
		Triangulation tr(pts.begin(),pts.end());
		CellIterator ci;
		for(ci=tr.finite_cells_begin(); ci!=tr.finite_cells_end(); ++ci) {
			Tetrahedron t=tr.tetrahedron(CellHandle(ci));
			volumes.append(t);
			total+=t.volume();
		}
		allPoints.append(pts);
	}

	CGAL::Cuboid3 b=CGAL::bounding_box(allPoints.begin(),allPoints.end());
	if(calcMass) {
		CGAL::Point3 cm=CGAL::centroid(volumes.begin(),volumes.end());
		return CGALVolume(b,total,cm);
	}

	CGAL::Scalar cx=(b.xmin()+b.xmax())/2.0;
	CGAL::Scalar cy=(b.ymin()+b.ymax())/2.0;
	CGAL::Scalar cz=(b.zmin()+b.zmax())/2.0;
	return CGALVolume(b,total,CGAL::Point3(cx,cy,cz));
}

HalfEdgeHandle ShellExplorer::findNewEdge(const QList<HalfEdgeHandle>& visited, const QList<HalfEdgeHandle>& edges)
{
	for(HalfEdgeHandle h: edges)
		if(!visited.contains(h) && !visited.contains(h->twin()))
			return h;

	return nullptr;
}

HalfEdgeHandle ShellExplorer::getID(HalfEdgeHandle h)
{
	HalfEdgeHandle t=h->twin();
	return h<t?h:t;
}

bool ShellExplorer::isBase(const CGALPolygon& p) const
{
	CGAL::Vector3 v=p.getNormal();
	return (v.x()==0.0&&v.y()==0.0)&&direction?v.z()<0.0:v.z()>0.0;
}

CGALExplorer::CGALExplorer(Primitive* p) :
	CGALExplorer(dynamic_cast<CGALPrimitive*>(p))
{
}

CGALExplorer::CGALExplorer(CGALPrimitive* p) :
	primitive(p),
	explorer(nullptr)
{
}

CGALExplorer::~CGALExplorer()
{
	delete explorer;
}

bool CGALExplorer::explore()
{
	if(!primitive) return false;
	if(!explorer) {
		explorer=new ShellExplorer(primitive->getNefPolyhedron());
		explorer->explore();
	}
	return true;
}

bool CGALExplorer::evaluate()
{
	if(!explore()) return false;

	explorer->evaluate();
	return true;
}

CGALPrimitive* CGALExplorer::getPrimitive()
{
	if(!evaluate())
		return  nullptr;

	return explorer->getPrimitive();
}

const QList<CGALPolygon*> CGALExplorer::getBase()
{
	if(!explore())
		return QList<CGALPolygon*>();

	return explorer->getBase();
}

CGALVolume CGALExplorer::getVolume(bool calcMass)
{
	if(!explore())
		return CGALVolume();

	return explorer->getVolume(calcMass);
}
#endif
