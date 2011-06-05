#ifndef CGALPRIMITIVE_H
#define CGALPRIMITIVE_H

#include <QVector>
#include "cgalpolygon.h"

class CGALPrimitive
{
public:
	CGALPrimitive();
	CGALPrimitive(QVector<CGAL::Point3> pl);
	CGALPrimitive(CGAL::Polyhedron3 poly);
	CGALPolygon* createPolygon();
	void appendVertex(CGAL::Point3);
	void prependVertex(CGAL::Point3);
	CGALPrimitive* buildVolume();
	CGALPrimitive* join(const CGALPrimitive*);
	CGALPrimitive* intersection(const CGALPrimitive*);
	CGALPrimitive* difference(const CGALPrimitive*);
	CGALPrimitive* symmetric_difference(const CGALPrimitive*);
	CGALPrimitive* minkowski(const CGALPrimitive*);
	CGALPrimitive* inset(double);
	void transform(const CGAL::AffTransformation3&);
	QList<CGALPolygon*> getPolygons() const;
	QList<CGAL::Point3> getPoints() const;
	const CGAL::NefPolyhedron3& getNefPolyhedron() const;
private:
	QList<CGALPolygon*> polygons;
	QList<CGAL::Point3> points;
	CGAL::NefPolyhedron3* nefPolyhedron;
};

#endif // CGALPRIMITIVE_H
