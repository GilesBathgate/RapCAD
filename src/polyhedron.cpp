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
#include "polyhedron.h"

Polyhedron::Polyhedron() :
	type(PrimitiveTypes::Lines)
{
}

Polyhedron::~Polyhedron()
{
	qDeleteAll(polygons);
	polygons.clear();
	qDeleteAll(children);
	children.clear();
}

Polygon& Polyhedron::createPolygon()
{
	auto* pg = new Polygon(*this);
	polygons.append(pg);
	return *pg;
}

void Polyhedron::createVertex(const Point& p)
{
	points.append(p);
}

void Polyhedron::setType(PrimitiveTypes t)
{
	type=t;
}

PrimitiveTypes Polyhedron::getType()
{
	return type;
}

void Polyhedron::groupLater(Primitive *p)
{
	appendChild(p);
}

void Polyhedron::joinLater(Primitive *p)
{
	appendChild(p);
}

Primitive* Polyhedron::group(Primitive* p)
{
	return join(p);
}

Primitive* Polyhedron::join(Primitive* p)
{
	appendChild(p);
	return this;
}

Primitive* Polyhedron::combine()
{
	return this;
}

Primitive* Polyhedron::intersection(Primitive* p)
{
	return join(p);
}

Primitive* Polyhedron::difference(Primitive* p)
{
	return join(p);
}

Primitive* Polyhedron::symmetric_difference(Primitive* p)
{
	return join(p);
}

Primitive* Polyhedron::minkowski(Primitive* p)
{
	return join(p);
}

Primitive* Polyhedron::copy()
{
	auto* c=new Polyhedron();
	for(const auto& p: getPoints()) {
		c->createVertex(p);
	}
	for(Polygon* pg: getPolygons()) {
		Polygon& npg=c->createPolygon();
		npg.setIndexes(pg->getIndexes());
	}
	return c;
}

void Polyhedron::transform(TransformMatrix* matrix)
{
	if(!matrix) return;
#if USE_CGAL
	CGAL::AffTransformation3 t=matrix->getTransform();
#else
	TransformMatrix* t=matrix;
#endif
	QList<Point> nps;
	for(const auto& p: getPoints()) {
		nps.append(p.transform(t));
	}
	points=nps;

	for(Primitive* p: getChildren())
		p->transform(matrix);
}

bool Polyhedron::isEmpty()
{
	return polygons.isEmpty();
}

const QList<Polygon*> Polyhedron::getPolygons() const
{
	return polygons;
}

const QList<Point> Polyhedron::getPoints() const
{
	return points;
}

const QList<Primitive*> Polyhedron::getChildren() const
{
	return children;
}

void Polyhedron::appendChild(Primitive* p)
{
	children.append(p);
}

void Polyhedron::appendChildren(QList<Primitive*> p)
{
	children.append(p);
}

void Polyhedron::discrete(int)
{
}
