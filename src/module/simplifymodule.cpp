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

#include "simplifymodule.h"
#include "context.h"
#include "node/simplifynode.h"
#include "numbervalue.h"

SimplifyModule::SimplifyModule(Reporter& r) : Module(r,"simplify")
{
	addDescription(tr("Performs a mesh simplification on its children."));
	addParameter("ratio",tr("The mesh simplification stops when the number of edges is below the ratio of the initial number of edges"));
}

Node* SimplifyModule::evaluate(const Context& ctx) const
{
	decimal ratio=0.1;
	auto* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal)
		ratio=numVal->getNumber();

	auto* n=new SimplifyNode();
	n->setRatio(ratio);
	n->setChildren(ctx.getInputNodes());
	return n;
}
