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
#ifndef CGALEXPLORER_H
#define CGALEXPLORER_H

#include <QList>
#include "cgalprimitive.h"

class CGALExplorer
{
public:
	typedef CGAL::NefPolyhedron3::Halfedge_const_handle HalfEdgeHandle;
	CGALExplorer(Primitive*);
	CGALExplorer(CGALPrimitive*);
	CGALPrimitive* getPerimeters();
	QList<HalfEdgeHandle> getHalfEdgePerimeter();
	CGALPrimitive* getPrimitive();
	QList<CGAL::Point3> getPoints();
	CGAL::Cuboid3 getBounds();
	QList<CGALPolygon*> getBase();
	CGALVolume getVolume(bool);
private:
	void evaluate();
	CGALPrimitive* perimeters;
	bool evaluated;
	CGALPrimitive* primitive;
	typedef QList<CGAL::Point3> Points;
	Points allPoints;
	QList<CGALPolygon*> basePolygons;
	QList<Points> volumePoints;
	QList<HalfEdgeHandle> visited;
};

#endif // CGALEXPLORER_H
#endif
