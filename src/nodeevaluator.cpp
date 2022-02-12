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

#include <QVector>
#include "nodeevaluator.h"
#include "rmath.h"
#include "onceonly.h"
#include "polyhedron.h"
#include "simpletextbuilder.h"
#include "module/cubemodule.h"

#ifdef USE_CGAL
#include <CGAL/centroid.h>

#include "cgalimport.h"
#include "cgalexplorer.h"
#include "cgalprimitive.h"
#include "cgalfragment.h"
#include "cgalbuilder.h"
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
	int count=op.childCount();
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
	CGAL::Vector3 axis(CGAL::ORIGIN,op.getAxis());
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
	auto* pr=dynamic_cast<CGALPrimitive*>(result);
	CGAL::Cuboid3 b=pr->getBounds();

	CGAL::Scalar xmin=b.xmin();
	CGAL::Scalar ymin=b.ymin();
	CGAL::Scalar xmax=b.xmax();
	CGAL::Scalar ymax=b.ymax();
	CGAL::Scalar zmin=b.zmin();
	CGAL::Scalar zmax=b.zmax();

	if(zmin!=0.0) {
		QString pos=to_string(zmin);
		QString where = zmin<0.0?tr("below"):tr("above");
		reporter.reportWarning(tr("the model is %1 %2 the build platform.").arg(pos).arg(where));

		SimpleTextBuilder t;
		t.setText(pos);
		CGAL::Scalar h=t.getHeight()+0.2;
		t.setLocation(CGAL::Point3(xmin,ymin-h,zmin));
		Primitive* c=t.buildPrimitive();
		result->appendChild(c);
	}

	Primitive* a=new Polyhedron();
	a->setType(PrimitiveTypes::Lines);
	CubeModule::createCuboid<CGAL::Point3>(a,xmin,xmax,ymin,ymax,zmin,zmax);

	result->appendChild(a);

	CGAL::Point3 lower(xmin,ymin,zmin);
	CGAL::Point3 upper(xmax,ymax,zmax);
	CGAL::Point3 size(xmax-xmin,ymax-ymin,zmax-zmin);
	reporter.reportMessage(tr("Bounds: [%1,%2]\nSize: %3").arg(to_string(lower),to_string(upper),to_string(size)));
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
	CGALExplorer e(result);
	CGALPrimitive* prim=e.getPrimitive();

	auto* a=new Polyhedron();
	a->setType(PrimitiveTypes::Lines);
	int i=0;
	for(CGALPolygon* pg: prim->getCGALPolygons()) {
		CGAL::Vector3 v=pg->getNormal();
		CGAL::Plane3 p=pg->getPlane();
		CGAL::Vector3 b=p.base1();
		QList<CGAL::Point3> pts=pg->getPoints();
		CGAL::Point3 c=CGAL::centroid(pts.begin(),pts.end());

		CGAL::Scalar l=r_sqrt(v.squared_length());
		CGAL::Point3 norm=c+(v/l);
		l=r_sqrt(b.squared_length());
		b=b/(l*8.0);

		Polygon& np=a->createPolygon();
		a->createVertex(c+b);
		a->createVertex(norm);
		a->createVertex(c-b);
		np.append(i++);
		np.append(i++);
		np.append(i++);
	}
	delete prim;

	result->appendChild(a);
#endif
}

void NodeEvaluator::visit(const SimplifyNode& n)
{
	if(!evaluate(n,Operations::Union)) return;

	result=result->simplify(n.getRatio());
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
		QList<Node*> allChildren=n.getChildren();
		QList<int> indexes=n.getIndexes();
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

	decimal l=50.0;
#if USE_CGAL
	auto* pr=dynamic_cast<CGALPrimitive*>(result);
	CGAL::Cuboid3 b=pr->getBounds();
	CGAL::Point3 size(b.xmax()-b.xmin(),b.ymax()-b.ymin(),b.zmax()-b.zmin());
	l=r_max(size.x(),r_max(size.y(),size.z()))*2.0;
#endif
	using Axis = TransformationNode::Axis;
	Axis axis=tr.getDatumAxis();
	if(axis!=Axis::None) {
		Primitive* a=new Polyhedron();
		a->setType(PrimitiveTypes::Surface);
		switch(axis) {
			case Axis::X: {
				a->createVertex(Point(0,-l,-l));
				a->createVertex(Point(0,-l,+l));
				a->createVertex(Point(0,+l,+l));
				a->createVertex(Point(0,+l,-l));
			}
			break;
			case Axis::Y: {
				a->createVertex(Point(-l,0,-l));
				a->createVertex(Point(-l,0,+l));
				a->createVertex(Point(+l,0,+l));
				a->createVertex(Point(+l,0,-l));
			}
			break;
			case Axis::Z: {
				a->createVertex(Point(-l,-l,0));
				a->createVertex(Point(-l,+l,0));
				a->createVertex(Point(+l,+l,0));
				a->createVertex(Point(+l,-l,0));
			}
			break;
			default:
				break;
		}
		Polygon& p=a->createPolygon();
		p.append(0);
		p.append(1);
		p.append(2);
		p.append(3);
		result->appendChild(a);
	}
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
	auto* pr=dynamic_cast<CGALPrimitive*>(result);
	CGAL::Circle3 circle=pr->getRadius();
	CGAL::Scalar r=r_sqrt(circle.squared_radius());
	QString rs=to_string(r);
	reporter.reportMessage(tr("Radius: %1").arg(rs));

	CGAL::Point3 c=circle.center();
	CGAL::Scalar a=c.x();
	CGAL::Scalar b=c.y();

	SimpleTextBuilder t;
	t.setText(rs);
	t.setLocation(CGAL::Point3(a,b,0));
	Primitive* cp=t.buildPrimitive();
	result->appendChild(cp);

	auto* p = new Polyhedron();
	p->setType(PrimitiveTypes::Lines);
	Polygon& pg=p->createPolygon();

	const int f=90;
	for(auto i=0; i<=f; ++i) {
		CGAL::Scalar phi = (r_tau()*i) / f;
		CGAL::Scalar x = a+r*r_cos(phi);
		CGAL::Scalar y = b+r*r_sin(phi);

		p->createVertex(CGAL::Point3(x,y,0));
		pg.append(i);
	}

	result->appendChild(p);
#endif
}

void NodeEvaluator::visit(const VolumesNode& n)
{
	if(!evaluate(n,Operations::Union)) return;
#ifdef USE_CGAL
	auto* pr=dynamic_cast<CGALPrimitive*>(result);
	bool calcMass = n.getCalcMass();
	const CGALVolume& v=pr->getVolume(calcMass);

	const CGAL::Scalar& vn=v.getSize();
	QString vs=to_string(vn);
	reporter.reportMessage(tr("Volume: %1").arg(vs));

	const CGAL::Point3& c=v.getCenter();

	if(calcMass)
		reporter.reportMessage(tr("Center of Mass: %1").arg(to_string(c)));

	const CGAL::Cuboid3& b=v.getBounds();
	CGAL::Scalar x=b.xmax()+((b.xmax()-b.xmin())/10.0);
	CGAL::Scalar y=b.ymax()+((b.ymax()-b.ymin())/10.0);
	CGAL::Scalar z=b.zmax()+((b.zmax()-b.zmin())/10.0);
	CGAL::Point3 tr(x,y,z);

	auto* p = new Polyhedron();
	p->setType(PrimitiveTypes::Lines);
	Polygon& pg=p->createPolygon();
	p->createVertex(c);
	p->createVertex(tr);
	pg.append(0);
	pg.append(1);
	result->appendChild(p);

	SimpleTextBuilder t;
	t.setText(vs);
	t.setLocation(tr);
	Primitive* tp=t.buildPrimitive();
	result->appendChild(tp);
#endif
}

Primitive* NodeEvaluator::getResult() const
{
	return result;
}
