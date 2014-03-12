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
		makeSideX(p,x1,y1,y2,z1,z2);
		return p;
	}

	if(y==0.0) {
		makeSideY(p,x1,x2,y1,z1,z2);
		return p;
	}

	if(z==0.0) {
		makeSideZ(p,x1,x2,y1,y2,z1);
		return p;
	}

	makeSideZ(p,x1,x2,y1,y2,z2); //Top   (use z2)
	makeSideY(p,x1,x2,y1,z1,z2); //Side1 (use y1)
	makeSideX(p,x2,y1,y2,z1,z2); //Side2 (use x2)
	makeSideY(p,x2,x1,y2,z1,z2); //Side3 (use y2) (swap x1 <--> x2)
	makeSideX(p,x1,y2,y1,z1,z2); //Side4 (use x1) (swap y1 <--> y2)
	makeSideZ(p,x1,x2,y2,y1,z1); //Top   (use z1) (swap y1 <--> y2)

	if(center) {
		AlignNode* n=new AlignNode();
		n->setCenter(true);
		n->addChild(p);
		return n;
	}

	return p;

}

void CubeModule::makeSideZ(PrimitiveNode* p,decimal x1,decimal x2,decimal y1,decimal y2,decimal z)
{
	p->createPolygon(); // sideZ
	p->appendVertex(x1, y1, z);
	p->appendVertex(x2, y1, z);
	p->appendVertex(x2, y2, z);
	p->appendVertex(x1, y2, z);
}

void CubeModule::makeSideY(PrimitiveNode* p,decimal x1,decimal x2,decimal y,decimal z1,decimal z2)
{
	p->createPolygon(); // sideY
	p->appendVertex(x1, y, z1);
	p->appendVertex(x2, y, z1);
	p->appendVertex(x2, y, z2);
	p->appendVertex(x1, y, z2);
}

void CubeModule::makeSideX(PrimitiveNode* p,decimal x,decimal y1,decimal y2,decimal z1,decimal z2)
{
	p->createPolygon(); // sideX
	p->appendVertex(x, y1, z1);
	p->appendVertex(x, y2, z1);
	p->appendVertex(x, y2, z2);
	p->appendVertex(x, y1, z2);
}
