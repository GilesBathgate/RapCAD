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
#include "prismmodule.h"
#include "numbervalue.h"

PrismModule::PrismModule() : PrimitiveModule("prism")
{
	addParameter("height");
	addParameter("sides");
	addParameter("apothem");
	addParameter("center");
}

Node* PrismModule::evaluate(Context* ctx)
{
	NumberValue* heightVal = dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	decimal h=1.0;
	if(heightVal)
		h=heightVal->getNumber();

	int n=3;
	NumberValue* sidesVal = dynamic_cast<NumberValue*>(getParameterArgument(ctx,1));
	if(sidesVal)
		n=sidesVal->getNumber();

	decimal r=1.0,a=1.0;
	NumberValue* apothemVal = dynamic_cast<NumberValue*>(getParameterArgument(ctx,2));
	if(apothemVal) {
		a=apothemVal->getNumber();
		r=a/cos(M_PI/n);
	} else {
		NumberValue* radiusVal = dynamic_cast<NumberValue*>(ctx->getArgument(2,"radius"));
		if(radiusVal) {
			r=radiusVal->getNumber();
			a=r*cos(M_PI/n);
		}
	}

	Value* centerVal = getParameterArgument(ctx,3);
	bool center=false;
	if(centerVal)
		center=centerVal->isTrue();

	decimal z1,z2;
	z1 = 0.0;
	z2 = h;

	QList<Point> p1=getPolygon(a,r,n,z1);
	QList<Point> p2=getPolygon(a,r,n,z2);

	PrimitiveNode* p = new PrimitiveNode();

	if(r > 0) {
		for(int i=0; i<n; i++) {
			int j=(i+1)%n;
			p->createPolygon();
			p->appendVertex(p1.at(i));
			p->appendVertex(p2.at(i));
			p->appendVertex(p2.at(j));
			p->appendVertex(p1.at(j));
		}

		p->createPolygon();
		foreach(Point pt,p1)
			p->appendVertex(pt);

		p->createPolygon();
		foreach(Point pt,p2)
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
