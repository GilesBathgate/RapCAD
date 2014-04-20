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

#include <math.h>
#include "cylindermodule.h"
#include "context.h"
#include "numbervalue.h"
#include "booleanvalue.h"
#include "node/primitivenode.h"
#include "tau.h"

CylinderModule::CylinderModule() : PrimitiveModule("cylinder")
{
	addParameter("height");
	addParameter("radius");
	addParameter("center");
}

Node* CylinderModule::evaluate(Context* ctx)
{
	NumberValue* heightValue = dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	decimal h=1.0;
	if(heightValue)
		h=heightValue->getNumber();

	NumberValue* r1Value = dynamic_cast<NumberValue*>(ctx->getArgument(1,"radius1"));
	NumberValue* r2Value = dynamic_cast<NumberValue*>(ctx->getArgument(2,"radius2"));
	BooleanValue* centerValue;

	decimal r1=1.0,r2=1.0;
	if(!r1Value) {
		NumberValue* rValue = dynamic_cast<NumberValue*>(getParameterArgument(ctx,1));
		centerValue = dynamic_cast<BooleanValue*>(getParameterArgument(ctx,2));
		if(rValue) {
			r1=r2=rValue->getNumber();
		} else {
			NumberValue* dValue = dynamic_cast<NumberValue*>(ctx->getArgument(1,"diameter"));
			if(dValue)
				r1=r2=(dValue->getNumber()/2.0);
		}
	} else {
		if(r1Value)
			r1=r1Value->getNumber();
		if(r2Value)
			r2=r2Value->getNumber();
		else
			r2=r1;
		centerValue = dynamic_cast<BooleanValue*>(ctx->getArgument(3,"center"));
	}
	bool center = false;
	if(centerValue)
		center=centerValue->isTrue();

	decimal z1,z2;
	z1 = 0.0;
	z2 = h;

	decimal r=fmax(r1,r2);
	Fragment fg=getSpecialVariables(ctx);
	int f = fg.getFragments(r);

	QList<Point> c1=getCircle(r1,f,z1);
	QList<Point> c2=getCircle(r2,f,z2);

	PrimitiveNode* p = new PrimitiveNode();

	if(r1 > 0) {
		p->createPolygon();
		foreach(Point pt,c1)
			p->appendVertex(pt);
	}

	if(h==0.0)
		return p;

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

	if(r2 > 0) {
		p->createPolygon();
		foreach(Point pt,c2)
			p->prependVertex(pt);
	}

	if(center) {
		AlignNode* n=new AlignNode();
		n->setCenter(true);
		n->addChild(p);
		return n;
	}

	return p;
}
