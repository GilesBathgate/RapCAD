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

#include "projectionmodule.h"
#include "node/projectionnode.h"
#include "node/slicenode.h"
#include "context.h"
#include "booleanvalue.h"

ProjectionModule::ProjectionModule() : Module("projection")
{
	addParameter("base");
}

Node* ProjectionModule::evaluate(Context* ctx)
{
	BooleanValue* cut=dynamic_cast<BooleanValue*>(ctx->getArgumentDeprecatedModule(0,"cut","'slice' module"));
	if(cut&&cut->isTrue()) {
		SliceNode* n=new SliceNode();
		n->setChildren(ctx->getInputNodes());
		return n;
	}

	bool base=false;
	BooleanValue* baseVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,0));
	if(baseVal)
		base=baseVal->isTrue();


	ProjectionNode* d = new ProjectionNode();
	d->setChildren(ctx->getInputNodes());
	d->setBase(base);
	return d;
}
