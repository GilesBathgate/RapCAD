#if USE_CGAL
#include "cgalprimitive.h"
#include <QPair>
#include <CGAL/minkowski_sum_3.h>
#include "cgalbuilder.h"
#include "onceonly.h"

CGALPrimitive::CGALPrimitive()
{
	type=Primitive::Volume;
	nUnion=NULL;
	nefPolyhedron=NULL;
}

CGALPrimitive::CGALPrimitive(CGAL::Polyhedron3 poly)
{
	type=Primitive::Volume;
	nUnion=NULL;
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
			QVector<CGAL::Point3> pl1,pl2;

			QList<CGAL::Point3> points=polygons.last()->getPoints();
			CGAL::Point3 p=points.last();
			CGAL::Point3 p1=CGAL::Point3(p.x()+1,p.y(),p.z());
			CGAL::Point3 p2=CGAL::Point3(p.x(),p.y()+1,p.z());

			pl1.append(p);
			pl1.append(p1);
			nefPolyhedron=createPolyline(pl1);

			pl2.append(p);
			pl2.append(p2);
			const CGAL::NefPolyhedron3* np=createPolyline(pl2);

			*nefPolyhedron=nefPolyhedron->intersection(*np);
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
	if(polygons.count()>0)
		polygons.last()->prepend(p);
}

QList<CGALPolygon*> CGALPrimitive::getPolygons() const
{
	return polygons;
}

QList<CGAL::Point3> CGALPrimitive::getPoints() const
{
	QList<CGAL::Point3> points;
	foreach(CGALPolygon* pg, polygons) {
		foreach(CGAL::Point3 p, pg->getPoints()) {
			if(!points.contains(p)) {
				points.append(p);
			}
		}
	}
	return points;
}

void CGALPrimitive::add(Primitive* pr)
{
	if(!nUnion) {
		nUnion=new CGAL::Nef_nary_union_3<CGAL::NefPolyhedron3>();
		this->buildPrimitive();
		nUnion->add_polyhedron(*nefPolyhedron);
	}
	CGALPrimitive* that=static_cast<CGALPrimitive*>(pr);
	that->buildPrimitive();
	nUnion->add_polyhedron(*that->nefPolyhedron);
}

Primitive* CGALPrimitive::join()
{
	if(nUnion) {
		*nefPolyhedron=nUnion->get_union();
		delete nUnion;
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

Primitive* CGALPrimitive::inset(const double amount)
{
	CGALBuilder b(this);
	CGALPrimitive* result=b.buildOffsetPolygons(amount);
	return result;
}

Primitive* CGALPrimitive::copy()
{
	CGALPrimitive* p=new CGALPrimitive();
	p->nefPolyhedron=new CGAL::NefPolyhedron3(*nefPolyhedron);
	return p;
}


void CGALPrimitive::transform(const CGAL::AffTransformation3& t)
{
	if(nefPolyhedron) {
		nefPolyhedron->transform(t);
	} else {
		foreach(CGALPolygon* pg, polygons) {
			pg->transform(t);
		}
	}
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

bool CGALPrimitive::isFullyDimentional()
{
	this->buildPrimitive();
	//For fully dimentional polyhedra there are always two volumes the outer
	//volume and the inner volume. So check volumes > 1
	return nefPolyhedron->number_of_volumes()>1;
}
#endif
