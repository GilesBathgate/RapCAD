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

#include "translatemodule.h"
#include "context.h"
#include "vectorvalue.h"
#include "node/transformationnode.h"

TranslateModule::TranslateModule(Reporter& r) : Module(r,"translate")
{
	addDescription(tr("Moves its children along the given vector."));
	addParameter("vector",tr("The vector to move along"));
}

Node* TranslateModule::evaluate(const Context& ctx) const
{
	Point v(0,0,0);
	auto* vec=dynamic_cast<VectorValue*>(getParameterArgument(ctx,0));

	auto* n=new TransformationNode();
	n->setChildren(ctx.getInputNodes());

	if(!vec)
		return n;

	v=vec->getPoint();
	auto* m = new TransformMatrix(
		1,0,0,v.x(),
		0,1,0,v.y(),
		0,0,1,v.z(),
		0,0,0,1
	);

	n->setMatrix(m);
	return n;
}
