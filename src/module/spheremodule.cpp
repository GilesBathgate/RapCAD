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

#include "spheremodule.h"
#include "context.h"
#include "node/pointsnode.h"
#include "numbervalue.h"
#include "rmath.h"

SphereModule::SphereModule(Reporter& r) : PrimitiveModule(r,"sphere")
{
	addDescription(tr("Constructs a sphere. The sphere will be centered at the origin."));
	addParameter("radius","num",tr("The radius of the sphere."));
}

Node* SphereModule::evaluate(const Context& ctx) const
{
	auto* rValue=getParameterArgument<NumberValue>(ctx,0);
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
		p->createSinglePoint();
		p->setChildren(ctx.getInputNodes());
		return p;
	}

	const int f = Fragment::getFragments(ctx,r);

	const int ringCount=f/2;

	auto* pn=new PrimitiveNode();
	Primitive* p=pn->createPrimitive();
	pn->setChildren(ctx.getInputNodes());

	for(auto i=0; i<ringCount; ++i) {
		const decimal& phi = (r_pi()*(i+0.5)) / ringCount;
		const decimal& r2 = r*r_sin(phi);
		const decimal& z = r*r_cos(phi);
		const QList<Point> c = getCircle(r2,f,z);
		for(const auto& pt: c) {
			p->createVertex(pt);
		}
	}

	Polygon& pg0=p->createPolygon();
	for(auto i=0; i<f; ++i) {
		pg0.append(i);
	}

	for(auto i=0; i<ringCount-1; ++i) {
		const int i1=i*f;
		const int i2=(i+1)*f;
		for(auto j=0; j<f; ++j) {
			const int j2=(j+1)%f;

			const int o=j+i1;
			const int k=j2+i1;
			const int m=j2+i2;
			const int l=j+i2;

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
