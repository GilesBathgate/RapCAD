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

#include "hullnode.h"

HullNode::HullNode() :
	chain(false),
	closed(false),
	concave(false)
{
}

bool HullNode::getChain() const
{
	return chain;
}

void HullNode::setChain(bool value)
{
	chain=value;
}
bool HullNode::getClosed() const
{
	return closed;
}

void HullNode::setClosed(bool value)
{
	closed=value;
}

bool HullNode::getConcave() const
{
	return concave;
}

void HullNode::setConcave(bool value)
{
	concave = value;
}

void HullNode::accept(NodeVisitor& v)
{
	v.visit(*this);
}
