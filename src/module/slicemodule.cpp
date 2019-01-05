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

#include "slicemodule.h"
#include "context.h"
#include "node/slicenode.h"
#include "numbervalue.h"

SliceModule::SliceModule(Reporter& r) : Module(r,"slice")
{
	addDescription(tr("Slices its children at the given height."));
	addParameter("height",tr("The height at which to slice."));
	addParameter("thickness",tr("The thickness of the slice."));
}

Node* SliceModule::evaluate(const Context& ctx) const
{
	decimal h=0.0;
	auto* height=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(height)
		h=height->getNumber();

	decimal t=0.0;
	auto* thick=dynamic_cast<NumberValue*>(getParameterArgument(ctx,1));
	if(thick)
		t=thick->getNumber();

	auto* d = new SliceNode();
	d->setHeight(h);
	d->setThickness(t);
	d->setChildren(ctx.getInputNodes());
	return d;
}
