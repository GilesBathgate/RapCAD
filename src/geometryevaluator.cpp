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
#include "polyhedron.h"

GeometryEvaluator::GeometryEvaluator()
{
}

static Primitive* evaluateChildren(Node* n)
{
	GeometryEvaluator g;
	n->accept(g);
	return g.getResult();
}

template <typename Function>
static QFuture<Primitive*> reduceChildren(const Node& n,Function f)
{
	const auto& children=n.getChildren();
	return QtConcurrent::mappedReduced<Primitive*>(children,evaluateChildren,f,QtConcurrent::SequentialReduce);
}

static Primitive* groupChildren(const Node& n)
{
	auto r=reduceChildren(n,[](auto& p,auto c) {
		p=p?p->group(c):c;
	});
	return r.result();
}

void GeometryEvaluator::visit(const PrimitiveNode& n)
{
	result=QtConcurrent::run([&n]() {
		return n.getPrimitive();
	});
}

void GeometryEvaluator::visit(const TriangulateNode& n)
{
	result=QtConcurrent::run([&n]() {
		Primitive* p=groupChildren(n);
		return p?p->triangulate():nullptr;
	});
}

void GeometryEvaluator::visit(const MaterialNode& n)
{
	result=QtConcurrent::run([&n]() {
		auto* p=groupChildren(n);
		Primitive* ph=new Polyhedron();
		ph->appendChild(p);
		return ph;
	});
}

void GeometryEvaluator::visit(const DiscreteNode& n)
{
	result=QtConcurrent::run([&n]() {
		Primitive* p=groupChildren(n);
		p->discrete(n.getPlaces());
		return p;
	});
}

void GeometryEvaluator::visit(const UnionNode& n)
{
	result=reduceChildren(n,[](auto& p,auto c) {
		p=p?p->join(c):c;
	});
}

void GeometryEvaluator::visit(const GroupNode& n)
{
	result=reduceChildren(n,[](auto& p,auto c) {
		p=p?p->group(c):c;
	});
}

void GeometryEvaluator::visit(const DifferenceNode& n)
{
	result=reduceChildren(n,[](auto& p,auto c) {
		p=p?p->difference(c):c;
	});
}

void GeometryEvaluator::visit(const IntersectionNode& n)
{
	result=reduceChildren(n,[](auto& p,auto c) {
		p=p?p->intersection(c):c;
	});
}

void GeometryEvaluator::visit(const SymmetricDifferenceNode& n)
{
	result=reduceChildren(n,[](auto& p,auto c) {
		p=p?p->symmetric_difference(c):c;
	});
}

void GeometryEvaluator::visit(const MinkowskiNode& n)
{
	result=reduceChildren(n,[](auto& p,auto c) {
		p=p?p->minkowski(c):c;
	});
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

void GeometryEvaluator::visit(const TransformationNode& n)
{
	result=QtConcurrent::run([&n]() {
		Primitive* p=groupChildren(n);
		if(p) p->transform(n.getMatrix());
		return p;
	});

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

Primitive* GeometryEvaluator::getResult() const
{
	return result.result();
}
