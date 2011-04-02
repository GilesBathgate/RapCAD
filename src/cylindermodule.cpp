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
#include <stdio.h>

CylinderModule::CylinderModule()
{
	setName("cylinder");
}

AbstractNode* CylinderModule::evaluate(Context* ctx,QVector<AbstractNode*>)
{
	NumberValue* heightValue = dynamic_cast<NumberValue*>(ctx->getArgument(0,"height"));
	double h = heightValue->getNumber();

	NumberValue* rValue = dynamic_cast<NumberValue*>(ctx->getArgument(1,"radius"));
	BooleanValue* centerValue;

	double r,r1,r2,fn,fs,fa;
	if(!rValue) {
		NumberValue* r1Value = dynamic_cast<NumberValue*>(ctx->getArgument(1,"radius1"));
		NumberValue* r2Value = dynamic_cast<NumberValue*>(ctx->getArgument(2,"radius2"));
		centerValue = dynamic_cast<BooleanValue*>(ctx->getArgument(3,"center"));
		r1=r1Value->getNumber();
		r2=r2Value->getNumber();
		r=fmax(r1,r2);
	} else {
		centerValue = dynamic_cast<BooleanValue*>(ctx->getArgument(2,"center"));
		r1=r2=r=rValue->getNumber();
	}
	bool center = false;
	if(centerValue)
		center=centerValue->isTrue();

	double z1,z2;
	if(center) {
		z1 = -h/2;
		z2 = +h/2;
	} else {
		z1 = 0;
		z2 = h;
	}
	fn=0.0;
	fs=1.0;
	fa=12.0;
	int f = getFragments(r,fn,fs,fa);
	PrimitiveNode::Polygon c1 = getCircle(r1,f);
	PrimitiveNode::Polygon c2 = getCircle(r2,f);
	PrimitiveNode* p = new PrimitiveNode();

	for(int i=0; i<f; i++) {
		int j=(i+1)%f;
		double c1ix = c1.at(i).x;
		double c1iy = c1.at(i).y;
		double c2ix = c2.at(i).x;
		double c2iy = c2.at(i).y;
		double c1jx = c1.at(j).x;
		double c1jy = c1.at(j).y;
		double c2jx = c2.at(j).x;
		double c2jy = c2.at(j).y;
		if(r1 > 0) {
			p->createPolygon();
			p->appendVertex(c1ix,c1iy,z1);
			p->appendVertex(c2ix,c2iy,z2);
			p->appendVertex(c1jx,c1jy,z1);
		}
		if(r2 > 0) {
			p->createPolygon();
			p->appendVertex(c2ix,c2iy,z2);
			p->appendVertex(c2jx,c2jy,z2);
			p->appendVertex(c1jx,c1jy,z1);
		}
	}

	if(r1 > 0) {
		p->createPolygon();
		for(int i=0; i<f; i++)
			p->appendVertex(c1.at(i).x, c1.at(i).y, z1);
	}

	if(r2 > 0) {
		p->createPolygon();
		for(int i=0; i<f; i++)
			p->appendVertex(c2.at(i).x, c2.at(i).y, z2);
	}

	return p;
}
