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

#ifndef NODEVISITOR_H
#define NODEVISITOR_H

class NodeVisitor
{
public:
	virtual ~NodeVisitor() {}
	virtual void visit(const class PrimitiveNode&)=0;
	virtual void visit(const class UnionNode&)=0;
	virtual void visit(const class GroupNode&)=0;
	virtual void visit(const class DifferenceNode&)=0;
	virtual void visit(const class IntersectionNode&)=0;
	virtual void visit(const class SymmetricDifferenceNode&)=0;
	virtual void visit(const class MinkowskiNode&)=0;
	virtual void visit(const class GlideNode&)=0;
	virtual void visit(const class HullNode&)=0;
	virtual void visit(const class LinearExtrudeNode&)=0;
	virtual void visit(const class RotateExtrudeNode&)=0;
	virtual void visit(const class BoundsNode&)=0;
	virtual void visit(const class SubDivisionNode&)=0;
	virtual void visit(const class OffsetNode&)=0;
	virtual void visit(const class BoundaryNode&)=0;
	virtual void visit(const class ImportNode&)=0;
	virtual void visit(const class TransformationNode&)=0;
	virtual void visit(const class ResizeNode&)=0;
	virtual void visit(const class AlignNode&)=0;
	virtual void visit(const class PointsNode&)=0;
	virtual void visit(const class SliceNode&)=0;
	virtual void visit(const class ProductNode&)=0;
	virtual void visit(const class ProjectionNode&)=0;
	virtual void visit(const class DecomposeNode&)=0;
	virtual void visit(const class ComplementNode&)=0;
	virtual void visit(const class RadialsNode&)=0;
	virtual void visit(const class VolumesNode&)=0;
	virtual void visit(const class TriangulateNode&)=0;
	virtual void visit(const class MaterialNode&)=0;
	virtual void visit(const class DiscreteNode&)=0;
	virtual void visit(const class NormalsNode&)=0;
	virtual void visit(const class SimplifyNode&)=0;
	virtual void visit(const class ChildrenNode&)=0;
};

#endif // NODEVISITOR_H
