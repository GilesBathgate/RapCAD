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

#include "boundarymodule.h"
#include "context.h"
#include "node/boundarynode.h"

BoundaryModule::BoundaryModule(Reporter& r, bool l) :
	Module(r,l?"outline":"boundary"),
	legacy(l)
{
	if(l)
		addDeprecated(tr("Creates the outline of its children."));
	else
		addDescription(tr("Creates the boundary of its children."));
}

OnceOnly BoundaryModule::depricateWarning;

Node* BoundaryModule::evaluate(const Context& ctx) const
{
	if(legacy&&depricateWarning())
		reporter.reportWarning(tr("'outline' module is deprecated please use 'boundary'\n"));

	auto* n = new BoundaryNode();
	n->setChildren(ctx.getInputNodes());
	return n;
}
