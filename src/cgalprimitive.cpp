#include "cgalprimitive.h"

CGALPrimitive::CGALPrimitive(CGAL::NefPolyhedron3* n)
{
	poly3=n;
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

void CGALPrimitive::transform(const CGAL::AffTransformation3& t)
{
	poly3->transform(t);
}

const CGAL::NefPolyhedron3& CGALPrimitive::getPoly3() const
{
	return *poly3;
}
