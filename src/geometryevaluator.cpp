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
#include "geometryevaluator.h"
#include "polyhedron.h"

#ifdef USE_CGAL
#include "cgalauxiliarybuilder.h"
#include "cgalimport.h"
#include "cgalprimitive.h"
#include <CGAL/exceptions.h>
#endif

class GeometryEvaluator::MapFunctor
{
public:
	MapFunctor(Reporter& r) : reporter(r) {}
	Primitive* operator()(Node* n)
	{
		GeometryEvaluator g(reporter);
		n->accept(g);
		return g.getResult();
	}
private:
	Reporter& reporter;
};

class GeometryEvaluator::ReduceFunctor
{
public:
	explicit ReduceFunctor(Reporter& r,const ReduceFunction& f) :
		reporter(r),function(f) {}
	void operator()(Primitive*& p,Primitive* c)
	{
		try {
			function(p,c);
#ifdef USE_CGAL
		} catch(const CGAL::Failure_exception& e) {
			destroy(p);
			destroy(c);
			reporter.reportException(QString::fromStdString(e.what()));
#endif
		} catch(...) {
			destroy(p);
			destroy(c);
			reporter.reportException();
		}
	}
private:
	static void destroy(Primitive*& p) { delete p; p=nullptr; }
	Reporter& reporter;
	ReduceFunction function;
};

GeometryEvaluator::GeometryEvaluator(Reporter& r) :
	pool(new QThreadPool()),
	reporter(r)
{
}

GeometryEvaluator::~GeometryEvaluator()
{
	delete pool;
}

Primitive* GeometryEvaluator::createPrimitive()
{
#ifdef USE_CGAL
	return new CGALPrimitive();
#else
	return new Polyhedron();
#endif
}

Primitive* GeometryEvaluator::noResult()
{
	return nullptr;
}

QFuture<Primitive*> GeometryEvaluator::reduceChildren(
	const Node& n,const ReduceFunction& function,QtConcurrent::ReduceOptions options)
{
	const auto& children=n.getChildren();
	const MapFunction& map=MapFunctor(reporter);
	const ReduceFunction& reduce=ReduceFunctor(reporter,function);
	return QtConcurrent::mappedReduced<Primitive*>(pool,children,map,reduce,options);
}

// blocking because we can't apply operations until evaluated
Primitive* GeometryEvaluator::unionChildren(const Node& n)
{
	const auto& children=n.getChildren();
	const MapFunction& map=MapFunctor(reporter);
	return QtConcurrent::blockingMappedReduced<Primitive*>(pool,children,map,
	[](auto& p,auto c) {
		p=p?p->join(c):c;
	},QtConcurrent::UnorderedReduce);
}

Primitive* GeometryEvaluator::appendChildren(const Node& n)
{
	const auto& children=n.getChildren();
	const MapFunction& map=MapFunctor(reporter);
	return QtConcurrent::blockingMappedReduced<Primitive*>(pool,children,map,
	[](auto& p,auto c) {
		if(!p) p=createPrimitive();
		p->appendChild(c);
	},QtConcurrent::UnorderedReduce);
}

void GeometryEvaluator::visit(const PrimitiveNode& n)
{
	if(n.childCount()>0) {
		result=reduceChildren(n,[&n](auto& p,auto c) {
			if(!p) p=n.getPrimitive();
			p=p->join(c);
		});
	} else {
		result=QtConcurrent::run(pool,[&n]() {
			return n.getPrimitive();
		});
	}
}

void GeometryEvaluator::visit(const TriangulateNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
		return p?p->triangulate():noResult();
	});
}

void GeometryEvaluator::visit(const MaterialNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		auto* p=unionChildren(n);
		Primitive* ph=new Polyhedron();
		ph->appendChild(p);
		return ph;
	});
}

void GeometryEvaluator::visit(const DiscreteNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
		if(p) p->discrete(n.getPlaces());
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

Primitive* GeometryEvaluator::chainHull(const HullNode& n)
{
	Primitive* first=nullptr;
	Primitive* previous=nullptr;
	for(Node* c: n.getChildren()) {
		GeometryEvaluator g(reporter);
		c->accept(g);
		Primitive* current=g.getResult();
		if(!previous) {
			first=current;
		} else {
			first=first->chain_hull(previous,current);
			first->appendChild(current);
		}
		previous=current;
	}
	if(first) {
		if(n.getClosed())
			first=first->chain_hull(first,previous);
		return first->combine();
	}

	return noResult();
}

void GeometryEvaluator::visit(const HullNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		if(n.getChain()) {
			return chainHull(n);
		} else {
			Primitive* p=appendChildren(n);
			return p?p->hull(n.getConcave()):noResult();
		}
	});
}

void GeometryEvaluator::visit(const LinearExtrudeNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
		return p?p->linear_extrude(n.getHeight(),n.getAxis()):noResult();
	});
}

void GeometryEvaluator::visit(const RotateExtrudeNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
		return p?p->rotate_extrude(n.getHeight(),n.getRadius(),n.getSweep(),n.getFragments(),n.getAxis()):noResult();
	});
}

void GeometryEvaluator::visit(const BoundsNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
#ifdef USE_CGAL
		CGALAuxiliaryBuilder b(reporter);
		return b.buildBoundsPrimitive(p);
#else
		return noResult();
#endif
	});
}

void GeometryEvaluator::visit(const SubDivisionNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
		return p?p->subdivide(n.getLevel()):noResult();
	});
}

void GeometryEvaluator::visit(const NormalsNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
#ifdef USE_CGAL
		CGALAuxiliaryBuilder b(reporter);
		return b.buildNormalsPrimitive(p);
#else
		return noResult();
#endif
	});
}

void GeometryEvaluator::visit(const SimplifyNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
		return p?p->simplify(n.getRatio()):noResult();
	});
}

void GeometryEvaluator::visit(const SolidNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
		return p?p->solidify():noResult();
	});
}

void GeometryEvaluator::visit(const ChildrenNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		//TODO: implement index based selection
		return unionChildren(n);
	});
}

void GeometryEvaluator::visit(const OffsetNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
		return p?p->inset(n.getAmount()):noResult();
	});
}

void GeometryEvaluator::visit(const BoundaryNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
		return p?p->boundary():noResult();
	});
}

void GeometryEvaluator::visit(const ImportNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
#ifdef USE_CGAL
		const QFileInfo f(n.getImport());
		const CGALImport i(f,reporter);
		return i.import();
#else
		return noResult();
#endif
	});
}

void GeometryEvaluator::visit(const TransformationNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
		if(!p) return noResult();
#ifdef USE_CGAL
		using Axis = TransformationNode::Axis;
		const Axis axis=n.getDatumAxis();
		if(axis!=Axis::None) {
			CGALAuxiliaryBuilder b(reporter);
			p=b.buildDatumsPrimitive(p,axis);
		}
#endif
		p->transform(n.getMatrix());
		return p;
	});
}

void GeometryEvaluator::visit(const ResizeNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
		if(p) p->resize(n.getAutoSize(),n.getSize());
		return p;
	});
}

void GeometryEvaluator::visit(const AlignNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
		if(p) p->align(n.getCenter(),n.getAlign());
		return p;
	});
}

void GeometryEvaluator::visit(const PointsNode& n)
{
	result=QtConcurrent::run(pool,[&n]() {
		return n.getPrimitive();
	});
}

void GeometryEvaluator::visit(const SliceNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
		return p?p->slice(n.getHeight(),n.getThickness()):noResult();
	});
}

void GeometryEvaluator::visit(const ProductNode& n)
{
	result=QtConcurrent::run(pool,[&n]() {
		Primitive* p=n.getPrimitive();
		return p?p->copy():noResult();
	});
}

void GeometryEvaluator::visit(const ProjectionNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
		return p?p->projection(n.getBase()):noResult();
	});
}

void GeometryEvaluator::visit(const DecomposeNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
		return p?p->decompose():noResult();
	});
}

void GeometryEvaluator::visit(const ComplementNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
		return p?p->complement():noResult();
	});
}

void GeometryEvaluator::visit(const RadialsNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
#ifdef USE_CGAL
		CGALAuxiliaryBuilder b(reporter);
		return b.buildRadialsPrimitive(p);
#else
		return noResult();
#endif
	});
}

void GeometryEvaluator::visit(const VolumesNode& n)
{
	result=QtConcurrent::run(pool,[&n,this]() {
		Primitive* p=unionChildren(n);
#ifdef USE_CGAL
		CGALAuxiliaryBuilder b(reporter);
		return b.buildVolumesPrimitive(p,n.getCalcMass());
#else
		return noResult();
#endif
	});
}

Primitive* GeometryEvaluator::getResult() const
{
	return result.result();
}
