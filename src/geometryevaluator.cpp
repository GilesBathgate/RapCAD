/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2021 Giles Bathgate
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
#include "geometryevaluator.h"
#include <QtConcurrent>

GeometryEvaluator::GeometryEvaluator()
{
}

void GeometryEvaluator::visit(const PrimitiveNode&)
{
}

void GeometryEvaluator::visit(const TriangulateNode&)
{
}

void GeometryEvaluator::visit(const MaterialNode&)
{
}

void GeometryEvaluator::visit(const DiscreteNode&)
{
}

void GeometryEvaluator::visit(const UnionNode&)
{
}

void GeometryEvaluator::visit(const GroupNode&)
{
}

void GeometryEvaluator::visit(const DifferenceNode&)
{
}

void GeometryEvaluator::visit(const IntersectionNode&)
{
}

void GeometryEvaluator::visit(const SymmetricDifferenceNode&)
{
}

void GeometryEvaluator::visit(const MinkowskiNode&)
{
}

void GeometryEvaluator::visit(const GlideNode&)
{
}


void GeometryEvaluator::visit(const HullNode&)
{
}

void GeometryEvaluator::visit(const LinearExtrudeNode&)
{

}

void GeometryEvaluator::visit(const RotateExtrudeNode&)
{
}


void GeometryEvaluator::visit(const BoundsNode&)
{

}

void GeometryEvaluator::visit(const SubDivisionNode&)
{

}

void GeometryEvaluator::visit(const NormalsNode&)
{
}

void GeometryEvaluator::visit(const SimplifyNode&)
{
}

void GeometryEvaluator::visit(const ChildrenNode&)
{
}

void GeometryEvaluator::visit(const OffsetNode&)
{
}

void GeometryEvaluator::visit(const BoundaryNode&)
{
}

void GeometryEvaluator::visit(const ImportNode&)
{
}

void GeometryEvaluator::visit(const TransformationNode&)
{
}

void GeometryEvaluator::visit(const ResizeNode&)
{

}

void GeometryEvaluator::visit(const AlignNode&)
{

}

void GeometryEvaluator::visit(const PointsNode&)
{

}

void GeometryEvaluator::visit(const SliceNode&)
{

}

void GeometryEvaluator::visit(const ProductNode&)
{
}

void GeometryEvaluator::visit(const ProjectionNode&)
{
}

void GeometryEvaluator::visit(const DecomposeNode&)
{
}

void GeometryEvaluator::visit(const ComplementNode&)
{
}

void GeometryEvaluator::visit(const RadialsNode&)
{
}

void GeometryEvaluator::visit(const VolumesNode&)
{
}
