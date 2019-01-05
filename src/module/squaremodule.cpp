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

#include "squaremodule.h"
#include "vectorvalue.h"

SquareModule::SquareModule(Reporter& r) : PrimitiveModule(r,"square")
{
	addDescription(tr("Constructs a square or rectangle. It will be placed in the first quadrant."));
	addParameter("size",tr("The size of the square"));
	addParameter("center",tr("Specifies whether to center the square on the xy plane"));
}

Node* SquareModule::evaluate(const Context& ctx) const
{
	Value* sizeVal=getParameterArgument(ctx,0);
	Value* centerVal=getParameterArgument(ctx,1);
	bool center=false;
	if(centerVal)
		center = centerVal->isTrue();

	decimal x=1.0,y=1.0;
	if(sizeVal) {
		VectorValue* size=sizeVal->toVector(2);
		Point pt = size->getPoint();
		x=pt.x(); y=pt.y();
	}

	auto* pn=new PrimitiveNode(reporter);
	Primitive* p=pn->createPrimitive();
	pn->setChildren(ctx.getInputNodes());

	Polygon* pg=p->createPolygon();
	p->createVertex(Point(0, 0, 0));
	p->createVertex(Point(x, 0, 0));
	p->createVertex(Point(x, y, 0));
	p->createVertex(Point(0, y, 0));
	pg->append(0);
	pg->append(1);
	pg->append(2);
	pg->append(3);

	if(center) {
		auto* an=new AlignNode();
		an->setCenter(true);
		an->addChild(pn);
		return an;
	}

	return pn;
}
