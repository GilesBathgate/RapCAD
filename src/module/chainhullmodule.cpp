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

#include "chainhullmodule.h"
#include "context.h"
#include "booleanvalue.h"
#include "node/hullnode.h"

ChainHullModule::ChainHullModule(Reporter& r) : Module(r,"chain_hull")
{
	addDescription(tr("Constructs a chained hull of its children."));
	addParameter("closed",tr("Specifies whether to close the chain."));
}

Node* ChainHullModule::evaluate(const Context& ctx) const
{
	bool closed=false;
	auto* bVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,0));
	if(bVal)
		closed=bVal->isTrue();

	auto* h=new HullNode();
	h->setChain(true);
	h->setClosed(closed);
	h->setChildren(ctx.getInputNodes());
	return h;
}
