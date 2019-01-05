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

#include "spheremodule.h"
#include "numbervalue.h"
#include "rmath.h"
#include "node/pointsnode.h"

SphereModule::SphereModule(Reporter& r) : PrimitiveModule(r,"sphere")
{
	addDescription(tr("Constructs a sphere. The sphere will be centered at the origin."));
	addParameter("radius",tr("The radius of the sphere."));
}

Node* SphereModule::evaluate(const Context& ctx) const
{
	auto* rValue=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	decimal r=0.0;
	if(rValue) {
		r=rValue->getNumber();
	} else {
		NumberValue* dValue = dynamic_cast<NumberValue*>(ctx.getArgument(0,"diameter"));
		if(dValue)
			r=(dValue->getNumber()/2.0);
	}
	if(r==0.0)
		return new PointsNode();

	Fragment* fg = Fragment::createFragment(ctx);
	int f = fg->getFragments(r);
	delete fg;

	int ringCount=f/2;

	auto* pn=new PrimitiveNode(reporter);
	Primitive* p=pn->createPrimitive();
	pn->setChildren(ctx.getInputNodes());

	for(auto i=0; i<ringCount; ++i) {
		decimal phi = (r_pi()*(i+0.5)) / ringCount;
		decimal r2 = r*r_sin(phi);
		decimal z = r*r_cos(phi);
		QList<Point> c = getCircle(r2,f,z);
		for(const auto& pt: c) {
			p->createVertex(pt);
		}
	}

	Polygon* pg=p->createPolygon();
	for(auto i=0; i<f; ++i) {
		pg->append(i);
	}

	for(auto i=0; i<ringCount-1; ++i) {
		int i1=i*f;
		int i2=(i+1)*f;
		for(auto j=0; j<f; ++j) {
			int j2=(j+1)%f;

			int o=j+i1;
			int k=j2+i1;
			int m=j2+i2;
			int l=j+i2;

			pg=p->createPolygon();
			pg->append(k);
			pg->append(o);
			pg->append(l);

			pg=p->createPolygon();
			pg->append(l);
			pg->append(m);
			pg->append(k);
		}
	}

	pg=p->createPolygon();
	for(auto i=f*ringCount; i>f*(ringCount-1); i--) {
		pg->append(i-1);
	}

	return pn;
}
