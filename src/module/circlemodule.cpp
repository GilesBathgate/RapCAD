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

#include "circlemodule.h"
#include "numbervalue.h"

CircleModule::CircleModule() : PrimitiveModule("circle")
{
}

Node* CircleModule::evaluate(Context* ctx,QList<Node*>)
{
	NumberValue* rValue=dynamic_cast<NumberValue*>(ctx->getArgument(0,"radius"));

	double r,fn,fs,fa;
	if(rValue) {

		r=rValue->getNumber();
	}

	fn=0.0;
	fs=1.0;
	fa=12.0;
	int f = getFragments(r,fn,fs,fa);
	Polygon c = getCircle(r,f,0);
	PrimitiveNode* p = new PrimitiveNode();

	if(r > 0) {
		p->createPolygon();
		for(int i=0; i<f; i++)
			p->appendVertex(c.at(i));
	}

	return p;
}
