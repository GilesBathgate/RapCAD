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
#include "spheremodule.h"
#include "numbervalue.h"
#include "tau.h"
#include "node/pointnode.h"

SphereModule::SphereModule() : PrimitiveModule("sphere")
{
	addParameter("radius");
	addParameter("center");
}

Node* SphereModule::evaluate(Context* ctx)
{
	NumberValue* rValue=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	Value* centerValue=getParameterArgument(ctx,1);

	bool center=true;
	if(centerValue)
		center=centerValue->isTrue();

	decimal r=1.0;
	if(rValue) {
		r=rValue->getNumber();
	} else {
		NumberValue* dValue = dynamic_cast<NumberValue*>(ctx->getArgument(0,"diameter"));
		if(dValue)
			r=(dValue->getNumber()/2.0);
	}
	if(r==0.0)
		return new PointNode();

	Fragment fg=getSpecialVariables(ctx);
	int f = fg.getFragments(r);
	int ringCount=f/2;

	QList<QList<Point> > rings;
	for(int i=0; i<ringCount; i++) {
		decimal phi = (M_PI*(i+0.5)) / ringCount;
		decimal r2 = r*sin(phi);
		decimal z = r*cos(phi)+!center*r;
		QList<Point> c = getCircle(r2,f,z);
		rings.append(c);
	}

	PrimitiveNode* p = new PrimitiveNode();

	p->createPolygon();
	QList<Point> top=rings.at(0);
	foreach(Point pt, top)
		p->appendVertex(pt);

	for(int i = 0; i < ringCount-1; i++) {
		QList<Point> r1 = rings.at(i);
		QList<Point> r2 = rings.at(i+1);
		int r1i = 0, r2i = 0;
		while(r1i < f || r2i < f) {
			if(r2i >= f||(decimal)r1i/f<(decimal)r2i/f) {
				p->createPolygon();
				int r1j = (r1i+1) % f;
				p->prependVertex(r1.at(r1i));
				p->prependVertex(r1.at(r1j));
				p->prependVertex(r2.at(r2i % f));
				r1i++;
			} else {
				p->createPolygon();
				int r2j = (r2i+1) % f;
				p->appendVertex(r2.at(r2i));
				p->appendVertex(r2.at(r2j));
				p->appendVertex(r1.at(r1i % f));
				r2i++;
			}
		}
	}

	p->createPolygon();
	QList<Point> bottom=rings.at(ringCount-1);
	foreach(Point pt, bottom)
		p->prependVertex(pt);

	return p;
}
