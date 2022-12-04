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

#include "pointsmodule.h"
#include "context.h"
#include "node/pointsnode.h"
#include "vectorvalue.h"

PointsModule::PointsModule(Reporter& r, bool multiple) : Module(r,multiple?"points":"point")
{
	addDescription(tr("Constructs points."));
	addParameter("points","list",tr("The location of the points."));
}

Node* PointsModule::evaluate(const Context& ctx) const
{
	auto* pointsVal=getParameterArgument<VectorValue>(ctx,0);

	QList<Point> points;
	if(pointsVal) {
		for(Value* child: pointsVal->getElements()) {
			auto* loc=dynamic_cast<VectorValue*>(child);
			if(!loc) {
				points.append(pointsVal->getPoint());
				break;
			}
			points.append(loc->getPoint());
		}
	}

	auto* n=new PointsNode();
	n->setChildren(ctx.getInputNodes());
	if(points.isEmpty()) {
		n->createSinglePoint();
	} else {
		Primitive* cp=n->createPrimitive();
		cp->setType(PrimitiveTypes::Points);
		for(const auto& p: points)
			cp->createVertex(p);
	}
	return n;
}
