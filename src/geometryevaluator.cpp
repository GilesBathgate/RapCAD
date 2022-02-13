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

#ifdef USE_CGAL
#include "cgalprimitive.h"
#include "cgalimport.h"
#endif

GeometryEvaluator::GeometryEvaluator(Reporter& r) :
	reporter(r)
{
	if(setThreads())
		QThreadPool::globalInstance()->setMaxThreadCount(100);
}

OnceOnly GeometryEvaluator::setThreads;

static Primitive* createPrimitive()
{
#ifdef USE_CGAL
	return new CGALPrimitive();
#else
	return new Polyhedron();
#endif
}

class MapFunctor
{
public:
	explicit MapFunctor(Reporter& r) : reporter(r) {}
	Primitive* operator()(Node* n)
	{
		GeometryEvaluator g(reporter);
		n->accept(g);
		return g.getResult();
	}
private:
	Reporter& reporter;
};

QFuture<Primitive*> GeometryEvaluator::reduceChildren(
	const Node& n,ReduceFunction reduce,QtConcurrent::ReduceOptions options)
{
	const auto& children=n.getChildren();
	MapFunction map=MapFunctor(reporter);
	return QtConcurrent::mappedReduced<Primitive*>(children,map,reduce,options);
}

// blocking because we can't apply operations until evaluated
Primitive* GeometryEvaluator::unionChildren(const Node& n)
{
	const auto& children=n.getChildren();
	MapFunction map=MapFunctor(reporter);
	return QtConcurrent::blockingMappedReduced<Primitive*>(children,map,
	[](auto& p,auto c) {
		p=p?p->join(c):c;
	},QtConcurrent::UnorderedReduce);
}

Primitive* GeometryEvaluator::appendChildren(const Node& n)
{
	const auto& children=n.getChildren();
	MapFunction map=MapFunctor(reporter);
	return QtConcurrent::blockingMappedReduced<Primitive*>(children,map,
	[](auto& p,auto c) {
		if(!p) p=createPrimitive();
		p->appendChild(c);
	},QtConcurrent::UnorderedReduce);

	// Ward off unused function warning (Unreachable)
	createPrimitive();
}

void GeometryEvaluator::visit(const PrimitiveNode& n)
{
	result=QtConcurrent::run([&n]() {
		return n.getPrimitive();
	});
}

void GeometryEvaluator::visit(const TriangulateNode& n)
{
	result=QtConcurrent::run([&n,this]() {
		Primitive* p=unionChildren(n);
		return p?p->triangulate():nullptr;
	});
}

void GeometryEvaluator::visit(const MaterialNode& n)
{
	result=QtConcurrent::run([&n,this]() {
		auto* p=unionChildren(n);
		Primitive* ph=new Polyhedron();
		ph->appendChild(p);
		return ph;
	});
}

void GeometryEvaluator::visit(const DiscreteNode& n)
{
	result=QtConcurrent::run([&n,this]() {
		Primitive* p=unionChildren(n);
		p->discrete(n.getPlaces());
		return p;
	});
}

void GeometryEvaluator::visit(const UnionNode& n)
{
	result=reduceChildren(n,[](auto& p,auto c) {
		p=p?p->join(c):c;
	},QtConcurrent::UnorderedReduce);
}

void GeometryEvaluator::visit(const GroupNode& n)
{
	result=reduceChildren(n,[](auto& p,auto c) {
		p=p?p->group(c):c;
	},QtConcurrent::UnorderedReduce);
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

void GeometryEvaluator::visit(const GlideNode& n)
{
	result=reduceChildren(n,[](auto& p,auto c) {
		p=p?p->glide(c):c;
	});
}


void GeometryEvaluator::visit(const HullNode& n)
{
	result=QtConcurrent::run([&n,this]() {
		Primitive* p=appendChildren(n);
		return p?p->hull(n.getConcave()):nullptr;
	});
}

void GeometryEvaluator::visit(const LinearExtrudeNode& n)
{
	result=QtConcurrent::run([&n,this]() {
		Primitive* p=unionChildren(n);
		return p?p->linear_extrude(n.getHeight(),n.getAxis()):nullptr;
	});
}

void GeometryEvaluator::visit(const RotateExtrudeNode& n)
{
	result=QtConcurrent::run([&n,this]() {
		Primitive* p=unionChildren(n);
		return p?p->rotate_extrude(n.getHeight(),n.getRadius(),n.getSweep(),n.getFragments(),n.getAxis()):nullptr;
	});
}

void GeometryEvaluator::visit(const BoundsNode&)
{

}

void GeometryEvaluator::visit(const SubDivisionNode& n)
{
	result=QtConcurrent::run([&n,this](){
		Primitive* p=unionChildren(n);
		return p?p->subdivide(n.getLevel()):nullptr;
	});
}

void GeometryEvaluator::visit(const NormalsNode&)
{
}

void GeometryEvaluator::visit(const SimplifyNode& n)
{
	result=QtConcurrent::run([&n,this](){
		Primitive* p=unionChildren(n);
		return p?p->simplify(n.getRatio()):nullptr;
	});
}

void GeometryEvaluator::visit(const ChildrenNode&)
{
}

void GeometryEvaluator::visit(const OffsetNode& n)
{
	result=QtConcurrent::run([&n,this](){
		Primitive* p=unionChildren(n);
		return p?p->inset(n.getAmount()):nullptr;
	});
}

void GeometryEvaluator::visit(const BoundaryNode& n)
{
	result=QtConcurrent::run([&n,this](){
		Primitive* p=unionChildren(n);
		return p?p->boundary():nullptr;
	});
}

void GeometryEvaluator::visit(const ImportNode& n)
{
	result=QtConcurrent::run([&n,this](){
#ifdef USE_CGAL
		QFileInfo f(n.getImport());
		CGALImport i(f,reporter);
		return i.import();
#else
		return static_cast<Primitive*>(nullptr);
#endif
	});
}

void GeometryEvaluator::visit(const TransformationNode& n)
{
	result=QtConcurrent::run([&n,this]() {
		Primitive* p=unionChildren(n);
		if(p) p->transform(n.getMatrix());
		return p;
	});
}

void GeometryEvaluator::visit(const ResizeNode& n)
{
	result=QtConcurrent::run([&n,this]() {
		Primitive* p=unionChildren(n);
		if(p) p->resize(n.getAutoSize(),n.getSize());
		return p;
	});
}

void GeometryEvaluator::visit(const AlignNode& n)
{
	result=QtConcurrent::run([&n,this](){
		Primitive* p=unionChildren(n);
		if(p) p->align(n.getCenter(),n.getAlign());
		return p;
	});
}

void GeometryEvaluator::visit(const PointsNode& n)
{
	result=QtConcurrent::run([&n](){
		return n.getPrimitive();
	});
}

void GeometryEvaluator::visit(const SliceNode& n)
{
	result=QtConcurrent::run([&n,this](){
		Primitive* p=unionChildren(n);
		return p?p->slice(n.getHeight(),n.getThickness()):nullptr;
	});
}

void GeometryEvaluator::visit(const ProductNode&)
{
}

void GeometryEvaluator::visit(const ProjectionNode& n)
{
	result=QtConcurrent::run([&n,this](){
		Primitive* p=unionChildren(n);
		return p?p->projection(n.getBase()):nullptr;
	});
}

void GeometryEvaluator::visit(const DecomposeNode& n)
{
	result=QtConcurrent::run([&n,this](){
		Primitive* p=unionChildren(n);
		return p?p->decompose():nullptr;
	});
}

void GeometryEvaluator::visit(const ComplementNode& n)
{
	result=QtConcurrent::run([&n,this](){
		Primitive* p=unionChildren(n);
		return p?p->complement():nullptr;
	});
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
