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

#ifndef TRANSFORMATIONNODE_H
#define TRANSFORMATIONNODE_H

#include "node.h"
#include "transformmatrix.h"

class TransformationNode : public Node
{
public:
	enum class Axis {
		None,
		X,
		Y,
		Z
	};
	TransformationNode();
	~TransformationNode();
	void accept(NodeVisitor&) override;
	TransformMatrix* getMatrix() const;
	void setMatrix(TransformMatrix*);
	Axis getDatumAxis() const;
	void setDatumAxis(const Axis&);
private:
	TransformMatrix* matrix;
	Axis datumAxis;
};

#endif // TRANSFORMATIONNODE_H
