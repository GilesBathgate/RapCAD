/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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

MirrorModule::MirrorModule(Reporter* r) : Module(r,"mirror")
{
	addDescription(tr("Mirrors its children through a plane perpendicular to the given normal vector."));
	addParameter("vector",tr("The normal vector of the mirror plane."));
}

Node* MirrorModule::evaluate(Context* ctx)
{
	Point vec;
	VectorValue* vecVal=dynamic_cast<VectorValue*>(getParameterArgument(ctx,0));
	if(vecVal)
		vec=vecVal->getPoint();

	decimal x=0,y=0,z=0;
	vec.getXYZ(x,y,z);

	decimal mag = r_sqrt(x*x + y*y + z*z);
	decimal u = x/mag;
	decimal v = y/mag;
	decimal w = z/mag;

	TransformMatrix* m = new TransformMatrix(
		1-2*u*u,-2*v*u,-2*w*u,0,
		-2*u*v,1-2*v*v,-2*w*v,0,
		-2*u*w,-2*v*w,1-2*w*w,0,
			 0,     0,      0,1
	);

	TransformationNode* n=new TransformationNode();
	n->setMatrix(m);

	n->setChildren(ctx->getInputNodes());
	return n;
}
