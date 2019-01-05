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
#ifndef CGALEXPLORER_H
#define CGALEXPLORER_H

#include <QList>
#include "cgalprimitive.h"

class CGALExplorer
{
public:
	explicit CGALExplorer(Primitive*);
	CGALPrimitive* getPerimeters();
	CGALPrimitive* getPrimitive();
	QList<CGAL::Point3> getPoints();
	CGAL::Cuboid3 getBounds();
	QList<CGALPolygon*> getBase();
	CGALVolume getVolume(bool);
private:
	typedef QList<CGAL::Point3> Points;
	void explore();
	void evaluate();
	bool evaluated;
	CGALPrimitive* primitive;
	CGALPrimitive* perimeters;
	const CGAL::NefPolyhedron3& nef;
	QList<CGALPolygon*> basePolygons;
	QList<Points> volumePoints;
	Points allPoints;
};

#endif // CGALEXPLORER_H
#endif
