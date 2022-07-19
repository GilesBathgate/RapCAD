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

#include "conemodule.h"
#include "context.h"
#include "node/alignnode.h"
#include "numbervalue.h"
#include "rmath.h"

ConeModule::ConeModule(Reporter& r) : PrimitiveModule(r,"cone")
{
	addDescription(tr("Constructs a cone. It is placed centered on the xy plane."));
	addParameter("height",tr("The height of the cone."));
	addParameter("radius1",tr("The radius of the bottom of the cone."));
	addParameter("radius2",tr("The radius of the top of the cone."));
	addParameter("center",tr("Specifies whether the cone should be vertically centered along the z axis."));
}

Node* ConeModule::evaluate(const Context& ctx) const
{
	auto* heightValue = dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	auto* r1Value = dynamic_cast<NumberValue*>(getParameterArgument(ctx,1));
	auto* r2Value = dynamic_cast<NumberValue*>(getParameterArgument(ctx,2));
	Value* centerValue = getParameterArgument(ctx,3);

	decimal h=1.0;
	if(heightValue)
		h=heightValue->getNumber();

	decimal r1=0.0;
	decimal r2=0.0;
	if(r1Value) {
		r1=r1Value->getNumber();
	} else {
		NumberValue* d1Value = dynamic_cast<NumberValue*>(ctx.getArgument(1,"diameter1"));
		if(d1Value)
			r1=(d1Value->getNumber()/2.0);
	}
	if(r2Value) {
		r2=r2Value->getNumber();
	} else {
		NumberValue* d2Value = dynamic_cast<NumberValue*>(ctx.getArgument(2,"diameter2"));
		if(d2Value)
			r2=(d2Value->getNumber()/2.0);
	}

	bool center = false;
	if(centerValue)
		center=centerValue->isTrue();

	decimal z1=0.0;
	decimal z2=h;

	decimal r=r_max(r1,r2);

	Fragment* fg = Fragment::createFragment(ctx);
	int f = fg->getFragments(r);
	delete fg;

	const QList<Point> c1=getCircle(r1,f,z1);
	const QList<Point> c2=getCircle(r2,f,z2);

	auto* pn=new PrimitiveNode();
	Primitive* p=pn->createPrimitive();
	pn->setChildren(ctx.getInputNodes());

	if(r1<=0.0&&r2<=0.0)
		return pn;

	int n=0;
	if(r1>0.0) {
		Polygon& pg=p->createPolygon();
		for(const auto& pt: c1) {
			p->createVertex(pt);
			pg.append(n++);
		}
	}

	if(h==0.0)
		return pn;

	if(r2>0.0) {
		Polygon& pg=p->createPolygon();
		for(const auto& pt: c2) {
			p->createVertex(pt);
			pg.prepend(n++);
		}
	}

	/* In the cases where r1 or r2 are 0,  n will now convinently be pointing
	 * one past the end, and point to the apex as defined here when needed */
	if(r1<=0.0)
		p->createVertex(Point(0.0,0.0,z1));
	if(r2<=0.0)
		p->createVertex(Point(0.0,0.0,z2));

	for(auto i=0; i<f; ++i) {
		int j=(i+1)%f;

		int k=r2<=0.0?n:i;
		int l=r1<=0.0?n:j;

		if(r1>0.0&&r2>0.0) {
			k+=f;
			j+=f;
		}

		if(r1 == r2) {
			createQuad(p,i,k,j,l);
		} else {
			if(r1>0.0) {
				createTriangle(p,i,k,l);
			}
			if(r2>0.0) {
				createTriangle(p,j,l,k);
			}
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
