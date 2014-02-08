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
#ifndef CGALPRIMITIVE_H
#define CGALPRIMITIVE_H

#include <QVector>
#include "cgalpolygon.h"
#include "primitive.h"

class CGALPrimitive : public Primitive
{
public:
	CGALPrimitive();
	CGALPrimitive(CGAL::Polyhedron3 poly);
	void setType(Primitive_t);
	Polygon* createPolygon();
	void appendVertex(Point);
	void appendVertex(CGAL::Point3);
	void prependVertex(Point);
	void prependVertex(CGAL::Point3);
	Primitive* group();
	Primitive* join(Primitive*);
	void add(Primitive*);
	Primitive* join();
	Primitive* intersection(Primitive*);
	Primitive* difference(Primitive*);
	Primitive* symmetric_difference(Primitive*);
	Primitive* minkowski(Primitive*);
	Primitive* inset(const double);
	Primitive* copy();
	void transform(const CGAL::AffTransformation3&);
	QList<CGALPolygon*> getPolygons() const;
	QList<CGAL::Point3> getPoints() const;
	const CGAL::NefPolyhedron3& getNefPolyhedron();
	CGAL::Polyhedron3* getPolyhedron();
	bool isEmpty();
	bool isFullyDimentional();
private:
	void buildPrimitive();
	QList<Primitive*> primitives;
	CGAL::NefPolyhedron3* createPolyline(QVector<CGAL::Point3> pl);
	QList<CGALPolygon*> polygons;
	CGAL::NefPolyhedron3* nefPolyhedron;
	Primitive_t type;
};

#endif // CGALPRIMITIVE_H
#endif
