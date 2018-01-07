#ifndef CGALPROJECTION_H
#define CGALPROJECTION_H

#include "cgal.h"

typedef CGAL::Point2 (*ProjectFunc)(const CGAL::Point3&);

class CGALProjection
{
public:
	explicit CGALProjection(const CGAL::Vector3&);
	CGAL::Point2 project(const CGAL::Point3&) const;
private:
	ProjectFunc projectFunc;
};

#endif // CGALPROJECTION_H
