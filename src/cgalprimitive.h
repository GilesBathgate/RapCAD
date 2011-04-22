#ifndef CGALPRIMITIVE_H
#define CGALPRIMITIVE_H

#include "cgal.h"

class CGALPrimitive
{
public:
	CGALPrimitive(CGAL::NefPolyhedron3*);
	CGALPrimitive* join(CGALPrimitive*);
	CGALPrimitive* intersection(CGALPrimitive*);
	CGALPrimitive* difference(CGALPrimitive*);
	CGALPrimitive* symmetric_difference(CGALPrimitive*);
	void transform(const CGAL::AffTransformation2&);
	void transform(const CGAL::AffTransformation3&);
	const CGAL::NefPolyhedron3& getPoly3() const;
private:
	int dimentions;
	CGAL::NefPolyhedron3* poly3;
	CGAL::NefPolyhedron2* poly2;
};

#endif // CGALPRIMITIVE_H
