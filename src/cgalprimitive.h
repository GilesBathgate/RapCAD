/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2012 Giles Bathgate
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
#ifndef CGALPRIMITIVE_H
#define CGALPRIMITIVE_H

#include <QVector>
#include "cgalpolygon.h"

class CGALPrimitive
{
public:
	CGALPrimitive();
	CGALPrimitive(QVector<CGAL::Point3> pl);
	CGALPrimitive(CGAL::Polyhedron3 poly);
	CGALPolygon* createPolygon();
	void appendVertex(CGAL::Point3);
	void prependVertex(CGAL::Point3);
	CGALPrimitive* buildVolume();
	CGALPrimitive* join(const CGALPrimitive*);
	CGALPrimitive* intersection(const CGALPrimitive*);
	CGALPrimitive* difference(const CGALPrimitive*);
	CGALPrimitive* symmetric_difference(const CGALPrimitive*);
	CGALPrimitive* minkowski(const CGALPrimitive*);
	CGALPrimitive* inset(double);
	void transform(const CGAL::AffTransformation3&);
	QList<CGALPolygon*> getPolygons() const;
	QList<CGAL::Point3> getPoints() const;
	const CGAL::NefPolyhedron3& getNefPolyhedron() const;
	CGAL::Polyhedron3* getPolyhedron();
private:
	QList<CGALPolygon*> polygons;
	QList<CGAL::Point3> points;
	CGAL::NefPolyhedron3* nefPolyhedron;
};

#endif // CGALPRIMITIVE_H
