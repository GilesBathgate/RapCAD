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

#include "cylindersurfacemodule.h"
#include "context.h"
#include "node/alignnode.h"
#include "numbervalue.h"

CylinderSurfaceModule::CylinderSurfaceModule(Reporter& r) : PrimitiveModule(r,"cylinder_surface")
{
	addDescription(tr("Constructs the surface of a cylinder without top and bottom facets."));
	addParameter("height","num",tr("The height of the cylinder surface."));
	addParameter("radius","num",tr("The radius of the cylinder surface."));
	addParameter("center","bool",tr("Specifies whether to center the cylinder along the z axis."));
}


Node* CylinderSurfaceModule::evaluate(const Context& ctx) const
{
	auto* heightValue=getParameterArgument<NumberValue>(ctx,0);
	decimal h=1.0;
	if(heightValue)
		h=heightValue->getNumber();

	auto* rValue=getParameterArgument<NumberValue>(ctx,1);
	decimal r=1.0;
	if(rValue)
		r=rValue->getNumber();

	auto* centerValue=getParameterArgument<Value>(ctx,2);
	bool center=false;

	if(centerValue)
		center=centerValue->isTrue();

	decimal z1=0.0;
	decimal z2=h;

	int f = Fragment::getFragments(ctx,r);

	const QList<Point> c1=getCircle(r,f,z1);
	const QList<Point> c2=getCircle(r,f,z2);

	auto* pn=new PrimitiveNode();
	Primitive* p=pn->createPrimitive();
	p->setType(PrimitiveTypes::Surface);
	pn->setChildren(ctx.getInputNodes());

	for(const auto& pt: c1) {
		p->createVertex(pt);
	}

	for(const auto& pt: c2) {
		p->createVertex(pt);
	}

	for(auto i=0; i<f; ++i) {
		int j=(i+1)%f;
		int k=i+f;
		int l=j+f;
		createQuad(p,i,k,l,j);
	}

	if(center) {
		auto* an=new AlignNode();
		an->setCenterVertical();
		an->addChild(pn);
		return an;
	}

	return pn;
}
