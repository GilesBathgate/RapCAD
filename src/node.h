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

#ifndef NODE_H
#define NODE_H

#include "visitablenode.h"
#include <QList>

class Node : public VisitableNode
{
	using size_type=QList<Node*>::size_type;
public:
	Node() = default;
	~Node() override;
	void addChild(Node*);
	void setChildren(const QList<Node*>&);
	const QList<Node*> getChildren() const;
	size_type childCount() const;
private:
	QList<Node*> children;
};

#endif // NODE_H
