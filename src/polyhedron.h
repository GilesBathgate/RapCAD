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
	Polyhedron();
	~Polyhedron();
	Polygon* createPolygon();
	void createVertex(Point);
	void setType(Primitive_t) { }
	Primitive* buildPrimitive() { return this; }
	bool overlaps(Primitive*) { return false; }
	void add(Primitive*,bool);
	Primitive* group(Primitive*);
	Primitive* join(Primitive*);
	Primitive* combine();
	Primitive* intersection(Primitive*);
	Primitive* difference(Primitive*);
	Primitive* symmetric_difference(Primitive*);
	Primitive* minkowski(Primitive*);
	Primitive* inset(decimal) { return this; }
	Primitive* decompose() { return this; }
	Primitive* complement() { return this; }
	bool isFullyDimentional() { return false; }
	Primitive* copy();
	Primitive* triangulate() { return this; }
	Primitive* simplify(int) { return this; }
	void transform(TransformMatrix*);
	bool isEmpty();
	QList<Polygon*> getPolygons() const;
	QList<Point> getPoints() const;
	QList<Primitive*> getChildren();
	void appendChild(Primitive*);
	void discrete(int);
private:
	QList<Point> points;
	QList<Polygon*> polygons;
	QList<Primitive*> children;
};
#endif // POLYHEDRON_H
