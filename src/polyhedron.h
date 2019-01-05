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

#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include "primitive.h"

class Polyhedron : public Primitive
{
public:
	Polyhedron();
	~Polyhedron() override;
	Polygon* createPolygon() override;
	void createVertex(const Point&) override;
	void setType(Primitive_t) override { }
	Primitive_t getType() override { return Lines; }
	void setSanitized(bool) override { }
	bool getSanitized() override { return false; }
	Primitive* buildPrimitive() { return this; }
	bool overlaps(Primitive*) override { return false; }
	void add(Primitive*,bool) override;
	Primitive* group(Primitive*) override;
	Primitive* join(Primitive*) override;
	Primitive* combine() override;
	Primitive* intersection(Primitive*) override;
	Primitive* difference(Primitive*) override;
	Primitive* symmetric_difference(Primitive*) override;
	Primitive* minkowski(Primitive*) override;
	Primitive* inset(const decimal&) override { return this; }
	Primitive* decompose() override { return this; }
	Primitive* complement() override { return this; }
	Primitive* boundary() override { return this; }
	bool isFullyDimentional() override { return false; }
	Primitive* copy() override;
	Primitive* triangulate() override { return this; }
	Primitive* simplify(const decimal&) override { return this; }
	void transform(TransformMatrix*) override;
	bool isEmpty() override;
	QList<Polygon*> getPolygons() const override;
	QList<Point> getPoints() const override;
	QList<Primitive*> getChildren() override;
	void appendChild(Primitive*) override;
	void discrete(int) override;
private:
	QList<Point> points;
	QList<Polygon*> polygons;
	QList<Primitive*> children;
};
#endif // POLYHEDRON_H
