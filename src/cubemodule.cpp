/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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
#include "abstractpolyhedron.h"
#include "context.h"
#include "vectorvalue.h"
#include "booleanvalue.h"
#include <stdio.h>

CubeModule::CubeModule()
{
	setName("cube");
	center=false;
}

void CubeModule::evaluate(Context* ctx, Instance*)
{
	VectorValue* size=dynamic_cast<VectorValue*>(ctx->getArgument(0,"size"));
	BooleanValue* center=dynamic_cast<BooleanValue*>(ctx->getArgument(1,"center"));
	if(center)
		this->center = center->isTrue();

	size->getXYZ(this->x,this->y,this->z);

	AbstractPolyhedron* p=new AbstractPolyhedron();
	double x1, x2, y1, y2, z1, z2;
	if(this->center) {
		x1 = -this->x/2;
		x2 = +this->x/2;
		y1 = -this->y/2;
		y2 = +this->y/2;
		z1 = -this->z/2;
		z2 = +this->z/2;
	} else {
		x1 = y1 = z1 = 0;
		x2 = this->x;
		y2 = this->y;
		z2 = this->z;
	}

	p->createPolygon(); // top
	p->appendVertex(x1, y1, z2);
	p->appendVertex(x2, y1, z2);
	p->appendVertex(x2, y2, z2);
	p->appendVertex(x1, y2, z2);

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

	printf("CUBE:\n%s",p->toString().toLocal8Bit().constData());

}
