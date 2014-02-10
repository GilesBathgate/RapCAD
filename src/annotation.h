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

#ifndef ANNOTATION_H
#define ANNOTATION_H

#include "primitive.h"

class Annotation : public Primitive
{
public:
	Polygon* createPolygon() { return NULL; }
	void appendVertex(Point) {}
	void prependVertex(Point) {}
	Primitive* buildPrimitive() { return this; }
	Primitive* combine(const Primitive*) { return this; }
	Primitive* intersection(const Primitive*) { return this; }
	Primitive* difference(const Primitive*) { return this; }
	Primitive* symmetric_difference(const Primitive*) { return this; }
	Primitive* minkowski(const Primitive*) { return this; }
	Primitive* inset(decimal) { return this; }
	bool isFullyDimentional() { return false; }
	Primitive* copy() { return this; }
	bool isEmpty() { return false; }
};
#endif // ANNOTATION_H
