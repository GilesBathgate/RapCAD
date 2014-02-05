#if USE_CGAL
#include "cgalprimitive.h"
#include <QPair>
#include <CGAL/minkowski_sum_3.h>
#include "cgalbuilder.h"
#include "onceonly.h"

CGALPrimitive::CGALPrimitive()
{
	skeleton=false;
	added=false;
}

CGALPrimitive::CGALPrimitive(CGAL::Polyhedron3 poly)
{
	skeleton=false;
	added=false;
	nefPolyhedron=new CGAL::NefPolyhedron3(poly);
}

void CGALPrimitive::setSkeleton(bool value)
{
	skeleton=value;
}

Primitive* CGALPrimitive::buildPrimitive()
{
	if(!skeleton) {
		CGALBuilder b(this);
		CGAL::Polyhedron3 poly;
		poly.delegate(b);
		nefPolyhedron=new CGAL::NefPolyhedron3(poly);
		return this;
	} else if(points.count()>1) {
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
		return this;

	} else if(points.count()==1) {
		QVector<CGAL::Point3> pl1,pl2;

		CGAL::Point3 p=points[0];
		CGAL::Point3 p1=CGAL::Point3(p.x()+1,p.y(),p.z());
		CGAL::Point3 p2=CGAL::Point3(p.x(),p.y()+1,p.z());

		pl1.append(p);
		pl1.append(p1);
		nefPolyhedron=createPolyline(pl1);

		pl2.append(p);
		pl2.append(p2);
		const CGAL::NefPolyhedron3* np=createPolyline(pl2);

		*nefPolyhedron=nefPolyhedron->intersection(*np);
		return this;
	}

	nefPolyhedron=new CGAL::NefPolyhedron3();
	return this;
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
	CGALPolygon* pg = new CGALPolygon();
	polygons.append(pg);
	return pg;
}

void CGALPrimitive::appendVertex(Point pt)
{
	double x,y,z;
	pt.getXYZ(x,y,z);
	CGAL::Point3 p(x,y,z);
	appendVertex(p);
}

void CGALPrimitive::appendVertex(CGAL::Point3 p)
{
	if(!points.contains(p))
		points.append(p);
	if(polygons.count()>0)
		polygons.last()->append(p);
}

void CGALPrimitive::prependVertex(Point pt)
{
	double x,y,z;
	pt.getXYZ(x,y,z);
	CGAL::Point3 p(x,y,z);
	prependVertex(p);
}

void CGALPrimitive::prependVertex(CGAL::Point3 p)
{
	if(!points.contains(p))
		points.append(p);
	if(polygons.count()>0)
		polygons.last()->prepend(p);
}

QList<CGALPolygon*> CGALPrimitive::getPolygons() const
{
	return polygons;
}

QList<CGAL::Point3> CGALPrimitive::getPoints() const
{
	return points;
}

void CGALPrimitive::add(const Primitive* pr)
{
	if(!added)
		nUnion.add_polyhedron(*nefPolyhedron);

	const CGALPrimitive* that=static_cast<const CGALPrimitive*>(pr);
	nUnion.add_polyhedron(*that->nefPolyhedron);
	added=true;
}

Primitive* CGALPrimitive::join()
{
	if(added)
		*nefPolyhedron=nUnion.get_union();

	return this;
}

Primitive* CGALPrimitive::join(const Primitive* pr)
{
	const CGALPrimitive* that=static_cast<const CGALPrimitive*>(pr);
	*nefPolyhedron=nefPolyhedron->join(*that->nefPolyhedron);
	return this;
}

Primitive* CGALPrimitive::intersection(const Primitive* pr)
{
	const CGALPrimitive* that=static_cast<const CGALPrimitive*>(pr);
	*nefPolyhedron=nefPolyhedron->intersection(*that->nefPolyhedron);
	return this;
}

Primitive* CGALPrimitive::difference(const Primitive* pr)
{
	const CGALPrimitive* that=static_cast<const CGALPrimitive*>(pr);
	*nefPolyhedron=nefPolyhedron->difference(*that->nefPolyhedron);
	return this;
}

Primitive* CGALPrimitive::symmetric_difference(const Primitive* pr)
{
	const CGALPrimitive* that=static_cast<const CGALPrimitive*>(pr);
	*nefPolyhedron=nefPolyhedron->symmetric_difference(*that->nefPolyhedron);
	return this;
}

Primitive* CGALPrimitive::minkowski(const Primitive* pr)
{
	const CGALPrimitive* that=static_cast<const CGALPrimitive*>(pr);
	*nefPolyhedron=CGAL::minkowski_sum_3(*nefPolyhedron,*that->nefPolyhedron);
	return this;
}

Primitive* CGALPrimitive::inset(const double amount)
{
	CGALBuilder b(this);
	CGALPrimitive* result=b.buildOffsetPolygons(amount);
	return result->buildPrimitive();
}

Primitive* CGALPrimitive::copy()
{
	CGALPrimitive* p=new CGALPrimitive();
	p->nefPolyhedron=new CGAL::NefPolyhedron3(*nefPolyhedron);
	return p;
}


void CGALPrimitive::transform(const CGAL::AffTransformation3& t)
{
	nefPolyhedron->transform(t);
}

const CGAL::NefPolyhedron3& CGALPrimitive::getNefPolyhedron() const
{
	return *nefPolyhedron;
}

CGAL::Polyhedron3* CGALPrimitive::getPolyhedron()
{
	CGAL::Polyhedron3* poly = new CGAL::Polyhedron3();
	if(nefPolyhedron->is_simple())
		nefPolyhedron->convert_to_polyhedron(*poly);
	return poly;
}

bool CGALPrimitive::isEmpty()
{
	return nefPolyhedron->is_empty();
}

bool CGALPrimitive::isFullyDimentional()
{
	//For fully dimentional polyhedra there are always two volumes the outer
	//volume and the inner volume. So check volumes > 1
	return nefPolyhedron->number_of_volumes()>1;
}
#endif
