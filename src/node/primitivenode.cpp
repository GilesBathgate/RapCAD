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

#include "primitivenode.h"
#include "polyhedron.h"

PrimitiveNode::PrimitiveNode()
{
	primitive=new Polyhedron();
}

PrimitiveNode::~PrimitiveNode()
{
	delete primitive;
}

Polygon* PrimitiveNode::createPolygon()
{
	return primitive->createPolygon();
}

void PrimitiveNode::appendVertex(decimal x, decimal y, decimal z)
{
	appendVertex(Point(x,y,z));
}

void PrimitiveNode::createVertex(decimal x, decimal y, decimal z)
{
	createVertex(Point(x,y,z));
}

void PrimitiveNode::createVertex(Point p)
{
	primitive->createVertex(p);
}

void PrimitiveNode::appendVertex(Point p)
{
	primitive->appendVertex(p);
}

void PrimitiveNode::prependVertex(Point p)
{
	primitive->prependVertex(p);
}

Primitive* PrimitiveNode::getPrimitive()
{
	return primitive;
}

void PrimitiveNode::accept(NodeVisitor& v)
{
	v.visit(this);
}
