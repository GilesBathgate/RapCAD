/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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
#ifndef CGALPOLYGON_H
#define CGALPOLYGON_H

#include "cgal.h"

#include "cgalprojection.h"
#include "polygon.h"
#include <QList>

namespace CGAL
{
using Segment3 = CGAL::Segment_3<CGAL::Kernel3>;
}

class CGALPolygon : public Polygon
{
public:

	explicit CGALPolygon(class CGALPrimitive&);
	~CGALPolygon();

	void appendVertex(CGAL::Point3);
	void appendVertex(CGAL::Point3,bool);

	QList<CGAL::Point3> getPoints() const;
	QList<CGAL::Point2> getProjectedPoints();
	QList<CGAL::Segment3> getSegments() const;

	CGAL::Vector3 getNormal() const;
	void calculatePlane();

	CGAL::Plane3 getPlane() const;
	void setPlane(const CGAL::Plane3&);

	CGALProjection* getProjection();
	bool sameProjection(CGALPolygon*);

	CGAL::Orientation getOrientation() const;
	void setOrientation(const CGAL::Orientation&);

private:
	void calculateProjection();

	CGAL::Plane3 plane;
	CGALProjection* projection;
	CGAL::Orientation orientation;
};
#endif // CGALPOLYGON_H
#endif
