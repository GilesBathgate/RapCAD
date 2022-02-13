/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2021 Giles Bathgate
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

#ifndef PRIMITIVEMODULE_H
#define PRIMITIVEMODULE_H

#include "module.h"
#include "polygon.h"
#include "node/primitivenode.h"
#include "node/alignnode.h"
#include "fragment.h"

class PrimitiveModule : public Module
{
public:
	PrimitiveModule(Reporter&, const QString&);
protected:
	static const QList<Point> getCircle(const decimal&,const decimal&,const decimal&);
	static const QList<Point> getPolygon(const decimal&,const decimal&,const decimal&,const decimal&);
	static void createTriangle(Primitive*,int,int,int);
	static void createQuad(Primitive*,int,int,int,int);
};

#endif // PRIMITIVEMODULE_H
