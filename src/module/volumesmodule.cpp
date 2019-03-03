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

#include "volumesmodule.h"
#include "context.h"
#include "booleanvalue.h"
#include "numbervalue.h"
#include "node/volumesnode.h"

VolumesModule::VolumesModule(Reporter& r) : Module(r,"volume")
{
	addDescription(tr("Provides information about the volume of its children."));
	addParameter("mass",tr("Specifies that the center of mass also be calculated."));
	auxilary=true;
}

Node* VolumesModule::evaluate(const Context& ctx) const
{
	bool mass=false;
	auto* massVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,0));
	if(massVal)
		mass=massVal->isTrue();

	auto* n=new VolumesNode();
	n->setCalcMass(mass);
	n->setChildren(ctx.getInputNodes());
	return n;
}
