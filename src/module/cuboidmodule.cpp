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
#include "cuboidmodule.h"
#include "context.h"
#include "cubemodule.h"

CuboidModule::CuboidModule(Reporter& r) : PrimitiveModule(r,"cuboid")
{
	addDescription(tr("Constructs a cuboid. It will be placed in the first octant"));
	addParameter("width","num",tr("The width (x) of the cuboid."));
	addParameter("depth","num",tr("The depth (y) of the cuboid."));
	addParameter("height","num",tr("The height (z) of the cuboid."));
}

Node* CuboidModule::evaluate(const Context& ctx) const
{
	auto* widthVal=getParameterArgument<NumberValue>(ctx,0);
	auto* depthVal=getParameterArgument<NumberValue>(ctx,1);
	auto* heightVal=getParameterArgument<NumberValue>(ctx,2);

	decimal w=widthVal?widthVal->getNumber():1.0;
	decimal d=depthVal?depthVal->getNumber():1.0;
	decimal h=heightVal?heightVal->getNumber():1.0;

	auto* pn=new PrimitiveNode();
	Primitive* p=pn->createPrimitive();
	pn->setChildren(ctx.getInputNodes());

	CubeModule::createCuboid<Point,decimal>(p,0.0,w,0.0,d,0.0,h);

	return pn;
}
