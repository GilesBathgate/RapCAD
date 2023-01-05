/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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

#include "polygonmodule.h"
#include "context.h"
#include "node/primitivenode.h"
#include "numbervalue.h"
#include "vectorvalue.h"

PolygonModule::PolygonModule(Reporter& r,bool polygon) :
	Module(r,polygon?"polygon":"polyline"),
	type(polygon?PrimitiveTypes::Surface:PrimitiveTypes::Lines)
{
	addDescription(polygon?tr("Constructs a polygon."):tr("Constructs a line connecting multiple points."));
	addParameter("points","list",tr("The vertices are provided by the points list."));
	addParameter("lines","list",tr("The lines are a list of indices to the vertices."));
}

Node* PolygonModule::evaluate(const Context& ctx) const
{
	auto* pointsVec=getParameterArgument<VectorValue>(ctx,0);
	VectorValue* linesVec=dynamic_cast<VectorValue*>(ctx.getArgumentDeprecated(1,"lines","paths",reporter));

	auto* pn=new PrimitiveNode();
	Primitive* p=pn->createPrimitive();
	p->setType(type);
	p->setSanitized(false);
	pn->setChildren(ctx.getInputNodes());

	if(!pointsVec)
		return pn;

	const QList<Value*> points=pointsVec->getElements();
	if(points.isEmpty())
		return pn;

	int count=0;
	for(Value* point: points) {
		auto* pointVec=dynamic_cast<VectorValue*>(point);
		if(!pointVec) continue;
		Point pt = pointVec->getPoint();
		p->createVertex(pt);
		++count;
	}

	/* If we are just given a single argument of points
	 * build a polygon from that. */
	if(!linesVec) {
		Polygon& pg=p->createPolygon();
		for(auto i=0; i<count; ++i)
			pg.append(i);
		return pn;
	}

	/* Otherwise use the lines argument to describe the multiple
	 * polygons */
	QList<Value*> lines=linesVec->getElements();

	//This is to remove the need for duplicate vector syntax in the lines argument
	// e.g. lines=[[0,1,2,3]] can just be writtern as lines=[0,1,2,3]
	if(!lines.isEmpty()) {
		auto* single=dynamic_cast<VectorValue*>(lines.at(0));
		if(!single) {
			lines.clear();
			lines.append(linesVec);
		}
	}

	for(Value* line: qAsConst(lines)) {
		auto* lineVec=dynamic_cast<VectorValue*>(line);
		if(!lineVec) continue;
		Polygon& pg=p->createPolygon();
		for(Value* indexVal: lineVec->getElements()) {
			auto* indexNum=dynamic_cast<NumberValue*>(indexVal);
			if(!indexNum) continue;
			int index = indexNum->toInteger();
			if(index>=0&&index<count) {
				pg.append(index);
			}
		}
	}

	return pn;
}
