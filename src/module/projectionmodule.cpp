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

#include "projectionmodule.h"
#include "node/projectionnode.h"
#include "node/slicenode.h"
#include "context.h"
#include "booleanvalue.h"

ProjectionModule::ProjectionModule(Reporter& r) : Module(r,"projection")
{
	addDescription(tr("Flattens its children onto the xy plane."));
	addParameter("base",tr("Specifies that only polygons with normals perpendicular to the xy plane be considered."));
}

Node* ProjectionModule::evaluate(const Context& ctx) const
{
	BooleanValue* cut=dynamic_cast<BooleanValue*>(ctx.getArgumentDeprecatedModule(0,"cut","'slice' module",reporter));
	if(cut&&cut->isTrue()) {
		auto* n=new SliceNode();
		n->setChildren(ctx.getInputNodes());
		return n;
	}

	bool base=false;
	auto* baseVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,0));
	if(baseVal)
		base=baseVal->isTrue();


	auto* d = new ProjectionNode();
	d->setChildren(ctx.getInputNodes());
	d->setBase(base);
	return d;
}
