#ifndef CGALPRIMITIVE_H
#define CGALPRIMITIVE_H

#include "cgal.h"

class CGALPrimitive
{
public:
	CGALPrimitive(CGAL::NefPolyhedron3*);
	CGALPrimitive* join(const CGALPrimitive*);
	CGALPrimitive* intersection(const CGALPrimitive*);
	CGALPrimitive* difference(const CGALPrimitive*);
	CGALPrimitive* symmetric_difference(const CGALPrimitive*);
	void transform(const CGAL::AffTransformation3&);
	const CGAL::NefPolyhedron3& getPoly3() const;
private:
	CGAL::NefPolyhedron3* poly3;
};

#endif // CGALPRIMITIVE_H
