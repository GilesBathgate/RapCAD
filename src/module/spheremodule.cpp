/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2021 Giles Bathgate
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
#include "context.h"

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
	if(r==0.0) {
		auto* p=new PointsNode();
		p->setChildren(ctx.getInputNodes());
		return p;
	}

	int f = Fragment::getFragments(ctx,r);

	int ringCount=f/2;

	auto* pn=new PrimitiveNode();
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

	Polygon& pg0=p->createPolygon();
	for(auto i=0; i<f; ++i) {
		pg0.append(i);
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

			Polygon& pg1=p->createPolygon();
			pg1.append(k);
			pg1.append(o);
			pg1.append(l);

			Polygon& pg2=p->createPolygon();
			pg2.append(l);
			pg2.append(m);
			pg2.append(k);
		}
	}

	Polygon& pg3=p->createPolygon();
	for(auto i=f*ringCount; i>f*(ringCount-1); i--) {
		pg3.append(i-1);
	}

	return pn;
}
