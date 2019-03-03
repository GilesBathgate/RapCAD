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

#include "offsetmodule.h"
#include "context.h"
#include "node/offsetnode.h"
#include "numbervalue.h"

OffsetModule::OffsetModule(Reporter& r) : Module(r,"offset")
{
	addDescription(tr("Moves the outlines of polygons outward or inward by a given amount."));
	addParameter("delta",tr("The amount to offset the polygon. The polygon is offset inwards, when negative."));
}

Node* OffsetModule::evaluate(const Context& ctx) const
{
	decimal a=1.0;
	auto* amountVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(amountVal)
		a=amountVal->getNumber();

	auto* n = new OffsetNode();
	n->setAmount(a);
	n->setChildren(ctx.getInputNodes());
	return n;
}
