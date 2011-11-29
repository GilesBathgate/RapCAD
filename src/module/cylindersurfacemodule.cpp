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

#include "cylindersurfacemodule.h"
#include "numbervalue.h"
#include "booleanvalue.h"

CylinderSurfaceModule::CylinderSurfaceModule() : PrimitiveModule("cylinder_surface")
{
}


Node* CylinderSurfaceModule::evaluate(Context* ctx)
{
	NumberValue* heightValue = dynamic_cast<NumberValue*>(ctx->getArgument(0,"height"));
	double h=1.0;
	if(heightValue)
		h=heightValue->getNumber();

	NumberValue* rValue = dynamic_cast<NumberValue*>(ctx->getArgument(1,"radius"));
	double r=1.0,fn,fs,fa;
	if(rValue)
		r=rValue->getNumber();

	BooleanValue* centerValue=dynamic_cast<BooleanValue*>(ctx->getArgument(2,"center"));
	bool center=false;
	if(centerValue)
		center=centerValue->isTrue();

	double z1,z2;
	if(center) {
		z1 = -h/2;
		z2 = +h/2;
	} else {
		z1 = 0.0;
		z2 = h;
	}
	fn=0.0;
	fs=1.0;
	fa=12.0;
	int f = getFragments(r,fn,fs,fa);
	Polygon c1 = getCircle(r,f,z1);
	Polygon c2 = getCircle(r,f,z2);
	PrimitiveNode* p = new PrimitiveNode();

	for(int i=0; i<f; i++) {
		int j=(i+1)%f;
		p->createPolygon();
		p->appendVertex(c1.at(i));
		p->appendVertex(c2.at(i));
		p->appendVertex(c2.at(j));
		p->appendVertex(c1.at(j));
	}

	return p;
}
