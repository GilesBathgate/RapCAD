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

#ifndef SLICENODE_H
#define SLICENODE_H

#include "node.h"

class SliceNode : public Node
{
public:
	SliceNode();
	void setHeight(decimal);
	decimal getHeight() const;
	void setThickness(decimal);
	decimal getThickness() const;
	void accept(NodeVisitor&) override;
private:
	decimal height;
	decimal thickness;
};

#endif // SLICENODE_H
