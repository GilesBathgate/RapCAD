/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2019 Giles Bathgate
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

CircleModule::CircleModule(Reporter& r) : PrimitiveModule(r,"circle")
{
	addDescription(tr("Constructs a circle. It will be placed centered on the xy plane."));
	addParameter("radius",tr("The radius of the circle."));
}

Node* CircleModule::evaluate(const Context& ctx) const
{
	auto* rValue=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));

	decimal r=1;
	if(rValue) {
		r=rValue->getNumber();
	} else {
		NumberValue* dValue = dynamic_cast<NumberValue*>(ctx.getArgument(1,"diameter"));
		if(dValue)
			r=(dValue->getNumber()/2.0);
	}

	Fragment* fg = Fragment::createFragment(ctx);
	int f = fg->getFragments(r);
	delete fg;

	QList<Point> c = getCircle(r,f,0);

	auto* pn=new PrimitiveNode(reporter);
	Primitive* p=pn->createPrimitive();
	pn->setChildren(ctx.getInputNodes());

	int i=0;
	if(r > 0) {
		Polygon* pg=p->createPolygon();
		for(const auto& pt: c) {
			p->createVertex(pt);
			pg->append(i++);
		}
	}

	return pn;
}
