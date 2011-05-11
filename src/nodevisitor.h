/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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

#ifndef NODEVISITOR_H
#define NODEVISITOR_H

class NodeVisitor
{
public:
	virtual ~NodeVisitor() {}
	virtual void visit(class PrimitiveNode*)=0;
	virtual void visit(class UnionNode*)=0;
	virtual void visit(class DifferenceNode*)=0;
	virtual void visit(class IntersectionNode*)=0;
	virtual void visit(class SymmetricDifferenceNode*)=0;
	virtual void visit(class MinkowskiNode*)=0;
	virtual void visit(class HullNode*)=0;
	virtual void visit(class LinearExtrudeNode*)=0;
	virtual void visit(class BoundsNode*)=0;
	virtual void visit(class SubDivisionNode*)=0;
	virtual void visit(class TransformationNode*)=0;
};

#endif // NODEVISITOR_H
