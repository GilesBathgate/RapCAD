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
	CGALExplorer(Primitive*);
	CGALExplorer(CGALPrimitive*);
	CGALExplorer(QList<Primitive*>);
	QList<CGALPolygon*> getPerimeters();
	CGALPrimitive* getPrimitive();
	QList<CGAL::Point3> getPoints();
	CGAL::Cuboid3 getBounds();
private:
	void evaluate();
	QList<CGALPolygon*> perimeters;
	bool evaluated;
	CGALPrimitive* primitive;
	QList<CGAL::Point3> allPoints;
};

#endif // CGALEXPLORER_H
#endif
