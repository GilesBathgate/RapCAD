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

#include "rotateextrudenode.h"

RotateExtrudeNode::RotateExtrudeNode() :
	axis(0,0,0),
	fragments(nullptr)
{
}

RotateExtrudeNode::~RotateExtrudeNode()
{
	delete fragments;
}

decimal RotateExtrudeNode::getRadius() const
{
	return radius;
}

void RotateExtrudeNode::setRadius(const decimal& r)
{
	radius=r;
}

decimal RotateExtrudeNode::getSweep() const
{
	return sweep;
}

void RotateExtrudeNode::setSweep(const decimal& value)
{
	sweep = value;
}

decimal RotateExtrudeNode::getHeight() const
{
	return height;
}

void RotateExtrudeNode::setHeight(const decimal& value)
{
	height = value;
}

Point RotateExtrudeNode::getAxis() const
{
	return axis;
}

void RotateExtrudeNode::setAxis(const Point& value)
{
	axis = value;
}

Fragment* RotateExtrudeNode::getFragments() const
{
	return fragments;
}

void RotateExtrudeNode::setFragments(Fragment* f)
{
	fragments=f;
}

void RotateExtrudeNode::accept(NodeVisitor& v)
{
	v.visit(*this);
}
