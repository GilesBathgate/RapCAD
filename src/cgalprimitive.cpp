#if USE_CGAL
#include "cgalprimitive.h"
#include <QPair>
#include <CGAL/minkowski_sum_3.h>
#include "cgalbuilder.h"

CGALPrimitive::CGALPrimitive()
{
}

CGALPrimitive::CGALPrimitive(QVector<CGAL::Point3> pl)
{
	typedef QPair<CGAL::Point3*,CGAL::Point3*>  PointRange;
	typedef QList<PointRange> PolyLine;

	PointRange p(pl.begin(),pl.end());
	PolyLine poly;
	poly.push_back(p);
	nefPolyhedron=new CGAL::NefPolyhedron3(poly.begin(), poly.end(), CGAL::NefPolyhedron3::Polylines_tag());
}

CGALPrimitive::CGALPrimitive(CGAL::Polyhedron3 poly)
{
	nefPolyhedron=new CGAL::NefPolyhedron3(poly);
}

CGALPrimitive* CGALPrimitive::buildVolume()
{
	CGALBuilder b(this);
	CGAL::Polyhedron3 poly;
	poly.delegate(b);
	nefPolyhedron=new CGAL::NefPolyhedron3(poly);
	return this;
}

CGALPolygon* CGALPrimitive::createPolygon()
{
	CGALPolygon* pg = new CGALPolygon();
	polygons.append(pg);
	return pg;
}

void CGALPrimitive::appendVertex(CGAL::Point3 p)
{
	if(!points.contains(p))
		points.append(p);
	polygons.last()->append(p);
}

void CGALPrimitive::prependVertex(CGAL::Point3 p)
{
	if(!points.contains(p))
		points.append(p);
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

CGALPrimitive* CGALPrimitive::join(const CGALPrimitive* that)
{
	*nefPolyhedron=nefPolyhedron->join(*that->nefPolyhedron);
	return this;
}

CGALPrimitive* CGALPrimitive::intersection(const CGALPrimitive* that)
{
	*nefPolyhedron=nefPolyhedron->intersection(*that->nefPolyhedron);
	return this;
}

CGALPrimitive* CGALPrimitive::difference(const CGALPrimitive* that)
{
	*nefPolyhedron=nefPolyhedron->difference(*that->nefPolyhedron);
	return this;
}

CGALPrimitive* CGALPrimitive::symmetric_difference(const CGALPrimitive* that)
{
	*nefPolyhedron=nefPolyhedron->symmetric_difference(*that->nefPolyhedron);
	return this;
}

CGALPrimitive* CGALPrimitive::minkowski(const CGALPrimitive* that)
{
	*nefPolyhedron=CGAL::minkowski_sum_3(*nefPolyhedron,*that->nefPolyhedron);
	return this;
}

CGALPrimitive* CGALPrimitive::inset(double amount)
{
	CGALBuilder b(this);
	CGALPrimitive* result=b.buildOffsetPolygons(amount);
	return result->buildVolume();
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

bool CGALPrimitive::isFullyDimentional()
{
	//For fully dimentional polyhedra there are always two volumes the outer
	//volume and the inner volume. So check volumes > 1
	return nefPolyhedron->number_of_volumes()>1;
}
#endif
