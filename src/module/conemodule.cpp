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

#include "conemodule.h"
#include "context.h"
#include "math.h"
#include "numbervalue.h"

ConeModule::ConeModule() : PrimitiveModule("cone")
{
	addParameter("height");
	addParameter("radius1");
	addParameter("radius2");
	addParameter("center");
}

Node* ConeModule::evaluate(Context* ctx)
{
	NumberValue* heightValue = dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	NumberValue* r1Value = dynamic_cast<NumberValue*>(getParameterArgument(ctx,1));
	NumberValue* r2Value = dynamic_cast<NumberValue*>(getParameterArgument(ctx,2));
	Value* centerValue = getParameterArgument(ctx,3);

	decimal h=1.0;
	if(heightValue)
		h=heightValue->getNumber();

	decimal r1=0,r2=0;
	if(r1Value)
		r1=r1Value->getNumber();
	if(r2Value)
		r2=r2Value->getNumber();

	bool center = false;
	if(centerValue)
		center=centerValue->isTrue();

	decimal z1,z2;
	z1 = 0.0;
	z2 = h;

	Fragment fg=getSpecialVariables(ctx);
	decimal r=fmax(r1,r2);
	int f = fg.getFragments(r);

	QList<Point> c1=getCircle(r1,f,z1);
	QList<Point> c2=getCircle(r2,f,z2);

	PrimitiveNode* p = new PrimitiveNode();

	if(r1 > 0) {
		p->createPolygon();
		foreach(Point pt, c1)
			p->appendVertex(pt);
	}

	if(h==0.0)
		return p;

	for(int i=0; i<f; i++) {
		int j=(i+1)%f;
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
