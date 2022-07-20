/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2022 Giles Bathgate
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

#include "regularpolygonmodule.h"
#include "context.h"
#include "node/primitivenode.h"
#include "rmath.h"

RegularPolygonModule::RegularPolygonModule(Reporter& r) : PrimitiveModule(r, "regular_polygon")
{
	addDescription(tr("Constructs a regular polygon. It will be placed centered on the xy plane."));
	addParameter("sides",tr("The number of size to the polygon."));
	addParameter("apothem",tr("The radius from the center to the outer faces of the polygon."));
}

Node* RegularPolygonModule::evaluate(const Context& ctx) const
{
	int s=3;
	auto* sidesVal = dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(sidesVal)
		s=sidesVal->toInteger();

	auto* pn=new PrimitiveNode();
	Primitive* p=pn->createPrimitive();
	p->setType(PrimitiveTypes::Surface);
	pn->setChildren(ctx.getInputNodes());

	if(s<=0.0)
		return pn;

	decimal r=1.0;
	decimal a=1.0;
	auto* apothemVal = dynamic_cast<NumberValue*>(getParameterArgument(ctx,1));
	if(apothemVal) {
		a=apothemVal->getNumber();
		r=a/r_cos(r_pi()/s);
	} else {
		NumberValue* radiusVal = dynamic_cast<NumberValue*>(ctx.getArgument(1,"radius"));
		if(radiusVal) {
			r=radiusVal->getNumber();
			a=r*r_cos(r_pi()/s);
		}
	}

	const QList<Point> p1=getPolygon(a,r,s,0.0);

	if(r>0.0) {
		int n=0;
		Polygon& pg0=p->createPolygon();
		for(const auto& pt: p1) {
			p->createVertex(pt);
			pg0.append(n++);
		}
	}

	return pn;
}


