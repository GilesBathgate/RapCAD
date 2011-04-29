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

#include "spheremodule.h"
#include "numbervalue.h"
#include "booleanvalue.h"
#include "tau.h"

SphereModule::SphereModule() : PrimitiveModule("sphere")
{
}

Node* SphereModule::evaluate(Context* ctx,QList<Node*>)
{
	NumberValue* rValue = dynamic_cast<NumberValue*>(ctx->getArgument(0,"radius"));
	BooleanValue* centerValue = dynamic_cast<BooleanValue*>(ctx->getArgument(1,"center"));

	bool center=true;
	if(centerValue)
		center=centerValue->isTrue();

	double r=1;
	if(rValue)
		r=rValue->getNumber();

	double fn=0.0;
	double fs=1.0;
	double fa=12.0;
	int f = getFragments(r,fn,fs,fa);
	int ringCount=f/2;

	QList<Polygon> rings;
	for(int i=0; i<ringCount; i++) {
		double phi = (M_PI*(i+0.5)) / ringCount;
		double r2 = r*sin(phi);
		double z = r*cos(phi);
		Polygon c = getCircle(r2,f,z);
		rings.append(c);
	}

	PrimitiveNode* p = new PrimitiveNode();

	p->createPolygon();
	for(int i=0; i<f; i++)
		p->appendVertex(rings.at(0).at(i));

	for(int i = 0; i < ringCount-1; i++) {
		Polygon r1 = rings.at(i);
		Polygon r2 = rings.at(i+1);
		int r1i = 0, r2i = 0;
		while(r1i < f || r2i < f) {
			if(r1i >= f)
				goto next_r2;
			if(r2i >= f)
				goto next_r1;
			if((double)r1i / f <
					(double)r2i / f) {
next_r1:
				p->createPolygon();
				int r1j = (r1i+1) % f;
				p->prependVertex(r1.at(r1i));
				p->prependVertex(r1.at(r1j));
				p->prependVertex(r2.at(r2i % f));
				r1i++;
			} else {
next_r2:
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
	for(int i=0; i<f; i++)
		p->prependVertex(rings.at(ringCount-1).at(i));

	return p;
}
