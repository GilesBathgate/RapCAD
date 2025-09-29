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

#include "datummodule.h"
#include "context.h"
#include "node/transformationnode.h"
#include "numbervalue.h"

DatumModule::DatumModule(Reporter& r) : Module(r,"datum")
{
	addDescription(tr("Creates a plane from which children will be relative to"));
	addParameter("x","num",tr("An yz plane offset by x"));
	addParameter("y","num",tr("An xz plane offset by y"));
	addParameter("z","num",tr("An xy plane offset by z"));
	auxilary=true;
}

Node* DatumModule::evaluate(const Context& ctx) const
{
	auto* xVal=dynamic_cast<NumberValue*>(ctx.getArgument(0,"x"));
	auto* yVal=dynamic_cast<NumberValue*>(ctx.getArgument(0,"y"));
	auto* zVal=dynamic_cast<NumberValue*>(ctx.getArgument(0,"z"));

	auto* n=new TransformationNode();
	n->setChildren(ctx.getInputNodes());

	if(!xVal&&!yVal&&!zVal)
		return n;

	decimal x=0.0;
	if(xVal) {
		x=xVal->getNumber();
		n->setDatumAxis(TransformationNode::Axis::X);
	}

	decimal y=0.0;
	if(yVal) {
		y=yVal->getNumber();
		n->setDatumAxis(TransformationNode::Axis::Y);
	}

	decimal z=0.0;
	if(zVal) {
		z=zVal->getNumber();
		n->setDatumAxis(TransformationNode::Axis::Z);
	}

	auto* m = new TransformMatrix(
		1.0,0.0,0.0,x,
		0.0,1.0,0.0,y,
		0.0,0.0,1.0,z,
		0.0,0.0,0.0,1.0
	);
	m->setType(TransformType::Translation);

	n->setMatrix(m);
	return n;
}
