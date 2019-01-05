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

#include "childmodule.h"
#include "context.h"
#include "numbervalue.h"

ChildModule::ChildModule(Reporter& r) : Module(r,"child")
{
	addDeprecated(tr("The child module is deprecated, use the children module instead."));
	addParameter("index",tr("The index of the child to use."));
}

Node* ChildModule::evaluate(const Context& ctx) const
{
	int index=0;
	auto* indexValue = dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(indexValue)
		index=indexValue->toInteger();

	return ctx.lookupChild(index);
}
