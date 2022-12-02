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

#include "alignmodule.h"
#include "booleanvalue.h"
#include "context.h"
#include "node/alignnode.h"
#include "vectorvalue.h"

AlignModule::AlignModule(Reporter& r) : Module(r,"align")
{
	addDescription(tr("Aligns its children to the given faces."));
	addParameter("top","bool",tr("Specifies alignment to the top face."));
	addParameter("bottom","bool",tr("Specifies alignment to the bottom face."));
	addParameter("north","bool",tr("Specifies alignment to the north face."));
	addParameter("south","bool",tr("Specifies alignment to the south face."));
	addParameter("east","bool",tr("Specifies alignment to the east face."));
	addParameter("west","bool",tr("Specifies alignment to the west face."));
}

Node* AlignModule::evaluate(const Context& ctx) const
{
	QList<ViewDirections> align;
	VectorValue* vecVal=dynamic_cast<VectorValue*>(ctx.getArgument(0,"anchor"));
	if(vecVal) {
		Point p = vecVal->getPoint();
		decimal x=p.x();
		decimal y=p.y();
		decimal z=p.z();
		if(x>0.0) {
			align.append(ViewDirections::East);
		} else if(x<0.0) {
			align.append(ViewDirections::West);
		} else {
			align.append(ViewDirections::East);
			align.append(ViewDirections::West);
		}
		if(y>0.0) {
			align.append(ViewDirections::North);
		} else if(y<0.0) {
			align.append(ViewDirections::South);
		} else {
			align.append(ViewDirections::North);
			align.append(ViewDirections::South);
		}
		if(z>0.0) {
			align.append(ViewDirections::Top);
		} else if(z<0.0) {
			align.append(ViewDirections::Bottom);
		} else {
			align.append(ViewDirections::Top);
			align.append(ViewDirections::Bottom);
		}
	} else {

		auto* topVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,0,0));
		auto* bottomVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,1,0));
		auto* northVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,2,0));
		auto* southVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,3,0));
		auto* eastVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,4,0));
		auto* westVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,5,0));

		if(topVal&&topVal->isTrue()) {
			align.append(ViewDirections::Top);
		}
		if(bottomVal&&bottomVal->isTrue()) {
			align.append(ViewDirections::Bottom);
		}
		if(northVal&&northVal->isTrue()) {
			align.append(ViewDirections::North);
		}
		if(southVal&&southVal->isTrue()) {
			align.append(ViewDirections::South);
		}
		if(eastVal&&eastVal->isTrue()) {
			align.append(ViewDirections::East);
		}
		if(westVal&&westVal->isTrue()) {
			align.append(ViewDirections::West);
		}
	}

	auto* n=new AlignNode();
	n->setChildren(ctx.getInputNodes());
	n->setAlign(align);

	return n;
}
