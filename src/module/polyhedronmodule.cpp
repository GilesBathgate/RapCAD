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

#include "polyhedronmodule.h"
#include "context.h"
#include "vectorvalue.h"
#include "numbervalue.h"
#include "node/primitivenode.h"

PolyhedronModule::PolyhedronModule(Reporter* r) : PrimitiveModule(r,"polyhedron")
{
	addDescription(tr("Construct a polyhedron. Special care must be taken to ensure the correct winding order."));
	addParameter("points",tr("The vertices of the shape are provided by the points list"));
	addParameter("faces",tr("The faces is list of indices to the vertices. These relate to the facets of the polyhedron."));
}

Node* PolyhedronModule::evaluate(Context* ctx)
{
	VectorValue* points=dynamic_cast<VectorValue*>(getParameterArgument(ctx,0));
	VectorValue* faces=dynamic_cast<VectorValue*>(ctx->getArgumentDeprecated(1,"faces","triangles",reporter));

	PrimitiveNode* p=new PrimitiveNode(reporter);
	p->setChildren(ctx->getInputNodes());

	if(!points||!faces)
		return p;

	QList<Value*> children = points->getChildren();
	foreach(Value* child,children) {
		VectorValue* point=dynamic_cast<VectorValue*>(child);
		if(point) {
			Point pt = point->getPoint();
			p->createVertex(pt);
		}
	}
	foreach(Value* s,faces->getChildren()) {
		Polygon* pg=p->createPolygon();
		VectorValue* surface=dynamic_cast<VectorValue*>(s);
		foreach(Value* indexVal,surface->getChildren()) {
			NumberValue* indexNum=dynamic_cast<NumberValue*>(indexVal);
			if(indexNum) {
				int index = indexNum->toInteger();
				if(index>=0&&index<children.count()) {
					pg->append(index);
				}
			}
		}

	}

	return p;

}
