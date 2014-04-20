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

#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include "primitive.h"

class Polyhedron : public Primitive
{
public:
	Polygon* createPolygon();
	void appendVertex(Point);
	void prependVertex(Point);
	void setType(Primitive_t) { }
	Primitive* buildPrimitive() { return this; }
	bool overlaps(Primitive*) { return false; }
	void add(Primitive*,bool) { }
	Primitive* group(Primitive*) { return this; }
	Primitive* join(Primitive*) { return this; }
	Primitive* combine() { return this; }
	Primitive* intersection(Primitive*) { return this; }
	Primitive* difference(Primitive*) { return this; }
	Primitive* symmetric_difference(Primitive*) { return this; }
	Primitive* minkowski(Primitive*) { return this; }
	Primitive* inset(decimal) { return this; }
	Primitive* decompose() { return this; }
	Primitive* complement() { return this; }
	bool isFullyDimentional() { return false; }
	Primitive* copy();
	bool isEmpty();
	QList<Polygon*> getPolygons() const;
	QList<Point> getPoints() const;
	QList<Primitive*> getChildren();
	void appendChild(Primitive*);
private:
	QList<Point> points;
	QList<Polygon*> polygons;
};
#endif // POLYHEDRON_H
