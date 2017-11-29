/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2017 Giles Bathgate
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

#include "shearmodule.h"
#include "vectorvalue.h"
#include "context.h"
#include "node/transformationnode.h"

ShearModule::ShearModule(Reporter* r) : Module(r,"shear")
{
	addDescription(tr("Shears its children in the given planes."));
	addParameter("x",tr("The yz plane."));
	addParameter("y",tr("The xz plane."));
	addParameter("z",tr("The xy plane."));
}

Node* ShearModule::evaluate(Context* ctx)
{
	Point sx(0,0,0);
	VectorValue* xVal=dynamic_cast<VectorValue*>(ctx->getArgument(0,"x"));
	if(xVal)
		sx=xVal->getPoint();

	Point sy(0,0,0);
	VectorValue* yVal=dynamic_cast<VectorValue*>(ctx->getArgument(0,"y"));
	if(yVal)
		sy=yVal->getPoint();

	Point sz(0,0,0);
	VectorValue* zVal=dynamic_cast<VectorValue*>(ctx->getArgument(0,"z"));
	if(zVal)
		sz=zVal->getPoint();

	decimal sxy=sx.y(),sxz=sx.y();
	decimal syx=sy.x(),syz=sy.z();
	decimal szx=sz.x(),szy=sz.y();

	auto* m = new TransformMatrix(
		1,sxy,sxz,0,
		syx,1,syz,0,
		szx,szy,1,0,
		0,0,0,1
	);

	auto* n=new TransformationNode();
	n->setMatrix(m);

	n->setChildren(ctx->getInputNodes());
	return n;
}
