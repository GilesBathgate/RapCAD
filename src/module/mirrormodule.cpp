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

#include "mirrormodule.h"
#include "context.h"
#include "vectorvalue.h"
#include "node/transformationnode.h"
#include "rmath.h"

MirrorModule::MirrorModule(Reporter& r) : Module(r,"mirror")
{
	addDescription(tr("Mirrors its children through a plane perpendicular to the given normal vector."));
	addParameter("vector",tr("The normal vector of the mirror plane."));
}

Node* MirrorModule::evaluate(const Context& ctx) const
{
	Point p(0,0,0);
	auto* vecVal=dynamic_cast<VectorValue*>(getParameterArgument(ctx,0));

	auto* n=new TransformationNode();
	n->setChildren(ctx.getInputNodes());

	if(!vecVal)
		return n;

	p = vecVal->getPoint();
	decimal x=p.x(), y=p.y(), z=p.z();
	decimal mag = r_sqrt(x*x + y*y + z*z);
	if(mag==0)
		return n;

	decimal u = x/mag;
	decimal v = y/mag;
	decimal w = z/mag;

	auto* m = new TransformMatrix(
		1-2*u*u,-2*v*u,-2*w*u,0,
		-2*u*v,1-2*v*v,-2*w*v,0,
		-2*u*w,-2*v*w,1-2*w*w,0,
		0,     0,      0,1
	);

	n->setMatrix(m);
	return n;
}
