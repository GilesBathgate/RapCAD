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

#include "cubemodule.h"
#include "context.h"
#include "vectorvalue.h"
#include "node/primitivenode.h"

CubeModule::CubeModule(Reporter& r) : PrimitiveModule(r,"cube")
{
	addDescription(tr("Constructs a cube or cuboid. It will be placed in the first octant unless the center parameter is true."));
	addParameter("size",tr("The size of the cube. A single value or x,y,z"));
	addParameter("center",tr("Specifies whether to center the cube at the origin"));
}

Node* CubeModule::evaluate(const Context& ctx) const
{
	Value* sizeVal=getParameterArgument(ctx,0);
	Value* centerVal=getParameterArgument(ctx,1);
	bool center=false;
	if(centerVal)
		center = centerVal->isTrue();

	Point pt(1.0,1.0,1.0);
	if(sizeVal) {
		VectorValue* size=sizeVal->toVector(3);
		pt = size->getPoint();
	}

	auto* pn=new PrimitiveNode(reporter);
	Primitive* p=pn->createPrimitive();
	pn->setChildren(ctx.getInputNodes());

	decimal x1=0,y1=0,z1=0;
	createCuboid<Point>(p,x1,pt.x(),y1,pt.y(),z1,pt.z());

	if(center) {
		auto* an=new AlignNode();
		an->setCenter(true);
		an->addChild(pn);
		return an;
	}

	return pn;

}
