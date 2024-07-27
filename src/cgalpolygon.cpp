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
#include "cgalpolygon.h"

#include "cgalprimitive.h"
#include "onceonly.h"
#include <CGAL/normal_vector_newell_3.h>

CGALPolygon::CGALPolygon(CGALPrimitive& p) :
	Polygon(p),
	projection(nullptr),
	orientation(CGAL::POSITIVE)
{
}

CGALPolygon::~CGALPolygon()
{
	delete projection;
}

void CGALPolygon::appendVertex(const CGAL::Point3& pt)
{
	appendVertex(pt,true);
}

void CGALPolygon::appendVertex(const CGAL::Point3& pt, bool direction)
{
	auto& pr=dynamic_cast<CGALPrimitive&>(parent);
	pr.appendVertex(this,pt,direction);
}

QList<CGAL::Point3> CGALPolygon::getPoints() const
{
	QList<CGAL::Point3> points;
	const auto& pr=dynamic_cast<const CGALPrimitive&>(parent);
	const QList<CGAL::Point3> parentPoints=pr.getPoints();
	for(auto i: indexes)
		points.append(parentPoints.at(i));
	return points;
}

QList<CGAL::Point2> CGALPolygon::getProjectedPoints()
{
	CGALProjection* pro=getProjection();
	QList<CGAL::Point2> points;
	const auto& pr=dynamic_cast<const CGALPrimitive&>(parent);
	const QList<CGAL::Point3>& parentPoints=pr.getPoints();
	for(auto i: getIndexes()) {
		const CGAL::Point3& p3=parentPoints.at(i);
		points.append(pro->project(p3));
	}
	return points;
}

QList<CGAL::Segment3> CGALPolygon::getSegments() const
{
	QList<CGAL::Segment3> segments;
	CGAL::Point3 prev;
	OnceOnly first;
	for(const auto& next: getPoints()) {
		if(!first())
			segments.append(CGAL::Segment3(prev,next));
		prev=next;
	}
	return segments;
}

CGAL::Vector3 CGALPolygon::getNormal() const
{
	return plane.orthogonal_vector();
}

void CGALPolygon::calculateProjection()
{
	const CGAL::Vector3& v=plane.orthogonal_vector();
	projection=new CGALProjection(v);
}

CGAL::Orientation CGALPolygon::getOrientation() const
{
	return orientation;
}

void CGALPolygon::setOrientation(const CGAL::Orientation& value)
{
	orientation=value;
}

void CGALPolygon::calculatePlane()
{
	QList<CGAL::Point3> points=getPoints();
#if CGAL_VERSION_NR < CGAL_VERSION_NUMBER(5,3,0)
	if(points.size()==3) {
		plane=CGAL::Plane3(points.at(0),points.at(1),points.at(2));
	} else
#endif
	{
		CGAL::Vector3 v;
		CGAL::normal_vector_newell_3(points.begin(),points.end(),v);
		plane=CGAL::Plane3(points.first(),v);
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
