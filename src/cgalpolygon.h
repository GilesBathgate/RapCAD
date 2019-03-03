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
#ifndef CGALPOLYGON_H
#define CGALPOLYGON_H

#include "cgal.h"

#include <QList>
#include "polygon.h"
#include "cgalprojection.h"

class CGALPolygon : public Polygon
{
public:

	explicit CGALPolygon(const class CGALPrimitive&);
	~CGALPolygon();
	QList<CGAL::Point3> getPoints() const;
	QList<CGAL::Point2> getProjectedPoints();

	CGAL::Direction3 getDirection() const;
	CGAL::Vector3 getNormal() const;
	void calculatePlane();

	CGAL::Plane3 getPlane() const;
	void setPlane(const CGAL::Plane3&);

	bool getHole() const;
	void setHole(bool value);

	CGALProjection* getProjection();
	bool sameProjection(CGALPolygon*);
private:
	void calculateProjection();

	CGAL::Plane3 plane;
	CGALProjection* projection;
	CGAL::Direction3 direction;
	bool hole;
};
#endif // CGALPOLYGON_H
#endif
