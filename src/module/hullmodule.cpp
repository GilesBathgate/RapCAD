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

#include "hullmodule.h"
#include "context.h"
#include "node/hullnode.h"
#include "booleanvalue.h"

HullModule::HullModule(Reporter& r) : Module(r,"hull")
{
	addDescription(tr("Creates a boundry shape from the points of its children."));
	addParameter("concave",tr("Determines whether the hull may be concave"));
}

Node* HullModule::evaluate(const Context& ctx) const
{
	bool concave=false;
	auto* concaveVal = dynamic_cast<BooleanValue*>(getParameterArgument(ctx,0));
	if(concaveVal)
		concave=concaveVal->isTrue();

	auto* d = new HullNode();
	d->setConcave(concave);
	d->setChildren(ctx.getInputNodes());
	return d;
}
