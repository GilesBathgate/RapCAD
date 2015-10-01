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

#include "alignmodule.h"
#include "context.h"
#include "booleanvalue.h"
#include "vectorvalue.h"
#include "numbervalue.h"
#include "node/alignnode.h"
#include "point.h"

AlignModule::AlignModule() : Module("align")
{
	addParameter("top");
	addParameter("bottom");
	addParameter("north");
	addParameter("south");
	addParameter("east");
	addParameter("west");
}

Node* AlignModule::evaluate(Context* ctx)
{
	BooleanValue* topVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,0,0));
	BooleanValue* bottomVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,1,0));
	BooleanValue* northVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,2,0));
	BooleanValue* southVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,3,0));
	BooleanValue* eastVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,4,0));
	BooleanValue* westVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,5,0));

	QList<AlignNode::Face_t> align;
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

	AlignNode* n=new AlignNode();
	n->setChildren(ctx->getInputNodes());
	n->setAlign(align);

	return n;
}
