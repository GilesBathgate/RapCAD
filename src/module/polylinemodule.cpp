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

#include "polylinemodule.h"
#include "node/polylinenode.h"
#include "vectorvalue.h"
#include "numbervalue.h"

PolylineModule::PolylineModule() : Module("polyline")
{
	addParameter("points");
	addParameter("lines");
}

Node* PolylineModule::evaluate(Context* ctx)
{
	VectorValue* pointsVec=dynamic_cast<VectorValue*>(getParameterArgument(ctx,0));
	VectorValue* linesVec=dynamic_cast<VectorValue*>(getParameterArgument(ctx,1));

	PolylineNode* p=new PolylineNode();

	if(!pointsVec)
		return p;

	QList<Value*> points=pointsVec->getChildren();

	foreach(Value* point, points) {
		VectorValue* pointVec=dynamic_cast<VectorValue*>(point);
		if(pointVec) {
			Point pt = pointVec->getPoint();
			p->createVertex(pt);

		}
	}

	/* If we are just given a single argument of points
	 * build a polyline from that. */
	if(!linesVec) {
		Polygon* pg=p->createPolygon();
		for(int i=0; i<points.length(); ++i)
			pg->append(i);
		return p;
	}

	/* Otherwise use the lines argument to describe the multiple
	 * polylines */
	QList<Value*> lines=linesVec->getChildren();

	foreach(Value* line,lines) {
		VectorValue* lineVec=dynamic_cast<VectorValue*>(line);
		if(lineVec) {
			Polygon* pg=p->createPolygon();
			foreach(Value* indexVal,lineVec->getChildren()) {
				NumberValue* indexNum=dynamic_cast<NumberValue*>(indexVal);
				if(indexNum) {
					int index = indexNum->getNumber();
					if(index>=0&&index<points.count()) {
						pg->append(index);
					}
				}
			}
		}
	}

	return p;
}
