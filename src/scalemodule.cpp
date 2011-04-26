/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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

#include "scalemodule.h"
#include "vectorvalue.h"
#include "transformationnode.h"

ScaleModule::ScaleModule() : Module("scale")
{
}

Node* ScaleModule::evaluate(Context* ctx,QList<Node*> childs)
{
	Point vec;
	VectorValue* vecVal=dynamic_cast<VectorValue*>(ctx->getArgument(0,"vector"));
	if(vecVal)
		vec=vecVal->getPoint();

	double x=0,y=0,z=0;
	vec.getXYZ(x,y,z);

	double m[16] = {
		x,0,0,0,
		0,y,0,0,
		0,0,z,0,
		0,0,0,1
	};

	TransformationNode* n=new TransformationNode();
	for(int i=0; i<16; i++)
		n->matrix[i]=m[i];

	n->setChildren(childs);
	return n;
}
