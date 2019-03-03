/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2019 Giles Bathgate
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
#include "cgalprimitive.h"
#include <QPair>
#include <CGAL/minkowski_sum_3.h>
#include <CGAL/convex_decomposition_3.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/bounding_box.h>
#include <CGAL/Polygon_2_algorithms.h>

//Mesh simplification
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_ratio_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_length_cost.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Midpoint_placement.h>

#include "cgalbuilder.h"
#include "cgalexplorer.h"
#include "onceonly.h"
#include "rmath.h"

CGALPrimitive::CGALPrimitive() :
	nefPolyhedron(nullptr),
	type(Primitive::Volume),
	sanitized(true),
	nUnion(nullptr)
{
}

CGALPrimitive::CGALPrimitive(CGAL::Polyhedron3& poly) : CGALPrimitive()
{
	nefPolyhedron=new CGAL::NefPolyhedron3(poly);
}

CGALPrimitive::CGALPrimitive(const CGAL::NefPolyhedron3& nef) : CGALPrimitive()
{
	nefPolyhedron=new CGAL::NefPolyhedron3(nef);
}

CGALPrimitive::~CGALPrimitive()
{
	clear();
}

void CGALPrimitive::clear()
{
	delete nefPolyhedron;
	nefPolyhedron=nullptr;

	clearPolygons();

	pointMap.clear();
	points.clear();

	qDeleteAll(children);
	children.clear();
}

void CGALPrimitive::clearPolygons()
{
	qDeleteAll(polygons);
	polygons.clear();
}

void CGALPrimitive::setType(Primitive_t t)
{
	type=t;
}

Primitive::Primitive_t CGALPrimitive::getType()
{
	return type;
}

void CGALPrimitive::setSanitized(bool v)
{
	sanitized=v;
}

bool CGALPrimitive::getSanitized()
{
	return sanitized;
}

typedef CGAL::Polyhedron3::Halfedge_handle HalfedgeHandle;

static CGAL::Scalar getLength(HalfedgeHandle h)
{
	return CGAL::squared_distance(h->vertex()->point(),h->opposite()->vertex()->point());
}

static void removeShortEdge(CGAL::Polyhedron3& p,HalfedgeHandle h)
{
	// Determine the number of edges surrounding the vertex. e.g. \|/ or |/
	auto edges=circulator_size(h->vertex_begin());
	if(edges<3) {
		p.erase_facet(h);
	} else if(edges==3) {
		p.join_facet(h->next());
		p.join_vertex(h);
	} else {
		p.join_facet(h->next());
		p.join_facet(h->opposite()->next());
		p.join_vertex(h);
	}
}

static void removeShortestEdges(CGAL::Polyhedron3& p,HalfedgeHandle h1,HalfedgeHandle h2,HalfedgeHandle h3)
{
	CGAL::Scalar l1=getLength(h1);
	CGAL::Scalar l2=getLength(h2);
	CGAL::Scalar l3=getLength(h3);

	if(l1<l2||l1<l3)
		p.join_facet(h1);
	if(l2<l1||l2<l3)
		p.join_facet(h2);
	if(l3<l1||l3<l2)
		p.join_facet(h3);
}

static void fixZero(CGAL::Polyhedron3& p)
{
	typedef CGAL::Polyhedron3::Facet_iterator FacetIterator;
	for(FacetIterator f=p.facets_begin(); f!=p.facets_end(); ++f) {
		if(f->facet_degree()<3) {
			p.erase_facet(f->halfedge());
		}
	}
}

static void fixZeroTriangles(CGAL::Polyhedron3& p)
{
	typedef CGAL::Polyhedron3::Facet_iterator FacetIterator;
	for(FacetIterator f=p.facets_begin(); f!=p.facets_end(); ++f) {
		if(f->facet_degree()>3) continue;
		HalfedgeHandle h1=f->halfedge();
		HalfedgeHandle h2=h1->next();
		HalfedgeHandle h3=h2->next();
		const CGAL::Point3& p1=h1->vertex()->point();
		const CGAL::Point3& p2=h2->vertex()->point();
		const CGAL::Point3& p3=h3->vertex()->point();
		if(CGAL::collinear(p1,p2,p3)) {
			removeShortestEdges(p,h1,h2,h3);
		}
	}
}

static void fixZeroEdges(CGAL::Polyhedron3& p)
{
	bool removed;
	do {
		removed=false;
		typedef CGAL::Polyhedron3::Halfedge_iterator HalfedgeIterator;
		for(HalfedgeIterator h=p.halfedges_begin(); h!=p.halfedges_end(); ++h) {
			if(getLength(h)==0.0) {
				removeShortEdge(p,h);
				removed=true;
				break;
			}
		}

	} while(removed);
}

void CGALPrimitive::buildPrimitive()
{
	if(nefPolyhedron)
		return;

	switch(type) {

		case Primitive::Volume: {
			nefPolyhedron=createVolume();
			return;
		}

		case Primitive::Surface: {
			if(!sanitized && detectHoles(true))
				triangulate();
			nefPolyhedron=createVolume();
			return;
		}

		case Primitive::Lines: {
			nefPolyhedron=createPolyline();
			return;
		}

		default: {
			nefPolyhedron=createPoints();
			return;
		}
	}
}

CGAL::NefPolyhedron3* CGALPrimitive::createVolume()
{
	CGALBuilder b(*this);
	CGAL::Polyhedron3 poly;
	poly.delegate(b);

	if(!sanitized) {
		fixZero(poly);
		fixZeroEdges(poly);
		fixZeroTriangles(poly);
	}
	setSanitized(true);
	return new CGAL::NefPolyhedron3(poly);
}

CGAL::NefPolyhedron3* CGALPrimitive::createPolyline()
{
	auto* result=new CGAL::NefPolyhedron3();
	for(CGALPolygon* pg: polygons) {
		QVector<CGAL::Point3> pl=pg->getPoints().toVector();
		CGAL::NefPolyhedron3* np=createPolyline(pl);
		*result=result->join(*np);
		delete np;
	}

	return result;
}

CGAL::NefPolyhedron3* CGALPrimitive::createPolyline(QVector<CGAL::Point3> pl)
{
	typedef QPair<CGAL::Point3*,CGAL::Point3*>  PointRange;
	typedef QList<PointRange> PolyLine;

	PointRange p(pl.begin(),pl.end());
	PolyLine poly;
	poly.push_back(p);
	return new CGAL::NefPolyhedron3(poly.begin(), poly.end(), CGAL::NefPolyhedron3::Polylines_tag());
}

CGAL::NefPolyhedron3* CGALPrimitive::createPoints()
{
#if CGAL_VERSION_NR < CGAL_VERSION_NUMBER(4,8,0)
	CGAL::NefPolyhedron3* result=nullptr;
	QVector<CGAL::Point3> pl1,pl2;
	CGAL::Point3 p1=CGAL::Point3(0.0,0.0,0.0);
	CGAL::Point3 p2=CGAL::Point3(1.0,0.0,0.0);
	CGAL::Point3 p3=CGAL::Point3(0.0,1.0,0.0);

	pl1.append(p1);
	pl1.append(p2);
	const CGAL::NefPolyhedron3* np1=createPolyline(pl1);

	pl2.append(p1);
	pl2.append(p3);
	const CGAL::NefPolyhedron3* np2=createPolyline(pl2);

	const CGAL::NefPolyhedron3& sp=np1->intersection(*np2);
	delete np1;
	delete np2;

	for(CGAL::Point3 p: points) {
		CGAL::NefPolyhedron3 np(sp);
		CGAL::AffTransformation3 t(
			1, 0, 0, p.x(),
			0, 1, 0, p.y(),
			0, 0, 1, p.z(), 1);
		np.transform(t);
		if(!result)
			result=new CGAL::NefPolyhedron3(np);
		else
			*result=result->join(np);
	}
	return result;
#else
	return new CGAL::NefPolyhedron3(points.begin(), points.end(), CGAL::NefPolyhedron3::Points_tag());
#endif
}

Polygon* CGALPrimitive::createPolygon()
{
	return createCGALPolygon();
}

CGALPolygon* CGALPrimitive::createCGALPolygon()
{
	auto* pg = new CGALPolygon(*this);
	polygons.append(pg);
	return pg;
}

void CGALPrimitive::createVertex(const CGAL::Scalar& x,const CGAL::Scalar& y,const CGAL::Scalar& z)
{
	CGAL::Point3 p(x,y,z);
	createVertex(p);
}

void CGALPrimitive::createVertex(const CGAL::Point3& p)
{
	points.append(p);
}

int CGALPrimitive::findIndex(const CGAL::Point3& p)
{
	/* Using pointMap.find allows to check whether the map contains the value
	 * whilst also providing a way to access it instead of doing two lookups */
	const auto& it=pointMap.find(p);
	if(it!=pointMap.end()) {
		return *it;
	} else {
		int i=points.size();
		pointMap.insert(p,i);
		createVertex(p);
		return i;
	}
}

void CGALPrimitive::addVertex(const CGAL::Point3& p,bool direction)
{
	if(!polygons.isEmpty()) {

		int i = findIndex(p);
		CGALPolygon* l=polygons.last();
		if(direction)
			l->append(i);
		else
			l->prepend(i);
	}
}

void CGALPrimitive::appendVertex(const CGAL::Point3& p)
{
	addVertex(p,true);
}

bool CGALPrimitive::overlaps(Primitive* pr)
{
	auto* that=static_cast<CGALPrimitive*>(pr);
	return CGAL::do_intersect(this->getBounds(),that->getBounds());
}


Primitive* CGALPrimitive::group(Primitive* pr)
{
	QList<Primitive*> primitives;
	primitives.append(this);
	primitives.append(pr);

	auto* cp=new CGALPrimitive();
	for(Primitive* pr: primitives) {
		auto* prim=static_cast<CGALPrimitive*>(pr);
		if(prim->nefPolyhedron) {
			CGALExplorer e(prim);
			prim=e.getPrimitive();
		}
		for(CGALPolygon* p: prim->getCGALPolygons()) {
			cp->createPolygon();
			for(const auto& pt: p->getPoints()) {
				cp->appendVertex(pt);
			}
		}
	}
	return cp;
}

QList<CGALPolygon*> CGALPrimitive::getCGALPolygons() const
{
	return polygons;
}

QList<CGAL::Point3> CGALPrimitive::getPoints() const
{
	return points;
}

CGAL::Cuboid3 CGALPrimitive::getBounds()
{
	if(nefPolyhedron) {
		CGALExplorer e(this);
		return e.getBounds();
	}

	return CGAL::bounding_box(points.begin(),points.end());
}

void CGALPrimitive::add(Primitive* pr,bool force)
{
	if(!nUnion) {
		nUnion=new CGAL::Nef_nary_union_3<Unionable>();
		nUnion->add_polyhedron(Unionable(this,force));
	}
	nUnion->add_polyhedron(Unionable(pr,force));
}

Primitive* CGALPrimitive::combine()
{
	if(nUnion) {
		const Unionable& un=nUnion->get_union();
		delete nUnion;
		nUnion=nullptr;
		return un.primitive;
	}
	return this;
}

Primitive* CGALPrimitive::join(Primitive* pr)
{
	auto* that=dynamic_cast<CGALPrimitive*>(pr);
	if(!that) {
		pr->appendChild(this);
		return pr;
	}
	this->buildPrimitive();
	that->buildPrimitive();
	*nefPolyhedron=nefPolyhedron->join(*that->nefPolyhedron);
	this->appendChild(that);
	return this;
}

Primitive* CGALPrimitive::intersection(Primitive* pr)
{
	auto* that=dynamic_cast<CGALPrimitive*>(pr);
	if(!that) {
		pr->appendChild(this);
		return pr;
	}
	this->buildPrimitive();
	that->buildPrimitive();
	*nefPolyhedron=nefPolyhedron->intersection(*that->nefPolyhedron);
	this->appendChild(that);
	return this;
}

Primitive* CGALPrimitive::difference(Primitive* pr)
{
	auto* that=dynamic_cast<CGALPrimitive*>(pr);
	if(!that) {
		pr->appendChild(this);
		return pr;
	}
	this->buildPrimitive();
	that->buildPrimitive();
	*nefPolyhedron=nefPolyhedron->difference(*that->nefPolyhedron);
	this->appendChild(that);
	return this;
}

Primitive* CGALPrimitive::symmetric_difference(Primitive* pr)
{
	auto* that=dynamic_cast<CGALPrimitive*>(pr);
	if(!that) {
		pr->appendChild(this);
		return pr;
	}
	this->buildPrimitive();
	that->buildPrimitive();
	*nefPolyhedron=nefPolyhedron->symmetric_difference(*that->nefPolyhedron);
	this->appendChild(that);
	return this;
}

Primitive* CGALPrimitive::minkowski(Primitive* pr)
{
	auto* that=dynamic_cast<CGALPrimitive*>(pr);
	if(!that) {
		pr->appendChild(this);
		return pr;
	}
	this->buildPrimitive();
	that->buildPrimitive();
	*nefPolyhedron=CGAL::minkowski_sum_3(*nefPolyhedron,*that->nefPolyhedron);
	this->appendChild(that);
	return this;
}

Primitive* CGALPrimitive::inset(const CGAL::Scalar& amount)
{
	CGALBuilder b(*this);
	b.buildOffsetPolygons(amount);
	return this;
}

Primitive* CGALPrimitive::decompose()
{
	this->buildPrimitive();
	CGAL::convex_decomposition_3(*nefPolyhedron);

	typedef CGAL::NefPolyhedron3::Volume_const_iterator VolumeIterator;
	// the first volume is the outer volume, which is
	// ignored in the decomposition
	VolumeIterator ci;
	for(ci = ++nefPolyhedron->volumes_begin(); ci!=nefPolyhedron->volumes_end(); ++ci) {
		if(ci->mark()) {
			CGAL::Polyhedron3 p;
			nefPolyhedron->convert_inner_shell_to_polyhedron(ci->shells_begin(), p);
			children.append(new CGALPrimitive(p));
		}
	}
	return this;
}

Primitive* CGALPrimitive::complement()
{
	this->buildPrimitive();
	*nefPolyhedron=nefPolyhedron->complement();
	return this;
}

Primitive* CGALPrimitive::boundary()
{
	this->buildPrimitive();
	*nefPolyhedron=nefPolyhedron->boundary();
	return this;
}

bool CGALPrimitive::detectHoles(bool check)
{
	for(auto* pg1: polygons) {
		for(auto* pg2: polygons) {
			if(pg1==pg2) continue;
			if(!pg1->sameProjection(pg2)) continue;

			QList<CGAL::Point2> p2=pg2->getProjectedPoints();
			for(auto& p1: pg1->getProjectedPoints()) {
				CGAL::Bounded_side side=CGAL::bounded_side_2(p2.begin(),p2.end(),p1);
				if(side==CGAL::ON_BOUNDED_SIDE) {
					if(check && pg1->getPlane()==pg2->getPlane().opposite())
						return true;
					pg1->setHole(true);
					break;
				}
			}
		}
	}
	return false;
}

Primitive* CGALPrimitive::triangulate()
{
	CGALBuilder b(*this);
	if(b.triangulate())
		return this;
	return nullptr;
}

#ifndef USE_SIMPLIFY
Primitive* CGALPrimitive::simplify(const CGAL::Scalar&)
{
	return this;
}
#else
Primitive* CGALPrimitive::simplify(const CGAL::Scalar& ratio)
{

	namespace SMS=CGAL::Surface_mesh_simplification;
	CGAL::Polyhedron3& p=*getPolyhedron();
	SMS::Count_ratio_stop_predicate<CGAL::Polyhedron3> stop(to_double(ratio));
	SMS::edge_collapse(p,stop,
#if CGAL_VERSION_NR >= CGAL_VERSION_NUMBER(4,7,0)
					   CGAL::parameters::vertex_index_map(get(CGAL::vertex_external_index,p))
#else
					   CGAL::vertex_index_map(get(CGAL::vertex_external_index,p))
#endif
					   .halfedge_index_map(get(CGAL::halfedge_external_index,p))
					   .get_cost(SMS::Edge_length_cost<CGAL::Polyhedron3>())
					   .get_placement(SMS::Midpoint_placement<CGAL::Polyhedron3>()));
	return new CGALPrimitive(p);
}
#endif

Primitive* CGALPrimitive::copy()
{
	auto* p=new CGALPrimitive();
	this->buildPrimitive();
	p->nefPolyhedron=new CGAL::NefPolyhedron3(*nefPolyhedron);
	return p;
}


void CGALPrimitive::transform(TransformMatrix* matrix)
{
	if(!matrix) return;

	CGAL::AffTransformation3 t=matrix->getTransform();
	if(nefPolyhedron) {
		nefPolyhedron->transform(t);
	} else {
		QList<CGAL::Point3> nps;
		for(const auto& pt: points)
			nps.append(pt.transform(t));
		points=nps;
	}

	for(Primitive* p: children)
		p->transform(matrix);
}

QList<Polygon*> CGALPrimitive::getPolygons() const
{
	return QList<Polygon*>();
}

const CGAL::NefPolyhedron3& CGALPrimitive::getNefPolyhedron()
{
	this->buildPrimitive();
	return *nefPolyhedron;
}

CGAL::Polyhedron3* CGALPrimitive::getPolyhedron()
{
	this->buildPrimitive();
	CGAL::Polyhedron3* poly = new CGAL::Polyhedron3();
	nefPolyhedron->convert_to_polyhedron(*poly);
	return poly;
}

bool CGALPrimitive::isEmpty()
{
	this->buildPrimitive();
	return nefPolyhedron->is_empty();
}

CGAL::Circle3 CGALPrimitive::getRadius()
{
	QList<CGAL::Point3> points3;
	if(nefPolyhedron) {
		CGALExplorer e(this);
		points3=e.getPoints();
	} else {
		points3=points;
	}

	typedef  CGAL::Min_circle_2_traits_2<CGAL::Kernel3> Traits;
	typedef  CGAL::Min_circle_2<Traits> Min_circle;

	QList<CGAL::Point2> points2;
	for(const auto& pt3: points3) {
		CGAL::Point2 pt2(pt3.x(),pt3.y());
		points2.append(pt2);
	}

	Min_circle mc2(points2.begin(),points2.end(),true);
	Min_circle::Circle circle2=mc2.circle();
	CGAL::Point2 center2=circle2.center();
	CGAL::Point3 center3(center2.x(),center2.y(),0);
	CGAL::Scalar sq_r=circle2.squared_radius();

	return CGAL::Circle3(center3,sq_r,CGAL::Vector3(0,0,1));

}

CGALVolume CGALPrimitive::getVolume(bool calcMass)
{
	this->buildPrimitive();
	Primitive* p=copy();
	p=p->decompose();

	CGALExplorer e(p);
	return e.getVolume(calcMass);
}

bool CGALPrimitive::isFullyDimentional()
{
	this->buildPrimitive();
	//For fully dimentional polyhedra there are always two volumes the outer
	//volume and the inner volume. So check volumes > 1
	return nefPolyhedron->number_of_volumes()>1;
}

QList<Primitive*> CGALPrimitive::getChildren()
{
	return children;
}

void CGALPrimitive::appendChild(Primitive* p)
{
	if(p!=this)
		children.append(p);
}

static CGAL::Point3 discretePoint(const CGAL::Point3& pt,int places)
{
	CGAL::Scalar x,y,z;
	x=r_round(pt.x(),places);
	y=r_round(pt.y(),places);
	z=r_round(pt.z(),places);
	return CGAL::Point3(x,y,z);
}

class DiscreteNef : public CGAL::NefPolyhedron3
{
public:
	void discrete(int places)
	{
		Vertex_iterator v;
		CGAL_forall_vertices(v,snc()) {
			v->point()=discretePoint(v->point(),places);
		}
	}
};

void CGALPrimitive::discrete(int places)
{
	if(nefPolyhedron) {
		auto* n=static_cast<DiscreteNef*>(nefPolyhedron);
		n->discrete(places);
	} else {
		QList<CGAL::Point3> nps;
		for(const auto& pt: points) {
			nps.append(discretePoint(pt,places));
		}
		points=nps;
	}
}

CGALPrimitive::Unionable& CGALPrimitive::Unionable::operator+(Unionable& other)
{
	if(force||primitive->overlaps(other.primitive))
		primitive=primitive->join(other.primitive);
	else
		primitive=primitive->group(other.primitive);

	return *this;
}
#endif
