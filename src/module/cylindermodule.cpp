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

	int n=0;
	Polygon* pg;
	if(r1>0) {
		pg=p->createPolygon();
		foreach(Point pt,c1) {
			p->createVertex(pt);
			pg->append(n++);
		}
	}

	if(h==0.0)
		return p;

	if(r2>0) {
		pg=p->createPolygon();
		foreach(Point pt,c2) {
			p->createVertex(pt);
			pg->prepend(n++);
		}
	}

	/* In the cases where r1 or r2 are 0,  n will now convinently be pointing
	 * one past the end, and point to the apex as defined here when needed */
	if(r1<=0)
		p->createVertex(0.0,0.0,z1);
	if(r2<=0)
		p->createVertex(0.0,0.0,z2);

	for(int i=0; i<f; ++i) {
		int j=(i+1)%f;

		int k=r2<=0?n:i;
		int l=r1<=0?n:j;

		if(r1>0&&r2>0) {
			k+=f;
			j+=f;
		}

		if(r1 == r2) {
			pg=p->createPolygon();
			pg->append(i);
			pg->append(k);
			pg->append(j);
			pg->append(l);
		} else {
			if(r1 > 0) {
				pg=p->createPolygon();
				pg->append(i);
				pg->append(k);
				pg->append(l);
			}
			if(r2 > 0) {
				pg=p->createPolygon();
				pg->append(j);
				pg->append(l);
				pg->append(k);
			}
		}
	}

	if(center) {
		AlignNode* n=new AlignNode();
		n->setCenter(true);
		n->addChild(p);
		return n;
	}

	return p;
}
