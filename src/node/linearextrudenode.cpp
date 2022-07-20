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

#include "linearextrudenode.h"

LinearExtrudeNode::LinearExtrudeNode() :
	axis(0,0,0)
{
}

void LinearExtrudeNode::setHeight(const decimal& h)
{
	height=h;
}

decimal LinearExtrudeNode::getHeight() const
{
	return height;
}

Point LinearExtrudeNode::getAxis() const
{
	return axis;
}

void LinearExtrudeNode::setAxis(const Point& value)
{
	axis = value;
}

void LinearExtrudeNode::accept(NodeVisitor& v)
{
	v.visit(*this);
}
