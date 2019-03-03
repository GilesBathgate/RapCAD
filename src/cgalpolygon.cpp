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
#include "cgalpolygon.h"
#include "cgalprimitive.h"
#include <CGAL/normal_vector_newell_3.h>

CGALPolygon::CGALPolygon(const CGALPrimitive& p) :
	Polygon(p),
	projection(nullptr),
	hole(false)
{
}

CGALPolygon::~CGALPolygon()
{
	delete projection;
}

QList<CGAL::Point3> CGALPolygon::getPoints() const
{
	QList<CGAL::Point3> points;
	auto& pr=static_cast<const CGALPrimitive&>(parent);
	QList<CGAL::Point3> parentPoints=pr.getPoints();
	for(auto i: indexes)
		points.append(parentPoints.at(i));
	return points;
}

QList<CGAL::Point2> CGALPolygon::getProjectedPoints()
{
	CGALProjection* pro=getProjection();
	QList<CGAL::Point2> points;
	auto& pr=static_cast<const CGALPrimitive&>(parent);
	QList<CGAL::Point3> parentPoints=pr.getPoints();
	for(auto i: indexes) {
		CGAL::Point3 p3=parentPoints.at(i);
		points.append(pro->project(p3));
	}
	return points;
}

CGAL::Direction3 CGALPolygon::getDirection() const
{
	return direction;
}

CGAL::Vector3 CGALPolygon::getNormal() const
{
	return plane.orthogonal_vector();
}

void CGALPolygon::calculateProjection()
{
	CGAL::Vector3 v=plane.orthogonal_vector();
	projection=new CGALProjection(v);
	direction=projection->getDirection(v);
}

void CGALPolygon::calculatePlane()
{
	QList<CGAL::Point3> points=getPoints();
	if(points.size()==3) {
		plane=CGAL::Plane3(points.at(0),points.at(1),points.at(2));
	} else {
		CGAL::Vector3 v;
		CGAL::normal_vector_newell_3(points.begin(),points.end(),v);
		plane=CGAL::Plane3(points.first(), v);
	}
	calculateProjection();
}

CGAL::Plane3 CGALPolygon::getPlane() const
{
	return plane;
}

void CGALPolygon::setPlane(const CGAL::Plane3& p)
{
	plane=p;
	calculateProjection();
}

bool CGALPolygon::getHole() const
{
	return hole;
}

void CGALPolygon::setHole(bool value)
{
	hole = value;
}

CGALProjection* CGALPolygon::getProjection()
{
	if(!projection)
		calculatePlane();
	return projection;
}

bool CGALPolygon::sameProjection(CGALPolygon* other)
{
	return (*getProjection())==(*other->getProjection());
}

#endif
