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

#include "nodeevaluator.h"

#include "cachemanager.h"
#include "polyhedron.h"

#ifdef USE_CGAL
#include "cgalauxiliarybuilder.h"
#include "cgalimport.h"
#include "cgalprimitive.h"
#endif

NodeEvaluator::NodeEvaluator(Reporter& r) :
	reporter(r),
	result(nullptr)
{
	auto& m=CacheManager::getInstance();
	cache=m.getCache();
}

Primitive* NodeEvaluator::createPrimitive()
{
#ifdef USE_CGAL
	return new CGALPrimitive();
#else
	return new Polyhedron();
#endif
}

void NodeEvaluator::visit(const PrimitiveNode& n)
{
	Primitive* cp=n.getPrimitive();
	cp=cache->fetch(cp);
	if(!evaluate(n,Operations::Union,cp)) return;
}

void NodeEvaluator::visit(const TriangulateNode& n)
{
	if(!evaluate(n,Operations::Union)) return;

	result=result->triangulate();
}

void NodeEvaluator::visit(const MaterialNode& n)
{
	if(!evaluate(n,Operations::Union)) return;
	auto* p=new Polyhedron();
	p->appendChild(result);
	result=p;
}

void NodeEvaluator::visit(const DiscreteNode& n)
{
	if(!evaluate(n,Operations::Union)) return;

	result->discrete(n.getPlaces());
}

void NodeEvaluator::visit(const UnionNode& op)
{
	if(!evaluate(op,Operations::Union)) return;
}

void NodeEvaluator::visit(const GroupNode& op)
{
	if(!evaluate(op,Operations::Group)) return;
}

void NodeEvaluator::visit(const DifferenceNode& op)
{
	const int count=op.childCount();
	if(count>2)
		reporter.reportWarning(tr("%1 children in difference may compute faster with union/group").arg(count));
	if(!evaluate(op,Operations::Difference)) return;
}

void NodeEvaluator::visit(const IntersectionNode& op)
{
	if(!evaluate(op,Operations::Intersection)) return;
}

void NodeEvaluator::visit(const SymmetricDifferenceNode& op)
{
	if(!evaluate(op,Operations::SymmetricDifference)) return;
}

void NodeEvaluator::visit(const MinkowskiNode& op)
{
	if(!evaluate(op,Operations::Minkowski)) return;
}

void NodeEvaluator::visit(const GlideNode& op)
{
	if(!evaluate(op,Operations::Glide)) return;
}

void NodeEvaluator::visit(const HullNode& n)
{
	if(n.getChain()) {
		Primitive* first=nullptr;
		Primitive* previous=nullptr;
		for(Node* c: n.getChildren()) {
			c->accept(*this);
			if(!previous) {
				first=result;
			} else {
				first=first->chain_hull(previous,result);
				first->appendChild(result);
			}

			previous=result;
		}
		if(first) {
			if(n.getClosed())
				first=first->chain_hull(first,previous);

			result=first->combine();
		}
	} else {
		auto* cp=createPrimitive();
		for(Node* c: n.getChildren()) {
			c->accept(*this);
			if(result)
				cp->appendChild(result);
		}

		result=cp->hull(n.getConcave());
	}
}

void NodeEvaluator::visit(const LinearExtrudeNode& op)
{
	if(!evaluate(op,Operations::Union)) return;

	result=result->linear_extrude(op.getHeight(),op.getAxis());
}

void NodeEvaluator::visit(const RotateExtrudeNode& op)
{
	if(!evaluate(op,Operations::Union)) return;

	if(result->isFullyDimentional()) {
		reporter.reportWarning(tr("Rotate extrude for volume not implemented"));
		return noResult(op);
	}
#ifdef USE_CGAL
	const CGAL::Vector3 axis(CGAL::ORIGIN,op.getAxis());
	if(axis.squared_length()==0.0) {
		reporter.reportWarning(tr("Invalid rotation axis specified"));
		return noResult(op);
	}
#endif
	result=result->rotate_extrude(op.getHeight(),op.getRadius(),op.getSweep(),op.getFragments(),op.getAxis());
}

bool NodeEvaluator::evaluate(const Node& op,Operations type)
{
	return evaluate(op,type,nullptr);
}

bool NodeEvaluator::evaluate(const Node& op,Operations type,Primitive* first)
{
	return evaluate(op.getChildren(),type,first);
}

bool NodeEvaluator::evaluate(const QList<Node*>& children, Operations type, Primitive* first)
{
	for(Node* n: children) {
		n->accept(*this);
		if(!first) {
			first=result;
		} else if(result) {
			switch(type) {
				case Operations::Group:
					first->groupLater(result);
					break;
				case Operations::Union:
					first->joinLater(result);
					break;
				case Operations::Difference:
					first=first->difference(result);
					break;
				case Operations::Intersection:
					first=first->intersection(result);
					break;
				case Operations::SymmetricDifference:
					first=first->symmetric_difference(result);
					break;
				case Operations::Minkowski:
					first=first->minkowski(result);
					break;
				case Operations::Glide:
					first=first->glide(result);
					break;
			}
		}
	}

	if(first)
		result=first->combine();
	else
		result=first;

	return (result!=nullptr);
}

void NodeEvaluator::noResult(const Node&)
{
	if(result)
		delete result;
	result=nullptr;
}

void NodeEvaluator::visit(const BoundsNode& n)
{
	if(!evaluate(n,Operations::Union)) return;
#ifdef USE_CGAL
	CGALAuxiliaryBuilder b(reporter);
	result=b.buildBoundsPrimitive(result);
#else
	return noResult(n);
#endif
}

void NodeEvaluator::visit(const SubDivisionNode& n)
{
	if(!evaluate(n,Operations::Union)) return;

	result=result->subdivide(n.getLevel());
}

void NodeEvaluator::visit(const NormalsNode& n)
{
	if(!evaluate(n,Operations::Union)) return;
#ifdef USE_CGAL
	CGALAuxiliaryBuilder b(reporter);
	result=b.buildNormalsPrimitive(result);
#else
	return noResult(n);
#endif
}

void NodeEvaluator::visit(const SimplifyNode& n)
{
	if(!evaluate(n,Operations::Union)) return;

	result=result->simplify(n.getRatio());
}

void NodeEvaluator::visit(const SolidNode& n)
{
	if(!evaluate(n,Operations::Union)) return;

	result=result->solidify();
}

static void appendChildren(Primitive* p,const QList<Node*> children)
{
	for(auto* child: children) {
		auto* pn=dynamic_cast<PrimitiveNode*>(child);
		if(pn)
			p->appendChild(pn->getPrimitive());
		if(child)
			appendChildren(p,child->getChildren());
	}
}

void NodeEvaluator::visit(const ChildrenNode& n)
{
	if(n.getIndexes().isEmpty()) {
		if(!evaluate(n,Operations::Union)) return;
	} else {
		const QList<Node*>& allChildren=n.getChildren();
		const QList<int>& indexes=n.getIndexes();
		QList<Node*> children;
		QList<Node*> others;
		for(auto i=0; i<allChildren.count(); ++i) {
			Node* child=allChildren.at(i);
			if(indexes.contains(i)) {
				children.append(child);
			} else {
				others.append(child);
			}
		}

		if(!evaluate(children,Operations::Union,nullptr)) return;

		appendChildren(result,others);
	}
}

void NodeEvaluator::visit(const OffsetNode& n)
{
	if(!evaluate(n,Operations::Union)) return;

	if(result->isFullyDimentional()) {
		reporter.reportWarning(tr("Offset for volume not implemented"));
		return noResult(n);
	}
	result=result->inset(n.getAmount());
}

void NodeEvaluator::visit(const BoundaryNode& op)
{
	if(!evaluate(op,Operations::Union)) return;

	result=result->boundary();
}

void NodeEvaluator::visit(const ImportNode& op)
{
#ifdef USE_CGAL
	const QFileInfo file(op.getImport());
	const CGALImport i(file,reporter);
	result=i.import();
#else
	return noResult(op);
#endif
}

void NodeEvaluator::visit(const TransformationNode& tr)
{
	if(!evaluate(tr,Operations::Union)) return;
#ifdef USE_CGAL
	using Axis = TransformationNode::Axis;
	const Axis axis=tr.getDatumAxis();
	if(axis!=Axis::None) {
		CGALAuxiliaryBuilder b(reporter);
		result=b.buildDatumsPrimitive(result,axis);
	}
#endif
	result->transform(tr.getMatrix());
}

void NodeEvaluator::visit(const ResizeNode& n)
{
	if(!evaluate(n,Operations::Union)) return;

	result->resize(n.getAutoSize(),n.getSize());
}

void NodeEvaluator::visit(const AlignNode& n)
{
	if(!evaluate(n,Operations::Union)) return;

	result->align(n.getCenter(),n.getAlign());
}

void NodeEvaluator::visit(const PointsNode& n)
{
	Primitive* cp=n.getPrimitive();
	if(n.getVisibleChildren()) {
		if(!evaluate(n,Operations::Union,cp)) return;
	} else {
		appendChildren(cp,n.getChildren());
		result=cp;
	}
}

void NodeEvaluator::visit(const SliceNode& n)
{
	if(!evaluate(n,Operations::Union)) return;

	result=result->slice(n.getHeight(),n.getThickness());
}

void NodeEvaluator::visit(const ProductNode& p)
{
	Primitive* prim = p.getPrimitive();
	result=prim->copy();
}

void NodeEvaluator::visit(const ProjectionNode& op)
{
	if(!evaluate(op,Operations::Union)) return;

	result=result->projection(op.getBase());
}

void NodeEvaluator::visit(const DecomposeNode& n)
{
	if(!evaluate(n,Operations::Union)) return;

	result=result->decompose();
}

void NodeEvaluator::visit(const ComplementNode& n)
{
	if(!evaluate(n,Operations::Union)) return;

	result=result->complement();
}

void NodeEvaluator::visit(const RadialsNode& n)
{
	if(!evaluate(n,Operations::Union)) return;
#ifdef USE_CGAL
	CGALAuxiliaryBuilder b(reporter);
	result=b.buildRadialsPrimitive(result);
#else
	return noResult(n);
#endif
}

void NodeEvaluator::visit(const VolumesNode& n)
{
	if(!evaluate(n,Operations::Union)) return;
#ifdef USE_CGAL
	CGALAuxiliaryBuilder b(reporter);
	result=b.buildVolumesPrimitive(result,n.getCalcMass());
#else
	return noResult(n);
#endif
}

Primitive* NodeEvaluator::getResult() const
{
	return result;
}
