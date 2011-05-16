#include "cgalprimitive.h"
#include <QPair>
#include <CGAL/minkowski_sum_3.h>

CGALPrimitive::CGALPrimitive(PrimitiveNode* n)
{
	this->setPrimitive(n);
	CGAL::Polyhedron3 poly;
	poly.delegate(*this);
	poly3=new CGAL::NefPolyhedron3(poly);
}

CGALPrimitive::CGALPrimitive(QVector<CGAL::Point3> pl)
{
	typedef QPair<CGAL::Point3*,CGAL::Point3*>  PointRange;
	typedef QList<PointRange> PolyLine;

	PointRange p(pl.begin(),pl.end());
	PolyLine poly;
	poly.push_back(p);
	poly3=new CGAL::NefPolyhedron3(poly.begin(), poly.end(), CGAL::NefPolyhedron3::Polylines_tag());
}

CGALPrimitive::CGALPrimitive(CGAL::Polyhedron3 poly)
{
	poly3=new CGAL::NefPolyhedron3(poly);
}

CGALPrimitive* CGALPrimitive::join(const CGALPrimitive* that)
{
	*poly3=poly3->join(*that->poly3);
	return this;
}

CGALPrimitive* CGALPrimitive::intersection(const CGALPrimitive* that)
{
	*poly3=poly3->intersection(*that->poly3);
	return this;
}

CGALPrimitive* CGALPrimitive::difference(const CGALPrimitive* that)
{
	*poly3=poly3->difference(*that->poly3);
	return this;
}

CGALPrimitive* CGALPrimitive::symmetric_difference(const CGALPrimitive* that)
{
	*poly3=poly3->symmetric_difference(*that->poly3);
	return this;
}

CGALPrimitive* CGALPrimitive::minkowski(const CGALPrimitive* that)
{
	*poly3=CGAL::minkowski_sum_3(*poly3,*that->poly3);
	return this;
}

void CGALPrimitive::transform(const CGAL::AffTransformation3& t)
{
	poly3->transform(t);
}

const CGAL::NefPolyhedron3& CGALPrimitive::getPoly3() const
{
	return *poly3;
}
