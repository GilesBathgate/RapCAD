/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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

#include "module/tapermodule.h"
#include "context.h"
#include "node/tapernode.h"
#include "numbervalue.h"

TaperModule::TaperModule(Reporter& r) : Module(r,"taper")
{
	addDescription(tr("Tapers 3d polyhedra by offsetting base polygons."));
	addParameter("amount","num",tr("The amount to taper."));
}

Node* TaperModule::evaluate(const Context& ctx) const
{
	decimal a=1.0;
	auto* amountVal=getParameterArgument<NumberValue>(ctx,0);
	if(amountVal)
		a=amountVal->getNumber();

	auto* n = new TaperNode(*this);
	n->setAmount(a);
	n->setChildren(ctx.getInputNodes());
	return n;
}
