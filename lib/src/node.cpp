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

#include "node.h"

#include <QSet>

static void descend(Node* node,QSet<Node*>& collect)
{
	for(auto child: node->getChildren())
	{
		auto size=collect.size();
		collect.insert(child);
		if(collect.size()>size)
			descend(child,collect);
	}
	node->clearChildren();
}

Node::~Node()
{
	if(children.isEmpty()) return;

	QSet<Node*> collect;
	descend(this,collect);
	collect.remove(this);
	qDeleteAll(collect);
}

void Node::addChild(Node* n)
{
	children.append(n);
}

void Node::setChildren(const QList<Node*>& c)
{
	children = c;
}

const QList<Node*>& Node::getChildren() const
{
	return children;
}

Node::size_type Node::childCount() const
{
	return children.count();
}

void Node::clearChildren()
{
	children.clear();
}
