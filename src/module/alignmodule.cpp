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

#include "alignmodule.h"
#include "context.h"
#include "booleanvalue.h"
#include "vectorvalue.h"
#include "numbervalue.h"
#include "node/alignnode.h"
#include "point.h"

AlignModule::AlignModule(Reporter& r) : Module(r,"align")
{
	addDescription(tr("Aligns its children to the given faces."));
	addParameter("top",tr("Specifies alignment to the top face."));
	addParameter("bottom",tr("Specifies alignment to the bottom face."));
	addParameter("north",tr("Specifies alignment to the north face."));
	addParameter("south",tr("Specifies alignment to the south face."));
	addParameter("east",tr("Specifies alignment to the east face."));
	addParameter("west",tr("Specifies alignment to the west face."));
}

Node* AlignModule::evaluate(const Context& ctx) const
{
	QList<AlignNode::Face_t> align;
	VectorValue* vecVal=dynamic_cast<VectorValue*>(ctx.getArgument(0,"anchor"));
	if(vecVal) {
		Point p = vecVal->getPoint();
		decimal x=p.x(),y=p.y(),z=p.z();
		if(x>0) {
			align.append(AlignNode::North);
		} else if(x<0) {
			align.append(AlignNode::South);
		} else {
			align.append(AlignNode::North);
			align.append(AlignNode::South);
		}
		if(y>0) {
			align.append(AlignNode::West);
		} else if(y<0) {
			align.append(AlignNode::East);
		} else {
			align.append(AlignNode::East);
			align.append(AlignNode::West);
		}
		if(z>0) {
			align.append(AlignNode::Top);
		} else if(z<0) {
			align.append(AlignNode::Bottom);
		} else {
			align.append(AlignNode::Top);
			align.append(AlignNode::Bottom);
		}
	} else {

		auto* topVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,0,0));
		auto* bottomVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,1,0));
		auto* northVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,2,0));
		auto* southVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,3,0));
		auto* eastVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,4,0));
		auto* westVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,5,0));

		if(topVal&&topVal->isTrue()) {
			align.append(AlignNode::Top);
		}
		if(bottomVal&&bottomVal->isTrue()) {
			align.append(AlignNode::Bottom);
		}
		if(northVal&&northVal->isTrue()) {
			align.append(AlignNode::North);
		}
		if(southVal&&southVal->isTrue()) {
			align.append(AlignNode::South);
		}
		if(eastVal&&eastVal->isTrue()) {
			align.append(AlignNode::East);
		}
		if(westVal&&westVal->isTrue()) {
			align.append(AlignNode::West);
		}
	}

	auto* n=new AlignNode();
	n->setChildren(ctx.getInputNodes());
	n->setAlign(align);

	return n;
}
