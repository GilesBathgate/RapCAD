#if USE_CGAL
#include "cgalprimitive.h"
#include <QPair>
#include <CGAL/minkowski_sum_3.h>
#include <CGAL/convex_decomposition_3.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include "cgalbuilder.h"
#include "cgalexplorer.h"
#include "onceonly.h"

CGAL::NefPolyhedron3* CGALPrimitive::singlePoint=NULL;

void CGALPrimitive::init()
{
	nUnion=NULL;
	type=Primitive::Volume;
}

CGALPrimitive::CGALPrimitive()
{
	init();
	nefPolyhedron=NULL;
}

CGALPrimitive::CGALPrimitive(CGAL::Polyhedron3 poly)
{
	init();
	nefPolyhedron=new CGAL::NefPolyhedron3(poly);
}

void CGALPrimitive::setType(Primitive_t t)
{
	type=t;
}

void CGALPrimitive::buildPrimitive()
{
	if(nefPolyhedron)
		return;

	switch(type) {
	case Primitive::Volume: {
		CGALBuilder b(this);
		CGAL::Polyhedron3 poly;
		poly.delegate(b);
		nefPolyhedron=new CGAL::NefPolyhedron3(poly);
		return;
	}

	case Primitive::Skeleton: {
		OnceOnly first;
		foreach(CGALPolygon* p,polygons) {
			QVector<CGAL::Point3> pl;
			foreach(CGAL::Point3 pt, p->getPoints()) {
				pl.append(pt);
			}

			if(first()) {
				nefPolyhedron=createPolyline(pl);
			} else {
				const CGAL::NefPolyhedron3* np=createPolyline(pl);
				*nefPolyhedron=nefPolyhedron->join(*np);
			}
		}
		return;
	}

	default: {
		if(!singlePoint) {
			QVector<CGAL::Point3> pl1,pl2;
			CGAL::Point3 p1=CGAL::Point3(0.0,0.0,0.0);
			CGAL::Point3 p2=CGAL::Point3(1.0,0.0,0.0);
			CGAL::Point3 p3=CGAL::Point3(0.0,1.0,0.0);

			pl1.append(p1);
			pl1.append(p2);
			singlePoint=createPolyline(pl1);

			pl2.append(p1);
			pl2.append(p3);
			const CGAL::NefPolyhedron3* np=createPolyline(pl2);

			*singlePoint=singlePoint->intersection(*np);
		}

		CGAL::Point3 p=points.last();

		nefPolyhedron=new CGAL::NefPolyhedron3(*singlePoint);

		CGAL::AffTransformation3 t(
			1, 0, 0, p.x(),
			0, 1, 0, p.y(),
			0, 0, 1, p.z(), 1);
		nefPolyhedron->transform(t);
		return;
	}
	}
	nefPolyhedron=new CGAL::NefPolyhedron3();
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

Polygon* CGALPrimitive::createPolygon()
{
	CGALPolygon* pg = new CGALPolygon(this);
	polygons.append(pg);
	return pg;
}

void CGALPrimitive::createVertex(Point pt)
{
	decimal x,y,z;
	pt.getXYZ(x,y,z);
	CGAL::Point3 p(x,y,z);
	points.append(p);
}

void CGALPrimitive::appendVertex(Point)
{
}

void CGALPrimitive::appendVertex(CGAL::Point3 p)
{
	if(!polygons.isEmpty()) {
		if(!points.contains(p))
			points.append(p);

		int i=points.indexOf(p);
		polygons.last()->append(i);
	}
}

void CGALPrimitive::prependVertex(Point)
{
}

void CGALPrimitive::prependVertex(CGAL::Point3 p)
{
	if(!polygons.isEmpty()) {
		if(!points.contains(p))
			points.append(p);

		int i=points.indexOf(p);
		polygons.last()->prepend(i);
	}
}

/* I don't know why this function doesn't exist in CGAL?
 * The CGAL::do_overlap only takes BBox_3 as arguments. */
namespace CGAL {
template < class R >
inline
bool
do_overlap(const CGAL::Iso_cuboid_3<R>& ic1, const CGAL::Iso_cuboid_3<R>& ic2)
{
	if(ic1.xmax() < ic2.xmin() || ic2.xmax() < ic1.xmin())
		return false;
	if(ic1.ymax() < ic2.ymin() || ic2.ymax() < ic1.ymin())
		return false;
	if(ic1.zmax() < ic2.zmin() || ic2.zmax() < ic1.zmin())
		return false;
	return true;
}
}

bool CGALPrimitive::overlaps(Primitive* pr)
{
	CGALPrimitive* that=static_cast<CGALPrimitive*>(pr);
	return CGAL::do_overlap(this->getBounds(),that->getBounds());
}


Primitive* CGALPrimitive::group(Primitive* pr)
{
	QList<Primitive*> primitives;
	primitives.append(this);
	primitives.append(pr);

	CGALPrimitive* cp=new CGALPrimitive();
	foreach(Primitive* pr, primitives) {
		CGALPrimitive* prim=static_cast<CGALPrimitive*>(pr);
		if(prim->nefPolyhedron) {
			CGALExplorer e(prim);
			prim=e.getPrimitive();
		}
		foreach(CGALPolygon* p, prim->getCGALPolygons()) {
			cp->createPolygon();
			foreach(CGAL::Point3 pt, p->getPoints()) {
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

QList<Point> CGALPrimitive::getPoints() const
{
	return QList<Point>();
}

QList<CGAL::Point3> CGALPrimitive::getCGALPoints() const
{
	return points;
}

CGAL::Cuboid3 CGALPrimitive::getBounds()
{
	if(nefPolyhedron) {
		CGALExplorer e(this);
		return e.getBounds();
	}

	QList<CGAL::Point3> points=getCGALPoints();
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
		Unionable un=nUnion->get_union();
		return un.primitive;
	}
	return this;
}

Primitive* CGALPrimitive::join(Primitive* pr)
{
	this->buildPrimitive();
	CGALPrimitive* that=static_cast<CGALPrimitive*>(pr);
	that->buildPrimitive();
	*nefPolyhedron=nefPolyhedron->join(*that->nefPolyhedron);
	return this;
}

Primitive* CGALPrimitive::intersection(Primitive* pr)
{
	this->buildPrimitive();
	CGALPrimitive* that=static_cast<CGALPrimitive*>(pr);
	that->buildPrimitive();
	*nefPolyhedron=nefPolyhedron->intersection(*that->nefPolyhedron);
	return this;
}

Primitive* CGALPrimitive::difference(Primitive* pr)
{
	this->buildPrimitive();
	CGALPrimitive* that=static_cast<CGALPrimitive*>(pr);
	that->buildPrimitive();
	*nefPolyhedron=nefPolyhedron->difference(*that->nefPolyhedron);
	return this;
}

Primitive* CGALPrimitive::symmetric_difference(Primitive* pr)
{
	this->buildPrimitive();
	CGALPrimitive* that=static_cast<CGALPrimitive*>(pr);
	that->buildPrimitive();
	*nefPolyhedron=nefPolyhedron->symmetric_difference(*that->nefPolyhedron);
	return this;
}

Primitive* CGALPrimitive::minkowski(Primitive* pr)
{
	this->buildPrimitive();
	CGALPrimitive* that=static_cast<CGALPrimitive*>(pr);
	that->buildPrimitive();
	*nefPolyhedron=CGAL::minkowski_sum_3(*nefPolyhedron,*that->nefPolyhedron);
	return this;
}

Primitive* CGALPrimitive::inset(const decimal amount)
{
	CGALBuilder b(this);
	CGALPrimitive* result=b.buildOffsetPolygons(amount);
	return result;
}

Primitive* CGALPrimitive::decompose()
{
	this->buildPrimitive();
	CGAL::convex_decomposition_3(*nefPolyhedron);
	return this;
}

Primitive* CGALPrimitive::complement()
{
	this->buildPrimitive();
	*nefPolyhedron=nefPolyhedron->complement();
	return this;
}

Primitive* CGALPrimitive::copy()
{
	CGALPrimitive* p=new CGALPrimitive();
	this->buildPrimitive();
	p->nefPolyhedron=new CGAL::NefPolyhedron3(*nefPolyhedron);
	return p;
}


void CGALPrimitive::transform(const CGAL::AffTransformation3& t)
{
	if(nefPolyhedron) {
		nefPolyhedron->transform(t);
	} else {
		QList<CGAL::Point3> nps;
		foreach(CGAL::Point3 pt, points)
			nps.append(pt.transform(t));
		points=nps;
	}
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
	if(nefPolyhedron->is_simple())
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
	QList<CGAL::Point3> points;
	if(nefPolyhedron) {
		CGALExplorer e(this);
		points=e.getPoints();
	} else {
		points=getCGALPoints();
	}

	typedef  CGAL::Min_circle_2_traits_2<CGAL::Kernel3> Traits;
	typedef  CGAL::Min_circle_2<Traits> Min_circle;

	QList<CGAL::Point2> pts;
	foreach(CGAL::Point3 pt,points) {
		CGAL::Point2 p2(pt.x(),pt.y());
		pts.append(p2);
	}

	Min_circle mc1(pts.begin(),pts.end(),true);
	Min_circle::Circle circle2=mc1.circle();
	CGAL::Point2 center2=circle2.center();
	CGAL::Point3 center3(center2.x(),center2.y(),0);
	CGAL::FT sq_r=circle2.squared_radius();

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
	children.append(p);
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
