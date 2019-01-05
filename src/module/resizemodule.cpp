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

#include "resizemodule.h"
#include "context.h"
#include "node/resizenode.h"
#include "vectorvalue.h"
#include "booleanvalue.h"

ResizeModule::ResizeModule(Reporter& r) : Module(r,"resize")
{
	addDescription(tr("Modifies its children to match the given size."));
	addParameter("newsize",tr("The new size of the shape."));
	addParameter("auto",tr("Specifies whether to auto-scale any 0-dimensions"));
}

Node* ResizeModule::evaluate(const Context& ctx) const
{
	Point size(0,0,0);
	auto* sizeVal=dynamic_cast<VectorValue*>(getParameterArgument(ctx,0));
	if(sizeVal)
		size=sizeVal->getPoint();

	bool autoSize=false;
	auto* autoSizeVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,1));
	if(autoSizeVal)
		autoSize=autoSizeVal->isTrue();

	auto* n=new ResizeNode();
	n->setSize(size);
	n->setChildren(ctx.getInputNodes());
	n->setAutoSize(autoSize);
	return n;
}
