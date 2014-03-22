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

#include "radialsmodule.h"
#include "context.h"
#include "numbervalue.h"
#include "node/radialsnode.h"

RadialsModule::RadialsModule() : Module("radial")
{
	auxilary=true;
	addParameter("precision");
}

Node* RadialsModule::evaluate(Context* ctx)
{
	NumberValue* precVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));

	RadialsNode* n=new RadialsNode();
	if(precVal)
		n->setPrecision(precVal->getNumber());

	n->setChildren(ctx->getInputNodes());
	return n;
}
