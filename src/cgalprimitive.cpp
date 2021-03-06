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
#include "cgalprimitive.h"
#include <QPair>
#include <CGAL/minkowski_sum_3.h>
#include <CGAL/convex_decomposition_3.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/bounding_box.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <CGAL/Nef_3/Mark_bounded_volumes.h>

#if CGAL_VERSION_NR >= CGAL_VERSION_NUMBER(4,12,2)
#include <CGAL/boost/graph/convert_nef_polyhedron_to_polygon_mesh.h>
#endif

//Mesh simplification
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_ratio_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_length_cost.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Midpoint_placement.h>

#include "cgalbuilder.h"
#include "cgalsanitizer.h"
#include "cgalexplorer.h"
#include "cgaldiscretemodifier.h"
#include "cgalgroupmodifier.h"
#include "onceonly.h"
#include "rmath.h"

CGALPrimitive::CGALPrimitive() :
	nefPolyhedron(nullptr),
	type(PrimitiveTypes::Volume),
	sanitized(true),
	nUnion(nullptr)
{
}

CGALPrimitive::CGALPrimitive(const CGAL::Polyhedron3& poly) : CGALPrimitive()
{
	nefPolyhedron=new CGAL::NefPolyhedron3(const_cast<CGAL::Polyhedron3&>(poly));
}

CGALPrimitive::CGALPrimitive(const CGAL::NefPolyhedron3& nef) : CGALPrimitive()
{
	nefPolyhedron=new CGAL::NefPolyhedron3(nef);
}

CGALPrimitive::~CGALPrimitive()
{
	delete nefPolyhedron;
	nefPolyhedron=nullptr;

	clearPolygons();
	qDeleteAll(perimeters);
	perimeters.clear();

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

void CGALPrimitive::setType(PrimitiveTypes t)
{
	type=t;
}

PrimitiveTypes CGALPrimitive::getType()
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

void CGALPrimitive::buildPrimitive()
{
	if(nefPolyhedron)
		return;

	switch(type) {

		case PrimitiveTypes::Volume: {
			nefPolyhedron=createVolume();
			return;
		}

		case PrimitiveTypes::Surface: {
			if(!sanitized && hasHoles())
				triangulate();
			nefPolyhedron=createVolume();
			return;
		}

		case PrimitiveTypes::Lines: {
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
		CGALSanitizer s(poly);
		s.sanitize();
	}
	setSanitized(true);

	if(poly.empty())
		return new CGAL::NefPolyhedron3();

	return new CGAL::NefPolyhedron3(poly);
}

static bool connected(const CGAL::Segment3& a,const CGAL::Segment3& b)
{
	return (a.source()==b.source()||a.source()==b.target()||a.target()==b.source()||a.target()==b.target());
}

static bool validPolyLine(const QList<CGAL::Segment3>& segments)
{
	for(const auto& segment: segments)
		for(const auto& other: segments)
			if(!segment.identical(other) && !connected(segment,other) && do_intersect(segment,other))
				return false;

	return true;
}

CGAL::NefPolyhedron3* CGALPrimitive::createPolyline()
{
	if(polygons.isEmpty())
		return new CGAL::NefPolyhedron3();

	CGAL::NefPolyhedron3* result=nullptr;
	for(CGALPolygon* pg: polygons) {
		if(!sanitized) {
			auto segments=pg->getSegments();
			if(!validPolyLine(segments)) {
				for(const auto& segment: segments) {
					if(!result) {
						result=createPolyline(segment);
					} else {
						auto* np=createPolyline(segment);
						*result=result->join(*np);
						delete np;
					}
				}
			}
		}

		if(!result) {
			result=createPolyline(pg);
		} else {
			auto* np=createPolyline(pg);
			*result=result->join(*np);
			delete np;
		}
	}
	if(!result)
		return new CGAL::NefPolyhedron3();

	setSanitized(true);
	return result;
}

CGAL::NefPolyhedron3* CGALPrimitive::createPolyline(CGALPolygon* pg)
{
	return createPolyline(pg->getPoints().toVector());
}

CGAL::NefPolyhedron3* CGALPrimitive::createPolyline(const CGAL::Segment3& s)
{
	QVector<CGAL::Point3> pl;
	pl.append(s.source());
	pl.append(s.target());
	return createPolyline(pl);
}

CGAL::NefPolyhedron3* CGALPrimitive::createPolyline(QVector<CGAL::Point3> pl)
{
	using PointRange = QPair<CGAL::Point3*,CGAL::Point3*>;
	using PolyLine = QList<PointRange>;

	PointRange p(pl.begin(),pl.end());
	PolyLine poly;
	poly.push_back(p);
	return new CGAL::NefPolyhedron3(poly.begin(), poly.end(), CGAL::NefPolyhedron3::Polylines_tag());
}

CGAL::NefPolyhedron3* CGALPrimitive::createPoints()
{
#if CGAL_VERSION_NR < CGAL_VERSION_NUMBER(4,8,0)
	using PointRange = QPair<CGAL::Point3*,CGAL::Point3*>;
	using PolyLine = QList<PointRange>;

	CGAL::NefPolyhedron3* result=nullptr;
	QVector<CGAL::Point3> pl1,pl2;
	CGAL::Point3 p1=CGAL::Point3(0.0,0.0,0.0);
	CGAL::Point3 p2=CGAL::Point3(1.0,0.0,0.0);
	CGAL::Point3 p3=CGAL::Point3(0.0,1.0,0.0);

	pl1.append(p1);
	pl1.append(p2);
	PointRange pr1(pl1.begin(),pl1.end());
	PolyLine poly1;
	poly1.push_back(pr1);
	const CGAL::NefPolyhedron3* np1=new CGAL::NefPolyhedron3(poly1.begin(), poly1.end(), CGAL::NefPolyhedron3::Polylines_tag());

	pl2.append(p1);
	pl2.append(p3);
	PointRange pr2(pl2.begin(),pl2.end());
	PolyLine poly2;
	poly2.push_back(pr2);
	const CGAL::NefPolyhedron3* np2=new CGAL::NefPolyhedron3(poly2.begin(), poly2.end(), CGAL::NefPolyhedron3::Polylines_tag());

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

CGALPolygon& CGALPrimitive::createPolygon()
{
	auto* pg = new CGALPolygon(*this);
	polygons.append(pg);
	return *pg;
}

CGALPolygon& CGALPrimitive::createPerimeter()
{
	auto* pg = new CGALPolygon(*this);
	perimeters.append(pg);
	return *pg;
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
	if(it!=pointMap.end()) return *it;

	int i=points.size();
	pointMap.insert(p,i);
	createVertex(p);
	return i;
}

void CGALPrimitive::addVertex(const CGAL::Point3& p,bool direction)
{
	if(!polygons.isEmpty())
		addVertex(polygons.last(),p,direction);
}

void CGALPrimitive::addVertex(CGALPolygon* pg,const CGAL::Point3& p,bool direction)
{
	int i = findIndex(p);
	if(direction)
		pg->append(i);
	else
		pg->prepend(i);
}

void CGALPrimitive::appendVertex(const CGAL::Point3& p)
{
	if(!polygons.isEmpty())
		addVertex(polygons.last(),p,true);
}

bool CGALPrimitive::overlaps(Primitive* pr)
{
	auto* that=dynamic_cast<CGALPrimitive*>(pr);
	if(!that) return false;
	return CGAL::do_intersect(this->getBounds(),that->getBounds());
}

Primitive* CGALPrimitive::group(Primitive* pr)
{
	if(!pr) return this;
	auto* that=dynamic_cast<CGALPrimitive*>(pr);
	if(!that) {
		pr->appendChild(this);
		return pr;
	}
	this->buildPrimitive();
	that->buildPrimitive();

	CGALGroupModifier m(*that->nefPolyhedron);
	nefPolyhedron->delegate(m,true,false);

	CGAL::Mark_bounded_volumes<CGAL::NefPolyhedron3::SNC_structure> mbv(true);
	nefPolyhedron->delegate(mbv,false,false);

	this->appendChild(that);
	return this;

}

QList<CGALPolygon*> CGALPrimitive::getCGALPolygons() const
{
	return polygons;
}

QList<CGALPolygon*> CGALPrimitive::getCGALPerimeter() const
{
	return perimeters;
}

QList<CGAL::Point3> CGALPrimitive::getPoints() const
{
	if(!nefPolyhedron)
		return points;

	QList<CGAL::Point3> pts;
	CGAL::NefPolyhedron3::Vertex_const_iterator v;
	CGAL_forall_vertices(v, *nefPolyhedron->sncp())
		pts.append(v->point());

	return pts;
}

CGAL::Cuboid3 CGALPrimitive::getBounds()
{
	QList<CGAL::Point3> pts=getPoints();
	return CGAL::bounding_box(pts.begin(),pts.end());
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
	if(!pr) return this;
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
	if(!pr) return this;
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
	if(!pr) return this;
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
	if(!pr) return this;
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
	if(!pr) return this;
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
	CGALPrimitive* offset=b.buildOffset(amount);
	offset->appendChild(this);
	return offset;
}

Primitive* CGALPrimitive::decompose()
{
	this->buildPrimitive();
	CGAL::convex_decomposition_3(*nefPolyhedron);

	using VolumeIterator = CGAL::NefPolyhedron3::Volume_const_iterator;
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
	if(isFullyDimentional()) {
		this->buildPrimitive();
		*nefPolyhedron=nefPolyhedron->boundary();
	} else {
		CGALExplorer explorer(this);
		CGALPrimitive* primitive=explorer.getPrimitive();
		primitive->convertBoundary();
		primitive->appendChild(this);
		return primitive;
	}
	return this;
}

void CGALPrimitive::convertBoundary()
{
	setType(PrimitiveTypes::Lines);
	polygons=perimeters;
	perimeters.clear();
}

void CGALPrimitive::detectPerimeterHoles()
{
	detectHoles(perimeters,false);
}

bool CGALPrimitive::hasHoles()
{
	return detectHoles(polygons,true);
}

bool CGALPrimitive::detectHoles(QList<CGALPolygon*> polys,bool check)
{
	for(auto* pg1: polys) {
		for(auto* pg2: polys) {
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
	if(b.triangulate()) {
		setType(PrimitiveTypes::Surface);
		return this;
	}

	auto* cp=new CGALPrimitive();
	cp->appendChild(this);
	return cp;
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
	CGAL::Polyhedron3* p=getPolyhedron();
	SMS::Count_ratio_stop_predicate<CGAL::Polyhedron3> stop(to_double(ratio));
	SMS::edge_collapse(*p,stop,
#if CGAL_VERSION_NR >= CGAL_VERSION_NUMBER(4,7,0)
					   CGAL::parameters::vertex_index_map(get(CGAL::vertex_external_index,*p))
#else
					   CGAL::vertex_index_map(get(CGAL::vertex_external_index,p))
#endif
					   .halfedge_index_map(get(CGAL::halfedge_external_index,*p))
					   .get_cost(SMS::Edge_length_cost<CGAL::Polyhedron3>())
					   .get_placement(SMS::Midpoint_placement<CGAL::Polyhedron3>()));
	auto* cp=new CGALPrimitive(*p);
	cp->appendChild(this);
	delete p;
	return cp;
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
	auto* poly = new CGAL::Polyhedron3();
#if CGAL_VERSION_NR >= CGAL_VERSION_NUMBER(4,12,2)
	CGAL::convert_nef_polyhedron_to_polygon_mesh(*nefPolyhedron,*poly,true);
#else
	nefPolyhedron->convert_to_polyhedron(*poly);
#endif
	return poly;
}

bool CGALPrimitive::isEmpty()
{
	this->buildPrimitive();
	return nefPolyhedron->is_empty();
}

CGAL::Circle3 CGALPrimitive::getRadius()
{
	using Traits = CGAL::Min_circle_2_traits_2<CGAL::Kernel3>;
	using Min_circle = CGAL::Min_circle_2<Traits>;

	QList<CGAL::Point3> points3=getPoints();
	QList<CGAL::Point2> points2;
	for(const auto& pt3: points3) {
		CGAL::Point2 pt2(pt3.x(),pt3.y());
		points2.append(pt2);
	}

	Min_circle mc2(points2.begin(),points2.end(),true);
	const Min_circle::Circle& circle2=mc2.circle();
	const CGAL::Point2& center2=circle2.center();
	CGAL::Point3 center3(center2.x(),center2.y(),0);
	const CGAL::Scalar& sq_r=circle2.squared_radius();

	return CGAL::Circle3(center3,sq_r,CGAL::Vector3(0,0,1));

}

CGALVolume CGALPrimitive::getVolume(bool calcMass)
{
	this->buildPrimitive();
	Primitive* p=copy();
	p=p->decompose();

	CGALExplorer e(p);
	CGALVolume v=e.getVolume(calcMass);
	delete e.getPrimitive();
	delete p;
	return v;
}

bool CGALPrimitive::isFullyDimentional()
{
	if(getType() != PrimitiveTypes::Volume)
		return false;

	this->buildPrimitive();
	//For fully dimentional polyhedra there are always two volumes the outer
	//volume and the inner volume. So check volumes > 1
	return nefPolyhedron->number_of_volumes()>1 && nefPolyhedron->number_of_facets()>3;
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

void CGALPrimitive::appendChildren(QList<Primitive*> p)
{
	children.append(p);
}

void CGALPrimitive::discrete(int places)
{
	if(nefPolyhedron) {
		CGALDiscreteModifier n(places);
		nefPolyhedron->delegate(n,false,false);
	} else {
		QList<CGAL::Point3> nps;
		for(const auto& pt: points) {
			nps.append(CGALDiscreteModifier::discretePoint(pt,places));
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
