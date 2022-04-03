/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2022 Giles Bathgate
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
	bool getSanitized() override { return false; }
	bool isEmpty() override;
	bool isFullyDimentional() override { return false; }
	bool overlaps(Primitive*) override { return false; }
	Polygon& createPolygon() override;
	Primitive* boundary() override { return this; }
	Primitive* chain_hull(Primitive*,Primitive*) override { return this; }
	Primitive* combine() override;
	Primitive* complement() override { return this; }
	Primitive* copy() override;
	Primitive* decompose() override { return this; }
	Primitive* difference(Primitive*) override;
	Primitive* glide(Primitive*) override { return this; }
	Primitive* group(Primitive*) override;
	Primitive* hull(bool) override { return this; }
	Primitive* hull(QList<Point>) override { return this; }
	Primitive* inset(const decimal&) override { return this; }
	Primitive* intersection(Primitive*) override;
	Primitive* join(Primitive*) override;
	Primitive* linear_extrude(const decimal&,const Point&) override { return this; }
	Primitive* minkowski(Primitive*) override;
	Primitive* projection(bool) override { return this; }
	Primitive* rotate_extrude(const decimal&,const decimal&,const decimal&,const Fragment*,const Point&) override { return this; }
	Primitive* simplify(const decimal&) override { return this; }
	Primitive* slice(const decimal&,const decimal&) override { return this; }
	Primitive* subdivide(int) override { return this; }
	Primitive* symmetric_difference(Primitive*) override;
	Primitive* triangulate() override { return this; }
	PrimitiveTypes getType() override;
	const QList<Point> getPoints() const override;
	const QList<Polygon*> getPolygons() const override;
	const QList<Primitive*> getChildren() const override;
	void align(bool,QList<ViewDirections>) override {}
	void appendChild(Primitive*) override;
	void appendChildren(QList<Primitive*>) override;
	void createVertex(const Point&) override;
	void discrete(int) override;
	void groupLater(Primitive*) override;
	void joinLater(Primitive*) override;
	void resize(bool,const Point&) override	{}
	void setSanitized(bool) override { }
	void setType(PrimitiveTypes) override;
	void transform(TransformMatrix*) override;
private:
	QList<Point> points;
	QList<Polygon*> polygons;
	QList<Primitive*> children;
	PrimitiveTypes type;
};
#endif // POLYHEDRON_H
