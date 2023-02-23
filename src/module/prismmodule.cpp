/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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
#include "context.h"
#include "node/alignnode.h"
#include "node/primitivenode.h"
#include "numbervalue.h"
#include "rmath.h"

PrismModule::PrismModule(Reporter& r) : PrimitiveModule(r,"prism")
{
	addDescription(tr("Constructs a regular prism. It will be placed centered on the xy plane."));
	addParameter("height","num",tr("The height of the prism."));
	addParameter("sides","int",tr("The number of size to the prism."));
	addParameter("apothem","num",tr("The radius from the center to the outer faces of the prism."));
	addParameter("center","bool",tr("Specifies whether to center the prism vertically along the z axis."));
}

Node* PrismModule::evaluate(const Context& ctx) const
{
	auto* heightVal=getParameterArgument<NumberValue>(ctx,0);
	decimal h=1.0;
	if(heightVal)
		h=heightVal->getNumber();

	int s=3;
	auto* sidesVal=getParameterArgument<NumberValue>(ctx,1);
	if(sidesVal)
		s=sidesVal->toInteger();

	auto* pn=new PrimitiveNode();
	Primitive* p=pn->createPrimitive();
	pn->setChildren(ctx.getInputNodes());

	if(h==0.0||s<=0.0)
		return pn;

	decimal r=1.0;
	decimal a=1.0;
	auto* apothemVal=getParameterArgument<NumberValue>(ctx,2);
	if(apothemVal) {
		a=apothemVal->getNumber();
		r=a/r_cos(r_pi()/s);
	} else {
		NumberValue* radiusVal = dynamic_cast<NumberValue*>(ctx.getArgument(2,"radius"));
		if(radiusVal) {
			r=radiusVal->getNumber();
			a=r*r_cos(r_pi()/s);
		}
	}

	auto* centerVal=getParameterArgument<Value>(ctx,3);
	bool center=false;
	if(centerVal)
		center=centerVal->isTrue();

	decimal z1=0.0;
	decimal z2=h;

	const QList<Point> p1=getPolygon(a,r,s,z1);
	const QList<Point> p2=getPolygon(a,r,s,z2);

	if(r>0.0) {
		int n=0;
		Polygon& pg0=p->createPolygon();
		for(const auto& pt: p1) {
			p->createVertex(pt);
			pg0.append(n++);
		}

		Polygon& pg1=p->createPolygon();
		for(const auto& pt: p2) {
			p->createVertex(pt);
			pg1.prepend(n++);
		}

		for(auto i=0; i<s; ++i) {
			int j=(i+1)%s;
			int k=i+s;
			int l=j+s;
			createQuad(p,i,k,l,j);
		}
	}

	if(center) {
		auto* an=new AlignNode();
		an->setCenterVertical();
		an->addChild(pn);
		return an;
	}

	return pn;
}
