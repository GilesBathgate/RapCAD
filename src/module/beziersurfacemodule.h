/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2020 Giles Bathgate
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

#ifndef BEZIERSURFACEMODULE_H
#define BEZIERSURFACEMODULE_H

#include "primitivemodule.h"
#include "context.h"
#include "point.h"

class BezierSurfaceModule : public PrimitiveModule
{
	Q_DECLARE_TR_FUNCTIONS(BezierSurfaceModule)
public:
	explicit BezierSurfaceModule(Reporter&);
	Node* evaluate(const Context&) const override;
private:
	typedef QList<decimal> Vector;
	typedef QList<Point> Points;
	typedef QList<Points> Mesh;

	static decimal bez03(const decimal&);
	static decimal bez13(const decimal&);
	static decimal bez23(const decimal&);
	static decimal bez33(const decimal&);
	static Point pointOnBez(const Points&,const decimal&);
	static Point pointOnBezMesh(const Mesh&,const Vector&);
};

#endif // BEZIERSURFACEMODULE_H
