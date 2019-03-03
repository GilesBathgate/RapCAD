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

#include "subdivisionmodule.h"
#include "context.h"
#include "node/subdivisionnode.h"
#include "numbervalue.h"

SubDivisionModule::SubDivisionModule(Reporter& r) : Module(r,"subdiv")
{
	addDescription(tr("Sibdivides its children into smoother geometry."));
	addParameter("level",tr("The number of smoothing levels to apply."));
}

Node* SubDivisionModule::evaluate(const Context& ctx) const
{
	int level=0;
	auto* levelVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(levelVal)
		level=levelVal->toInteger();

	auto* d = new SubDivisionNode();
	d->setChildren(ctx.getInputNodes());
	d->setLevel(level);
	return d;
}
