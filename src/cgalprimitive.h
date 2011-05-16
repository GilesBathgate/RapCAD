#ifndef CGALPRIMITIVE_H
#define CGALPRIMITIVE_H

#include <QVector>
#include "cgalbuilder.h"
#include "primitivenode.h"

class CGALPrimitive : private CGALBuilder
{
public:
	CGALPrimitive(PrimitiveNode* n);
	CGALPrimitive(QVector<CGAL::Point3> pl);
	CGALPrimitive(CGAL::Polyhedron3 poly);
	CGALPrimitive* join(const CGALPrimitive*);
	CGALPrimitive* intersection(const CGALPrimitive*);
	CGALPrimitive* difference(const CGALPrimitive*);
	CGALPrimitive* symmetric_difference(const CGALPrimitive*);
	CGALPrimitive* minkowski(const CGALPrimitive*);
	void transform(const CGAL::AffTransformation3&);
	const CGAL::NefPolyhedron3& getPoly3() const;
private:
	CGAL::NefPolyhedron3* poly3;
};

#endif // CGALPRIMITIVE_H
