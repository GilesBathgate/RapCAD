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

#include "cubemodule.h"
#include "context.h"
#include "vectorvalue.h"

CubeModule::CubeModule() : PrimitiveModule("cube")
{
	addParameter("size");
	addParameter("center");
}

Node* CubeModule::evaluate(Context* ctx)
{
	Value* sizeVal=getParameterArgument(ctx,0);
	Value* centerVal=getParameterArgument(ctx,1);
	decimal center=false;
	if(centerVal)
		center = centerVal->isTrue();

	decimal x=1.0,y=1.0,z=1.0;
	if(sizeVal) {
		VectorValue* size=sizeVal->toVector(3);
		Point p = size->getPoint();
		p.getXYZ(x,y,z);
	}

	PrimitiveNode* p=new PrimitiveNode();
	decimal x1, x2, y1, y2, z1, z2;
	x1 = y1 = z1 = 0;
	x2 = x;
	y2 = y;
	z2 = z;

	if(x==0.0) {
		p->createVertex(x1,y2,z1); //0
		p->createVertex(x1,y1,z1); //1
		p->createVertex(x1,y1,z2); //2
		p->createVertex(x1,y2,z2); //3
		Polygon* pg=p->createPolygon();
		pg->append(0);
		pg->append(1);
		pg->append(2);
		pg->append(3);
		return p;
	}

	if(y==0.0) {
		p->createVertex(x1,y1,z1); //0
		p->createVertex(x2,y1,z1); //1
		p->createVertex(x2,y1,z2); //2
		p->createVertex(x1,y1,z2); //3
		Polygon* pg=p->createPolygon();
		pg->append(0);
		pg->append(1);
		pg->append(2);
		pg->append(3);
		return p;
	}

	if(z==0.0) {
		p->createVertex(x1,y2,z1); //0
		p->createVertex(x2,y2,z1); //1
		p->createVertex(x2,y1,z1); //2
		p->createVertex(x1,y1,z1); //3
		Polygon* pg=p->createPolygon();
		pg->append(0);
		pg->append(1);
		pg->append(2);
		pg->append(3);
		return p;
	}

	p->createVertex(x1,y1,z2); //0
	p->createVertex(x2,y1,z2); //1
	p->createVertex(x2,y2,z2); //2
	p->createVertex(x1,y2,z2); //3
	p->createVertex(x1,y1,z1); //4
	p->createVertex(x2,y1,z1); //5
	p->createVertex(x2,y2,z1); //6
	p->createVertex(x1,y2,z1); //7

	//Top
	Polygon* pg=p->createPolygon();
	pg->append(0);
	pg->append(1);
	pg->append(2);
	pg->append(3);

	pg=p->createPolygon();
	pg->append(4);
	pg->append(5);
	pg->append(1);
	pg->append(0);

	pg=p->createPolygon();
	pg->append(5);
	pg->append(6);
	pg->append(2);
	pg->append(1);

	pg=p->createPolygon();
	pg->append(6);
	pg->append(7);
	pg->append(3);
	pg->append(2);

	pg=p->createPolygon();
	pg->append(7);
	pg->append(4);
	pg->append(0);
	pg->append(3);

	//Bottom
	pg=p->createPolygon();
	pg->append(7);
	pg->append(6);
	pg->append(5);
	pg->append(4);

	if(center) {
		AlignNode* n=new AlignNode();
		n->setCenter(true);
		n->addChild(p);
		return n;
	}

	return p;

}
