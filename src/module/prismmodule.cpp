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

#include "prismmodule.h"
#include "numbervalue.h"
#include "tau.h"

PrismModule::PrismModule() : PrimitiveModule("prism")
{
}

Node* PrismModule::evaluate(Context* ctx)
{
	NumberValue* heightVal = dynamic_cast<NumberValue*>(ctx->getArgument(0,"height"));
	double h=1.0;
	if(heightVal)
		h=heightVal->getNumber();

	int n=3;
	NumberValue* sidesVal = dynamic_cast<NumberValue*>(ctx->getArgument(1,"sides"));
	if(sidesVal)
		n=sidesVal->getNumber();

	double r=1.0,a=1.0;
	NumberValue* apothemVal = dynamic_cast<NumberValue*>(ctx->getArgument(2,"apothem"));
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

	Value* centerVal = ctx->getArgument(3,"center");
	bool center=false;
	if(centerVal)
		center=centerVal->isTrue();

	double z1,z2;
	if(center) {
		z1 = -h/2;
		z2 = +h/2;
	} else {
		z1 = 0.0;
		z2 = h;
	}

	Polygon p1 = getPolygon(a,r,n,z1);
	Polygon p2 = getPolygon(a,r,n,z2);

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
		for(int i=0; i<n; i++)
			p->appendVertex(p1.at(i));

		p->createPolygon();
		for(int i=0; i<n; i++)
			p->prependVertex(p2.at(i));
	}

	return p;
}
