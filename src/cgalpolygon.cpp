/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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
#if USE_CGAL
#include "cgalpolygon.h"
#include "cgalprimitive.h"

CGALPolygon::CGALPolygon(CGALPrimitive* p) : Polygon(p)
{
}

QList<CGAL::Point3> CGALPolygon::getPoints() const
{
	QList<CGAL::Point3> points;
	CGALPrimitive* pr=static_cast<CGALPrimitive*>(parent);
	QList<CGAL::Point3> parentPoints=pr->getCGALPoints();
	foreach(int i,indexes)
		points.append(parentPoints.at(i));
	return points;
}

void CGALPolygon::setNormal(CGAL::Vector3 v)
{
	normal=v;
}

CGAL::Vector3 CGALPolygon::getNormal() const
{
	return normal;
}
#endif
