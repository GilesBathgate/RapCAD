/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2012 Giles Bathgate
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
#include "node/primitivenode.h"
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
	double center=false;
	if(centerVal)
		center = centerVal->isTrue();

	double x=1.0,y=1.0,z=1.0;
	if(sizeVal) {
		VectorValue* size=sizeVal->toVector(3);
		Point p = size->getPoint();
		p.getXYZ(x,y,z);
	}

	PrimitiveNode* p=new PrimitiveNode();
	double x1, x2, y1, y2, z1, z2;
	if(center) {
		x1 = -x/2;
		x2 = +x/2;
		y1 = -y/2;
		y2 = +y/2;
		z1 = -z/2;
		z2 = +z/2;
	} else {
		x1 = y1 = z1 = 0;
		x2 = x;
		y2 = y;
		z2 = z;
	}

	p->createPolygon(); // top
	p->appendVertex(x1, y1, z2);
	p->appendVertex(x2, y1, z2);
	p->appendVertex(x2, y2, z2);
	p->appendVertex(x1, y2, z2);

	if(z==0.0)
		return p;

	p->createPolygon(); // side1
	p->appendVertex(x1, y1, z1);
	p->appendVertex(x2, y1, z1);
	p->appendVertex(x2, y1, z2);
	p->appendVertex(x1, y1, z2);

	p->createPolygon(); // side2
	p->appendVertex(x2, y1, z1);
	p->appendVertex(x2, y2, z1);
	p->appendVertex(x2, y2, z2);
	p->appendVertex(x2, y1, z2);

	p->createPolygon(); // side3
	p->appendVertex(x2, y2, z1);
	p->appendVertex(x1, y2, z1);
	p->appendVertex(x1, y2, z2);
	p->appendVertex(x2, y2, z2);

	p->createPolygon(); // side4
	p->appendVertex(x1, y2, z1);
	p->appendVertex(x1, y1, z1);
	p->appendVertex(x1, y1, z2);
	p->appendVertex(x1, y2, z2);

	p->createPolygon(); // bottom
	p->appendVertex(x1, y2, z1);
	p->appendVertex(x2, y2, z1);
	p->appendVertex(x2, y1, z1);
	p->appendVertex(x1, y1, z1);

	return p;

}
