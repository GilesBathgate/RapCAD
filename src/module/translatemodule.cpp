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

#include "translatemodule.h"
#include "context.h"
#include "vectorvalue.h"
#include "node/transformationnode.h"

TranslateModule::TranslateModule(Reporter* r) : Module(r,"translate")
{
	addDescription(tr("Moves its children along the given vector."));
	addParameter("vector",tr("The vector to move along"));
}

Node* TranslateModule::evaluate(Context* ctx)
{
	Point v;
	VectorValue* vec=dynamic_cast<VectorValue*>(getParameterArgument(ctx,0));
	if(vec)
		v=vec->getPoint();

	decimal x=0,y=0,z=0;
	v.getXYZ(x,y,z);

	TransformMatrix* m = new TransformMatrix(
		1,0,0,x,
		0,1,0,y,
		0,0,1,z,
		0,0,0,1
	);

	TransformationNode* n=new TransformationNode();
	n->setMatrix(m);

	n->setChildren(ctx->getInputNodes());
	return n;
}
