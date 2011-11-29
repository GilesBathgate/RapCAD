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

#include "polylinemodule.h"
#include "node/polylinenode.h"
#include "vectorvalue.h"

PolylineModule::PolylineModule() : Module("polyline")
{
}

Node* PolylineModule::evaluate(Context* ctx)
{
	VectorValue* points=dynamic_cast<VectorValue*>(ctx->getArgument(0,"points"));

	PolylineNode* p=new PolylineNode();

	Polygon polyline;
	QList<Value*> children = points->getChildren();
	foreach(Value* point, children) {
		VectorValue* pointVec=dynamic_cast<VectorValue*>(point);
		Point pt = pointVec->getPoint();
		polyline.append(pt);
	}
	p->setPoints(polyline);
	return p;
}
