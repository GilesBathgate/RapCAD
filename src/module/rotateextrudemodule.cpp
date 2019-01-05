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

#include "rotateextrudemodule.h"
#include "node/rotateextrudenode.h"
#include "node/transformationnode.h"
#include "numbervalue.h"
#include "vectorvalue.h"

RotateExtrudeModule::RotateExtrudeModule(Reporter& r) : PrimitiveModule(r,"rotate_extrude")
{
	addDescription(tr("Extrudes its children about the given axis."));
	addParameter("angle",tr("The sweep angle for the extrusion."));
	addParameter("axis",tr("The axis of rotation. When no axis is given the shape is first rotated 90° about the x axis, then extruded about the z axis."));
	addParameter("radius",tr("The radius of the extrusion."));
	addParameter("height",tr("The helical height of the extrusion."));
}

Node* RotateExtrudeModule::evaluate(const Context& ctx) const
{
	decimal angle=360.0;
	auto* angleVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(angleVal)
		angle=angleVal->getNumber();

	bool compatible=true;
	Point axis(0,0,1);
	auto* vecVal=dynamic_cast<VectorValue*>(getParameterArgument(ctx,1));
	if(vecVal) {
		axis=vecVal->getPoint();
		compatible=false;
	}

	decimal radius=0.0;
	auto* radiusVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,2));
	if(radiusVal)
		radius=radiusVal->getNumber();

	decimal height=0.0;
	auto* heightVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,3));
	if(heightVal)
		height=heightVal->getNumber();

	auto* n=new RotateExtrudeNode();
	n->setSweep(angle);
	n->setAxis(axis);
	n->setRadius(radius);
	n->setHeight(height);

	Fragment* fg = Fragment::createFragment(ctx);
	n->setFragments(fg);

	if(compatible) {
		//if no axis is given we fall into legacy compatibility mode
		auto* Rx90=new TransformMatrix(1,0,0,0,0,0,-1,0,0,1,0,0,0,0,0,1);
		auto* t=new TransformationNode();
		t->setMatrix(Rx90);
		t->setChildren(ctx.getInputNodes());
		n->addChild(t);
	} else {
		n->setChildren(ctx.getInputNodes());
	}

	return n;
}
