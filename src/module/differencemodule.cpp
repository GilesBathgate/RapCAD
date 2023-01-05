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

#include "differencemodule.h"
#include "context.h"
#include "node/differencenode.h"

DifferenceModule::DifferenceModule(Reporter& r) : Module(r,"difference")
{
	addDescription(tr("Subtracts from the first child its subsequent children."));
	addExample("difference(){\n%1cube(10);\n%1translate([5,5,5])cube(10);\n}");
}

Node* DifferenceModule::evaluate(const Context& ctx) const
{
	auto* d = new DifferenceNode();
	d->setChildren(ctx.getInputNodes());
	return d;
}
