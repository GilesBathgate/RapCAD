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

#include "discretemodule.h"
#include "context.h"
#include "node/discretenode.h"
#include "numbervalue.h"


DiscreteModule::DiscreteModule(Reporter& r) : Module(r,"discrete")
{
	addDescription(tr("Rearranges the vertices of its children to lie on discrete values rounded to the given number of decimal places."));
	addParameter("places",tr("The number of decimal places to which to round."));
}

Node* DiscreteModule::evaluate(const Context& ctx) const
{
	int places=6;
	auto* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal)
		places=numVal->toInteger();

	auto* n=new DiscreteNode();
	n->setPlaces(places);
	n->setChildren(ctx.getInputNodes());
	return n;
}
