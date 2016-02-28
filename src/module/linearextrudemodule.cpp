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

#include "linearextrudemodule.h"
#include "context.h"
#include "node/linearextrudenode.h"
#include "numbervalue.h"

LinearExtrudeModule::LinearExtrudeModule(Reporter* r) : Module(r,"linear_extrude")
{
	addDescription(tr("Extrudes its children along the z axis."));
	addParameter("height",tr("The height of the extrusion."));
}

Node* LinearExtrudeModule::evaluate(Context* ctx)
{
	decimal h=1.0;
	NumberValue* height=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(height)
		h=height->getNumber();

	LinearExtrudeNode* d = new LinearExtrudeNode();
	d->setHeight(h);
	d->setChildren(ctx->getInputNodes());
	return d;
}
