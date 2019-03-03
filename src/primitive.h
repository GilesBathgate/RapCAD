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

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "point.h"
#include "polygon.h"
#include "transformmatrix.h"

class Primitive
{
public:
	enum Primitive_t {
		Volume,
		Surface,
		Lines,
		Points
	};

	virtual ~Primitive() {}
	virtual Polygon* createPolygon()=0;
	virtual void setType(Primitive_t)=0;
	virtual Primitive_t getType()=0;
	virtual void setSanitized(bool)=0;
	virtual bool getSanitized()=0;
	virtual void createVertex(const Point&)=0;
	virtual bool overlaps(Primitive*)=0;
	virtual Primitive* group(Primitive*)=0;
	virtual Primitive* join(Primitive*)=0;
	virtual void add(Primitive*,bool)=0;
	virtual Primitive* combine()=0;
	virtual Primitive* intersection(Primitive*)=0;
	virtual Primitive* difference(Primitive*)=0;
	virtual Primitive* symmetric_difference(Primitive*)=0;
	virtual Primitive* minkowski(Primitive*)=0;
	virtual Primitive* inset(const decimal&)=0;
	virtual Primitive* decompose()=0;
	virtual Primitive* complement()=0;
	virtual Primitive* boundary()=0;
	virtual Primitive* copy()=0;
	virtual Primitive* triangulate()=0;
	virtual Primitive* simplify(const decimal&)=0;
	virtual void transform(TransformMatrix*)=0;
	virtual bool isEmpty()=0;
	virtual bool isFullyDimentional()=0;
	virtual QList<Point> getPoints() const=0;
	virtual QList<Polygon*> getPolygons() const=0;
	virtual QList<Primitive*> getChildren()=0;
	virtual void appendChild(Primitive*)=0;
	virtual void discrete(int)=0;
};

#endif // PRIMITIVE_H
