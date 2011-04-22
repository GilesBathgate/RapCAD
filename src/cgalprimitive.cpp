#include "cgalprimitive.h"

CGALPrimitive::CGALPrimitive(CGAL::NefPolyhedron3* n)
{
	dimentions=3;
	poly3=n;
}

CGALPrimitive* CGALPrimitive::join(CGALPrimitive* that)
{
	if(dimentions==2) {
		*poly2=poly2->join(*that->poly2);
	} else if(dimentions==3) {
		*poly3=poly3->join(*that->poly3);
	}
	return this;
}

CGALPrimitive* CGALPrimitive::intersection(CGALPrimitive* that)
{
	if(dimentions==2) {
		*poly2=poly2->intersection(*that->poly2);
	} else if(dimentions==3) {
		*poly3=poly3->intersection(*that->poly3);
	}
	return this;
}

CGALPrimitive* CGALPrimitive::difference(CGALPrimitive* that)
{
	if(dimentions==2) {
		*poly2=poly2->difference(*that->poly2);
	} else if(dimentions==3) {
		*poly3=poly3->difference(*that->poly3);
	}
	return this;
}

CGALPrimitive* CGALPrimitive::symmetric_difference(CGALPrimitive* that)
{
	if(dimentions==2) {
		*poly2=poly2->symmetric_difference(*that->poly2);
	} else if(dimentions==3) {
		*poly3=poly3->symmetric_difference(*that->poly3);
	}
	return this;
}

void CGALPrimitive::transform(const CGAL::AffTransformation3& t)
{
	poly3->transform(t);
}

void CGALPrimitive::transform(const CGAL::AffTransformation2&)
{
	//poly2->transform()
}

const CGAL::NefPolyhedron3& CGALPrimitive::getPoly3() const
{
	return *poly3;
}
