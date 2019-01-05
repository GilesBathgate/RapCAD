/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2019 Giles Bathgate
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef USE_CGAL
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
	ortho = CGAL::abs(v[0]) > CGAL::abs(v[1]) ? 0 : 1;
	ortho = CGAL::abs(v[2]) > CGAL::abs(v[ortho]) ? 2 : ortho;

	switch(ortho) {
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

bool CGALProjection::operator==(const CGALProjection& other) const
{
	return ortho==other.ortho;
}

CGAL::Direction3 CGALProjection::getDirection(const CGAL::Vector3& v) const
{
	switch(ortho) {
		case 0:
			return CGAL::Direction3((v[0]>0.0?1.0:-1.0),0.0,0.0);
		case 1:
			return CGAL::Direction3(0.0,(v[1]>0.0?1.0:-1.0),0.0);
		default:
			return CGAL::Direction3(0.0,0.0,(v[2]>0.0?1.0:-1.0));
	}
}
#endif
