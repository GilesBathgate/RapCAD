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

#include "cylindermodule.h"
#include "context.h"
#include "numbervalue.h"
#include "booleanvalue.h"
#include "primitivenode.h"
#include "tau.h"

CylinderModule::CylinderModule() : PrimitiveModule("cylinder")
{
}

Node* CylinderModule::evaluate(Context* ctx,QList<Node*>)
{
	NumberValue* heightValue = dynamic_cast<NumberValue*>(ctx->getArgument(0,"height"));
	double h=1.0;
	if(heightValue)
		h=heightValue->getNumber();

	NumberValue* r1Value = dynamic_cast<NumberValue*>(ctx->getArgument(1,"radius1",true));
	NumberValue* r2Value = dynamic_cast<NumberValue*>(ctx->getArgument(2,"radius2",true));
	BooleanValue* centerValue;

	double r=1.0,r1=1.0,r2=1.0,fn,fs,fa;
	if(!r1Value) {
		NumberValue* rValue = dynamic_cast<NumberValue*>(ctx->getArgument(1,"radius"));
		centerValue = dynamic_cast<BooleanValue*>(ctx->getArgument(2,"center"));
		if(rValue) {
			r1=r2=r=rValue->getNumber();
		} else {
			NumberValue* dValue = dynamic_cast<NumberValue*>(ctx->getArgument(1,"diameter"));
			if(dValue)
				r1=r2=r=(dValue->getNumber()/2.0);
		}
	} else {
		if(r1Value)
			r1=r1Value->getNumber();
		if(r2Value)
			r2=r2Value->getNumber();
		centerValue = dynamic_cast<BooleanValue*>(ctx->getArgument(3,"center"));
		r=fmax(r1,r2);
	}
	bool center = false;
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
	Polygon c1 = getCircle(r1,f,z1);
	Polygon c2 = getCircle(r2,f,z2);
	PrimitiveNode* p = new PrimitiveNode();

	for(int i=0; i<f; i++) {
		int j=(i+1)%f;
		if(r1 == r2) {
			p->createPolygon();
			p->appendVertex(c1.at(i));
			p->appendVertex(c2.at(i));
			p->appendVertex(c2.at(j));
			p->appendVertex(c1.at(j));
		} else {
			if(r1 > 0) {
				p->createPolygon();
				p->appendVertex(c1.at(i));
				p->appendVertex(c2.at(i));
				p->appendVertex(c1.at(j));
			}
			if(r2 > 0) {
				p->createPolygon();
				p->appendVertex(c2.at(i));
				p->appendVertex(c2.at(j));
				p->appendVertex(c1.at(j));
			}
		}
	}

	if(r1 > 0) {
		p->createPolygon();
		for(int i=0; i<f; i++)
			p->appendVertex(c1.at(i));
	}

	if(r2 > 0) {
		p->createPolygon();
		for(int i=0; i<f; i++)
			p->prependVertex(c2.at(i));
	}

	return p;
}
