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

#include "glidemodule.h"
#include "context.h"
#include "node/glidenode.h"
#include "value.h"

GlideModule::GlideModule(Reporter* r) : Module(r,"glide")
{
	addDescription(tr("Glides its subsequent children along the outline of the first child."));
	addParameter("closed",tr("Specfies whether to close the outline."));
}

Node* GlideModule::evaluate(Context* ctx)
{
	bool close=false;
	Value* closeVal=getParameterArgument(ctx,0);
	if(closeVal)
		close=closeVal->isTrue();

	GlideNode* n=new GlideNode();
	n->setClosed(close);
	n->setChildren(ctx->getInputNodes());
	return n;
}
