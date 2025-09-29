/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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
#include "fragment.h"
#include "polygon.h"
#include "transformmatrix.h"
#include "viewdirections.h"

enum class PrimitiveTypes {
	Volume,
	Surface,
	Lines,
	Points
};

class Primitive
{
	Q_DISABLE_COPY_MOVE(Primitive)
protected:
	Primitive()=default;
public:
	virtual ~Primitive()=default;
	virtual bool getSanitized()=0;
	virtual bool isEmpty()=0;
	virtual bool isFullyDimentional()=0;
	virtual bool overlaps(Primitive*)=0;
	virtual Polygon& createPolygon()=0;
	virtual Primitive* boundary()=0;
	virtual Primitive* chain_hull(Primitive*,Primitive*)=0;
	virtual Primitive* combine()=0;
	virtual Primitive* complement()=0;
	virtual Primitive* copy()=0;
	virtual Primitive* decompose()=0;
	virtual Primitive* difference(Primitive*)=0;
	virtual Primitive* glide(Primitive*)=0;
	virtual Primitive* group(Primitive*)=0;
	virtual Primitive* hull(bool)=0;
	virtual Primitive* hull(const QList<Point>&)=0;
	virtual Primitive* inset(const decimal&)=0;
	virtual Primitive* intersection(Primitive*)=0;
	virtual Primitive* join(Primitive*)=0;
	virtual Primitive* linear_extrude(const decimal&,const Point&)=0;
	virtual Point locate(const Point&,const Point&)=0;
	virtual Primitive* minkowski(Primitive*)=0;
	virtual Primitive* projection(bool)=0;
	virtual	Primitive* rotate_extrude(const decimal&,const decimal&,const decimal&,const Fragment*,const Point&)=0;
	virtual Primitive* simplify(const decimal&)=0;
	virtual Primitive* slice(const decimal&,const decimal&)=0;
	virtual Primitive* subdivide(int)=0;
	virtual Primitive* symmetric_difference(Primitive*)=0;
	virtual Primitive* solidify()=0;
	virtual Primitive* triangulate()=0;
	virtual PrimitiveTypes getType()=0;
	virtual QList<Point> getPoints() const=0;
	virtual const QList<Polygon*>& getPolygons() const=0;
	virtual const QList<Primitive*>& getChildren() const=0;
	virtual void align(bool,QList<ViewDirections>)=0;
	virtual void appendChild(Primitive*)=0;
	virtual void appendChildren(QList<Primitive*>)=0;
	virtual void createVertex(const Point&)=0;
	virtual void discrete(int)=0;
	virtual void groupLater(Primitive*)=0;
	virtual void joinLater(Primitive*)=0;
	virtual void resize(bool,const Point&)=0;
	virtual void setSanitized(bool)=0;
	virtual void setType(PrimitiveTypes)=0;
	virtual void transform(TransformMatrix*)=0;
};

#endif // PRIMITIVE_H
