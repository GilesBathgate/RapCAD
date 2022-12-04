/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2022 Giles Bathgate
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

#include "multmatrixmodule.h"
#include "context.h"
#include "node/transformationnode.h"
#include "numbervalue.h"
#include "vectorvalue.h"

MultMatrixModule::MultMatrixModule(Reporter& r) : Module(r,"multmatrix")
{
	addDescription(tr("Multiplies its children with the given 4 by 4 affine transformation matrix."));
	addParameter("matrix","mat4x4",tr("The 4 by 4 affine transformation matrix"));
}

Node* MultMatrixModule::evaluate(const Context& ctx) const
{
	auto* matrixVec=getParameterArgument<VectorValue>(ctx,0);

	auto* n=new TransformationNode();
	n->setChildren(ctx.getInputNodes());

	if(!matrixVec)
		return n;

	auto* matrix=new TransformMatrix();

	int i=0;
	int j=0;
	for(Value* rowVal: matrixVec->getElements()) {
		auto* row=dynamic_cast<VectorValue*>(rowVal);
		if(!row) continue;
		j=0;
		for(Value* colVal: row->getElements()) {
			auto* col=dynamic_cast<NumberValue*>(colVal);
			if(!col) continue;
			matrix->setValue(i,j,col->getNumber());
			if(++j >= 4) break;
		}
		if(++i >= 4) break;
	}

	n->setMatrix(matrix);
	return n;
}
