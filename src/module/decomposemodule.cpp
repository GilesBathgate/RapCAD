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

#include "decomposemodule.h"
#include "context.h"
#include "node/decomposenode.h"

DecomposeModule::DecomposeModule(Reporter& r) : Module(r,"decompose")
{
	addDescription(tr("Decomposes its children into convex geometry."));
}

Node* DecomposeModule::evaluate(const Context& ctx) const
{
	auto* n=new DecomposeNode();
	n->setChildren(ctx.getInputNodes());
	return n;
}
