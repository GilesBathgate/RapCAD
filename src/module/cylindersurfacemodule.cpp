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

#include "cylindersurfacemodule.h"
#include "numbervalue.h"
#include "booleanvalue.h"

CylinderSurfaceModule::CylinderSurfaceModule(Reporter& r) : PrimitiveModule(r,"cylinder_surface")
{
	addDescription(tr("Constructs the surface of a cylinder without top and bottom facets."));
	addParameter("height",tr("The height of the cylinder surface."));
	addParameter("radius",tr("The radius of the cylinder surface."));
	addParameter("center",tr("Specifies whether to center the cylinder along the z axis."));
}


Node* CylinderSurfaceModule::evaluate(const Context& ctx) const
{
	auto* heightValue = dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	decimal h=1.0;
	if(heightValue)
		h=heightValue->getNumber();

	auto* rValue = dynamic_cast<NumberValue*>(getParameterArgument(ctx,1));
	decimal r=1.0;
	if(rValue)
		r=rValue->getNumber();

	Value* centerValue=getParameterArgument(ctx,2);
	bool center=false;

	if(centerValue)
		center=centerValue->isTrue();

	decimal z1,z2;
	z1 = 0.0;
	z2 = h;

	Fragment* fg = Fragment::createFragment(ctx);
	int f = fg->getFragments(r);
	delete fg;

	QList<Point> c1=getCircle(r,f,z1);
	QList<Point> c2=getCircle(r,f,z2);

	auto* pn=new PrimitiveNode(reporter);
	Primitive* p=pn->createPrimitive();
	p->setType(Primitive::Surface);
	pn->setChildren(ctx.getInputNodes());

	for(const auto& pt: c1) {
		p->createVertex(pt);
	}

	for(const auto& pt: c2) {
		p->createVertex(pt);
	}

	Polygon* pg;
	for(auto i=0; i<f; ++i) {
		int j=(i+1)%f;
		int k=i+f;
		int l=j+f;
		pg=p->createPolygon();
		pg->append(i);
		pg->append(k);
		pg->append(l);
		pg->append(j);
	}

	if(center) {
		auto* an=new AlignNode();
		an->setCenterVertical();
		an->addChild(pn);
		return an;
	}

	return pn;
}
