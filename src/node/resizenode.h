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

#ifndef RESIZENODE_H
#define RESIZENODE_H

#include "node.h"
#include "point.h"

class ResizeNode : public Node
{
public:
	ResizeNode();
	void accept(NodeVisitor&) override;
	void setSize(const Point&);
	Point getSize() const;
	void setAutoSize(bool);
	bool getAutoSize() const;
private:
	Point size;
	bool autosize;
};

#endif // RESIZENODE_H
