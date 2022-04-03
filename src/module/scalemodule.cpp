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

#include "scalemodule.h"
#include "context.h"
#include "node/pointsnode.h"
#include "node/transformationnode.h"
#include "vectorvalue.h"

ScaleModule::ScaleModule(Reporter& r) : Module(r,"scale")
{
	addDescription(tr("Scales its children by the given vector."));
	addParameter("size",tr("The factor by which to scale the object."));
	addParameter("reference",tr("A center reference point for the scaling."));
}

Node* ScaleModule::evaluate(const Context& ctx) const
{
	Point s(1,1,1);
	auto* sizeVal=getParameterArgument(ctx,0);
	if(sizeVal) {
		VectorValue& v=sizeVal->toVector(3);
		s=v.getPoint();
	}

	decimal x=s.x();
	decimal y=s.y();
	decimal z=s.z();
	if(x==0.0||y==0.0||z==0.0) {
		auto* pn=new PointsNode();
		pn->createSinglePoint();
		pn->setVisibleChildren(false);
		pn->setChildren(ctx.getInputNodes());
		return pn;
	}

	TransformMatrix* m;
	auto* refVal=dynamic_cast<VectorValue*>(getParameterArgument(ctx,1));
	if(refVal) {
		Point r=refVal->getPoint();

		decimal a=r.x();
		decimal b=r.y();
		decimal c=r.z();

		//Derived reference translation using
		//http://tinyurl.com/nfmph3r
		m = new TransformMatrix(
			x  ,0.0,0.0,(a*x)-a,
			0.0,y  ,0.0,(b*y)-b,
			0.0,0.0,z  ,(c*z)-c,
			0.0,0.0,0.0,1.0
		);
	} else {
		m = new TransformMatrix(
			x  ,0.0,0.0,0.0,
			0.0,y  ,0.0,0.0,
			0.0,0.0,z  ,0.0,
			0.0,0.0,0.0,1.0
		);
		if(x==y && y==z)
			m->setType(TransformType::UniformScaling);
	}

	auto* n=new TransformationNode();
	n->setChildren(ctx.getInputNodes());
	n->setMatrix(m);
	return n;
}
