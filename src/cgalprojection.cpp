#include "cgalprojection.h"

static CGAL::Point2 projectXY(const CGAL::Point3& p)
{
	return CGAL::Point2(p.x(),p.y());
}

static CGAL::Point2 projectXZ(const CGAL::Point3& p)
{
	return CGAL::Point2(p.x(),p.z());
}

static CGAL::Point2 projectYZ(const CGAL::Point3& p)
{
	return CGAL::Point2(p.y(),p.z());
}

CGALProjection::CGALProjection(const CGAL::Vector3& v)
{
	int o = CGAL::abs(v[0]) > CGAL::abs(v[1]) ? 0 : 1;
	o = CGAL::abs(v[2]) > CGAL::abs(v[o]) ? 2 : o;

	switch(o)
	{
	case 0:
		projectFunc=&projectYZ;
		break;
	case 1:
		projectFunc=&projectXZ;
		break;
	default:
		projectFunc=&projectXY;
		break;
	}
}

CGAL::Point2 CGALProjection::project(const CGAL::Point3& p3) const
{
	return projectFunc(p3);
}
