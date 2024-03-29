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

#ifndef ROTATEEXTRUDENODE_H
#define ROTATEEXTRUDENODE_H

#include "node.h"
#include "fragment.h"
#include "point.h"

class RotateExtrudeNode : public Node
{
	Q_DISABLE_COPY_MOVE(RotateExtrudeNode)
public:
	RotateExtrudeNode();
	~RotateExtrudeNode() override;
	decimal getRadius() const;
	void setRadius(const decimal&);
	decimal getSweep() const;
	void setSweep(const decimal&);
	decimal getHeight() const;
	void setHeight(const decimal&);
	Fragment* getFragments() const;
	void setFragments(Fragment*);
	Point getAxis() const;
	void setAxis(const Point&);
	void accept(NodeVisitor&) override;

private:
	decimal radius;
	decimal height;
	decimal sweep;
	Point axis;
	Fragment* fragments;
};

#endif // ROTATEEXTRUDENODE_H
