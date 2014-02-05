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

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "point.h"
#include "polygon.h"

class Primitive
{
public:
	virtual ~Primitive() {}
	virtual Polygon* createPolygon()=0;
	virtual void setSkeleton(bool)=0;
	virtual void appendVertex(Point)=0;
	virtual void prependVertex(Point)=0;
	virtual Primitive* buildPrimitive()=0;
	virtual Primitive* join(const Primitive*)=0;
	virtual void add(const Primitive*)=0;
	virtual Primitive* join()=0;
	virtual Primitive* intersection(const Primitive*)=0;
	virtual Primitive* difference(const Primitive*)=0;
	virtual Primitive* symmetric_difference(const Primitive*)=0;
	virtual Primitive* minkowski(const Primitive*)=0;
	virtual Primitive* inset(const double)=0;
	virtual Primitive* copy()=0;
	virtual bool isEmpty()=0;
	virtual bool isFullyDimentional()=0;
};

#endif // PRIMITIVE_H
