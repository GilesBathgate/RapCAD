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

#ifndef CUBEMODULE_H
#define CUBEMODULE_H

#include "primitivemodule.h"

class CubeModule : public PrimitiveModule
{
	Q_DECLARE_TR_FUNCTIONS(CubeModule)
public:
	explicit CubeModule(Reporter&);
	Node* evaluate(const Context&) const override;
	template <class Point,class Scalar, class Primitive>
	static void createCuboid(Primitive* p,Scalar x1,Scalar x2,Scalar y1,Scalar y2,Scalar z1,Scalar z2);
};

template <class Point,class Scalar, class Primitive>
void CubeModule::createCuboid(Primitive* p,Scalar x1,Scalar x2,Scalar y1,Scalar y2,Scalar z1,Scalar z2)
{
	if(x2-x1==0.0) {
		p->createVertex(Point(x1,y2,z1)); //0
		p->createVertex(Point(x1,y1,z1)); //1
		p->createVertex(Point(x1,y1,z2)); //2
		p->createVertex(Point(x1,y2,z2)); //3
		Polygon& pg=p->createPolygon();
		pg.append(0);
		pg.append(1);
		pg.append(2);
		pg.append(3);
		return;
	}

	if(y2-y1==0.0) {
		p->createVertex(Point(x1,y1,z1)); //0
		p->createVertex(Point(x2,y1,z1)); //1
		p->createVertex(Point(x2,y1,z2)); //2
		p->createVertex(Point(x1,y1,z2)); //3
		Polygon& pg=p->createPolygon();
		pg.append(0);
		pg.append(1);
		pg.append(2);
		pg.append(3);
		return;
	}

	if(z2-z1==0.0) {
		p->createVertex(Point(x1,y2,z1)); //0
		p->createVertex(Point(x2,y2,z1)); //1
		p->createVertex(Point(x2,y1,z1)); //2
		p->createVertex(Point(x1,y1,z1)); //3
		Polygon& pg=p->createPolygon();
		pg.append(0);
		pg.append(1);
		pg.append(2);
		pg.append(3);
		return;
	}

	p->createVertex(Point(x1,y1,z2)); //0
	p->createVertex(Point(x2,y1,z2)); //1
	p->createVertex(Point(x2,y2,z2)); //2
	p->createVertex(Point(x1,y2,z2)); //3
	p->createVertex(Point(x1,y1,z1)); //4
	p->createVertex(Point(x2,y1,z1)); //5
	p->createVertex(Point(x2,y2,z1)); //6
	p->createVertex(Point(x1,y2,z1)); //7

	//Top
	Polygon& pg0=p->createPolygon();
	pg0.append(0);
	pg0.append(1);
	pg0.append(2);
	pg0.append(3);

	Polygon& pg1=p->createPolygon();
	pg1.append(4);
	pg1.append(5);
	pg1.append(1);
	pg1.append(0);

	Polygon& pg2=p->createPolygon();
	pg2.append(5);
	pg2.append(6);
	pg2.append(2);
	pg2.append(1);

	Polygon& pg3=p->createPolygon();
	pg3.append(6);
	pg3.append(7);
	pg3.append(3);
	pg3.append(2);

	Polygon& pg4=p->createPolygon();
	pg4.append(7);
	pg4.append(4);
	pg4.append(0);
	pg4.append(3);

	//Bottom
	Polygon& pg5=p->createPolygon();
	pg5.append(7);
	pg5.append(6);
	pg5.append(5);
	pg5.append(4);
}

#endif // CUBEMODULE_H
