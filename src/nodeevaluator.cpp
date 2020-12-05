/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2020 Giles Bathgate
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
#include <CGAL/convex_hull_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Alpha_shape_vertex_base_3.h>
#include <CGAL/Alpha_shape_3.h>
#if CGAL_VERSION_NR < CGAL_VERSION_NUMBER(4,11,0)
#include <CGAL/Subdivision_method_3.h>
#else
#include <CGAL/Subdivision_method_3/subdivision_methods_3.h>
#endif
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
	CacheManager& m=CacheManager::getInstance();
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
	Primitive* first=nullptr;
	for(Node* n: op.getChildren()) {
		n->accept(*this);
		if(!first) {
			first=result;
		} else if(result) {
#ifdef USE_CGAL
			if(result->isFullyDimentional()) {
				reporter.reportWarning(tr("Second child of glide module cannot be fully dimentional"));
				return;
			}
			QList<CGAL::Point3> points;
			if(result->getType()==PrimitiveTypes::Lines) {
				points = result->getPoints();
			} else {
				CGALExplorer explorer(result);
				CGALPrimitive* primitive=explorer.getPrimitive();
				points = primitive->getCGALPerimeter().first()->getPoints();
				delete primitive;

				/* TODO glide all polygons?
				for(CGALPolygon* pg: peri->getCGALPolygons()) {
					points = pg->getPoints();
					// break;
				}
				*/
			}
			bool closed=false;
			auto* cp=new CGALPrimitive();
			cp->setType(PrimitiveTypes::Lines);
			CGAL::Point3 fp;
			CGAL::Point3 np;
			OnceOnly first_p;
			cp->createPolygon();
			for(const auto& pt: points) {
				if(first_p()) {
					fp=pt;
				} else if(pt==fp) {
					closed=true;
					break;
				}
				cp->appendVertex(pt);
				np=pt;
			}
			cp->appendChild(result);

			if(!closed) {
				result=first->minkowski(cp);
			} else {
				Primitive* next=first->copy();
				result=first->minkowski(cp);
				cp=new CGALPrimitive();
				cp->setType(PrimitiveTypes::Lines);
				cp->createPolygon();
				cp->appendVertex(np);
				cp->appendVertex(fp);
				result->join(next->minkowski(cp));
			}
			if(result) break;
#endif
		}
	}
}

#ifdef USE_CGAL
static void evaluateHull(Primitive* first,Primitive* previous, Primitive* next)
{
	QList<CGAL::Point3> points;
	if(previous)
		points.append(previous->getPoints());

	if(next)
		points.append(next->getPoints());

	if(points.count()<3)
		return;

	CGAL::Polyhedron3 hull;
	CGAL::convex_hull_3(points.begin(),points.end(),hull);
	first->add(new CGALPrimitive(hull),true);
}
#endif

void NodeEvaluator::visit(const HullNode& n)
{
#ifdef USE_CGAL

	if(n.getChain()) {
		Primitive* first=nullptr;
		Primitive* previous=nullptr;
		for(Node* c: n.getChildren()) {
			c->accept(*this);
			if(!previous) {
				first=result;
			} else {
				evaluateHull(first,previous,result);
				first->appendChild(result);
			}
			previous=result;
		}
		if(first) {
			if(n.getClosed())
				evaluateHull(first,first,previous);

			result=first->combine();
		}
	} else {
		QList<CGAL::Point3> points;
		QList<Primitive*> children;
		for(Node* c: n.getChildren()) {
			c->accept(*this);
			if(result) {
				points.append(result->getPoints());
				children.append(result);
			}
		}

		if(!n.getConcave()) {
			CGAL::Object o;
			CGAL::convex_hull_3(points.begin(),points.end(),o);
			const auto* t=CGAL::object_cast<CGAL::Triangle3>(&o);
			if(t) {
				auto* cp=new CGALPrimitive();
				auto* ct=cp->createPolygon();
				ct->appendVertex(t->vertex(0));
				ct->appendVertex(t->vertex(1));
				ct->appendVertex(t->vertex(2));
				cp->appendChildren(children);
				result=cp;
				return;
			}
			const auto* hull=CGAL::object_cast<CGAL::Polyhedron3>(&o);
			if(hull) {
				auto* cp=new CGALPrimitive(*hull);
				cp->appendChildren(children);
				result=cp;
				return;
			}
			return;
		}

		using Vb = CGAL::Alpha_shape_vertex_base_3<CGAL::Kernel3>;
		using Fb = CGAL::Alpha_shape_cell_base_3<CGAL::Kernel3>;
		using Tds = CGAL::Triangulation_data_structure_3<Vb, Fb>;
		using Triangulation_3 = CGAL::Delaunay_triangulation_3<CGAL::Kernel3, Tds>;
		using Alpha_shape_3 = CGAL::Alpha_shape_3<Triangulation_3>;
		using Facet = Alpha_shape_3::Facet;

		auto* cp = new CGALPrimitive();
		Alpha_shape_3 as(points.begin(), points.end(),0.001,Alpha_shape_3::GENERAL);
		const auto& opt = as.find_optimal_alpha(1);
		if(opt != as.alpha_end()) {

			as.set_alpha(*opt);

			QList<Facet> facets;
			as.get_alpha_shape_facets(std::back_inserter(facets),Alpha_shape_3::REGULAR);

			for(Facet f: facets) {
				auto& t=f.first;
				//To have a consistent orientation of the facet, always consider an exterior cell
				if(as.classify(t) != Alpha_shape_3::EXTERIOR)
					f = as.mirror_facet(f);

				int i=f.second;
				int indices[3] = { (i + 1) % 4, (i + 2) % 4, (i + 3) % 4 };
				//According to the encoding of vertex indices, this is needed to get a consistent orientation
				if(i % 2 == 0)
					std::swap(indices[0], indices[1]);

				//Build triangle faces
				cp->createPolygon();
				CGAL::Point3 p1=t->vertex(indices[0])->point();
				CGAL::Point3 p2=t->vertex(indices[1])->point();
				CGAL::Point3 p3=t->vertex(indices[2])->point();
				cp->appendVertex(p1);
				cp->appendVertex(p2);
				cp->appendVertex(p3);
			}
		}
		cp->appendChildren(children);
		result=cp;
	}
#endif
}

#ifdef USE_CGAL
static CGAL::Point3 flatten(const CGAL::Point3& p)
{
	return CGAL::Point3(p.x(),p.y(),0.0);
}

static CGAL::Point3 translate(const CGAL::Point3& p,const CGAL::Vector3& v)
{
	CGAL::AffTransformation3 t(
		1.0,0.0,0.0,v.x(),
		0.0,1.0,0.0,v.y(),
		0.0,0.0,1.0,v.z(),1.0);
	return p.transform(t);
}

static CGAL::Point3 rotate(const CGAL::Point3& p,const CGAL::Scalar& a,const CGAL::Vector3& axis)
{
	CGAL::Scalar u=axis.x();
	CGAL::Scalar v=axis.y();
	CGAL::Scalar w=axis.z();
	CGAL::Scalar c=r_cos(a);
	CGAL::Scalar s=r_sin(a);

	CGAL::Scalar c1=1.0-c;

	CGAL::AffTransformation3 t(
		u*u*c1+c,u*v*c1-w*s,u*w*c1+v*s,0.0,
		u*v*c1+w*s,v*v*c1+c,v*w*c1-u*s,0.0,
		u*w*c1-v*s,v*w*c1+u*s,w*w*c1+c,0.0,1.0
	);

	return p.transform(t);
}
#endif

void NodeEvaluator::visit(const LinearExtrudeNode& op)
{
	if(!evaluate(op,Operations::Union)) return;
#ifdef USE_CGAL

	CGAL::Scalar height=op.getHeight();
	CGAL::Vector3 axis(CGAL::ORIGIN,op.getAxis());
	CGAL::Vector3 t=axis*height;

	auto* extruded=new CGALPrimitive();
	if(result->isFullyDimentional()) {
		extruded->setType(PrimitiveTypes::Lines);
		extruded->createPolygon();
		extruded->appendVertex(CGAL::ORIGIN);
		extruded->appendVertex(CGAL::Point3(t.x(),t.y(),t.z()));
		result=result->minkowski(extruded);
	} else {

		CGALExplorer explorer(result);

		CGALPrimitive* primitive=explorer.getPrimitive();
		QList<CGALPolygon*> polygons=primitive->getCGALPolygons();

		CGAL::Direction3 d=t.direction();
		for(CGALPolygon* pg: polygons) {
			extruded->createPolygon();
			bool up=(pg->getDirection()==d);
			for(const auto& pt: pg->getPoints())
				extruded->addVertex(pt,up);
		}

		for(CGALPolygon* pg: primitive->getCGALPerimeter()) {
			bool up=(pg->getDirection()==d)!=pg->getHole();
			OnceOnly first;
			CGAL::Point3 pn;
			for(const auto& pt: pg->getPoints()) {
				if(!first()) {
					extruded->createPolygon();
					extruded->addVertex(translate(pn,t),up);
					extruded->addVertex(translate(pt,t),up);
					extruded->addVertex(pt,up);
					extruded->addVertex(pn,up);
				}
				pn=pt;
			}
		}

		for(CGALPolygon* pg: polygons) {
			extruded->createPolygon();
			bool up=(pg->getDirection()==d);
			for(const auto& pt: pg->getPoints())
				extruded->addVertex(translate(pt,t),!up);
		}
		delete primitive;
		extruded->appendChild(result);

		result=extruded;
	}
#endif
}

void NodeEvaluator::visit(const RotateExtrudeNode& op)
{
	if(!evaluate(op,Operations::Union)) return;

	if(result->isFullyDimentional()) {
		reporter.reportWarning(tr("Rotate extrude for volume not implemented"));
		return;
	}
#ifdef USE_CGAL
	CGAL::Vector3 axis(CGAL::ORIGIN,op.getAxis());
	CGAL::Scalar mag=r_sqrt(axis.squared_length(),false);
	if(mag==0.0)
		return;
	axis=axis/mag;

	CGAL::Scalar r=op.getRadius();
	CGAL::Scalar height=op.getHeight();
	CGAL::Scalar sweep=op.getSweep();
	CGAL::Plane3 plane(CGAL::ORIGIN,axis);
	CGAL::Direction3 d=plane.base2().direction();

	CGALExplorer explorer(result);
	CGALPrimitive* primitive=explorer.getPrimitive();
	QList<CGALPolygon*> polygons=primitive->getCGALPolygons();

	CGAL::Cuboid3 b=primitive->getBounds();
	auto* fg=op.getFragments();
	int f=fg->getFragments((b.xmax()-b.xmin())+r);
	CGAL::Vector3 t(r,0.0,0.0);

	bool caps=(sweep!=360.0||height>0.0);

	auto* extruded=new CGALPrimitive();
	if(caps) {
		foreach(CGALPolygon* pg,polygons) {
			extruded->createPolygon();
			bool up=(pg->getDirection()==d);
			foreach(CGAL::Point3 pt,pg->getPoints()) {
				CGAL::Point3 q=translate(pt,t);
				extruded->addVertex(q,up);
			}
		}
	}

	if(sweep==0.0) {
		delete primitive;
		extruded->appendChild(result);
		result=extruded;
		return;
	}

	CGAL::Scalar phi;
	CGAL::Scalar nphi;
	for(auto i=0; i<f; ++i) {
		int j=caps?i+1:(i+1)%f;
		CGAL::Scalar ang=r_tau()*sweep/360.0;
		phi=ang*i/f;
		nphi=ang*j/f;

		for(CGALPolygon* pg: primitive->getCGALPerimeter()) {
			bool hole=pg->getHole();
			if(!caps && hole) continue;
			bool up=(pg->getDirection()==d)!=hole;
			CGAL::Point3 pn;
			OnceOnly first;
			for(const auto& pt: pg->getPoints()) {
				if(!first()) {
					CGAL::Point3 q=translate(pn,t);
					CGAL::Point3 p=translate(pt,t);
					if(q.x()<=0.0&&p.x()<=0.0) {
						pn=pt;
						continue;
					}

					extruded->createPolygon();
					CGAL::Point3 q1=rotate(q,nphi,axis);
					CGAL::Point3 p1=rotate(p,nphi,axis);
					CGAL::Point3 p2=rotate(p,phi,axis);
					CGAL::Point3 q2=rotate(q,phi,axis);
					extruded->addVertex(q1,up);
					extruded->addVertex(p1,up);
					if(p2!=p1)
						extruded->addVertex(p2,up);
					if(q2!=q1)
						extruded->addVertex(q2,up);
				}
				pn=pt;
			}
		}
	}

	if(caps) {
		foreach(CGALPolygon* pg,polygons) {
			extruded->createPolygon();
			bool up=(pg->getDirection()==d);
			foreach(CGAL::Point3 pt,pg->getPoints()) {
				CGAL::Point3 q=translate(pt,t);
				CGAL::Point3 p=rotate(q,nphi,axis);
				extruded->addVertex(p,!up);
			}
		}
	}

	delete primitive;
	extruded->appendChild(result);

	result=extruded;
#endif
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
					first->add(result,false);
					break;
				case Operations::Union:
					first->add(result,true);
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
			}
		}
	}

	if(first)
		result=first->combine();
	else
		result=first;

	return (result!=nullptr);
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
#ifdef USE_CGAL
	auto* cp=dynamic_cast<CGALPrimitive*>(result);
	CGAL::Polyhedron3* p=cp->getPolyhedron();

#if CGAL_VERSION_NR <= CGAL_VERSION_NUMBER(4,11,0)
	CGAL::Subdivision_method_3::Loop_subdivision(*p,n.getLevel());
#else
	CGAL::Subdivision_method_3::Loop_subdivision(*p,CGAL::parameters::number_of_iterations(n.getLevel()));
#endif
	cp=new CGALPrimitive(*p);
	cp->appendChild(result);
	delete p;
	result=cp;
#endif
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
		CGAL::Point3 n=c+(v/l);
		l=r_sqrt(b.squared_length());
		b=b/(l*8.0);

		Polygon* np=a->createPolygon();
		a->createVertex(c+b);
		a->createVertex(n);
		a->createVertex(c-b);
		np->append(i++);
		np->append(i++);
		np->append(i++);
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
	result=nullptr;
#endif
}

void NodeEvaluator::visit(const TransformationNode& tr)
{
	if(!evaluate(tr,Operations::Union)) return;

	result->transform(tr.getMatrix());
}

void NodeEvaluator::visit(const ResizeNode& n)
{
	if(!evaluate(n,Operations::Union)) return;
#ifdef USE_CGAL
	auto* pr=dynamic_cast<CGALPrimitive*>(result);
	CGAL::Cuboid3 b=pr->getBounds();
	CGAL::Point3 s=n.getSize();
	CGAL::Scalar x=s.x();
	CGAL::Scalar y=s.y();
	CGAL::Scalar z=s.z();
	CGAL::Scalar autosize=1.0;

	if(z!=0.0) {
		z/=(b.zmax()-b.zmin());
		autosize=z;
	}
	if(y!=0.0) {
		y/=(b.ymax()-b.ymin());
		autosize=y;
	}
	if(x!=0.0) {
		x/=(b.xmax()-b.xmin());
		autosize=x;
	}
	if(!n.getAutoSize())
		autosize=1.0;

	if(x==0.0) x=autosize;
	if(y==0.0) y=autosize;
	if(z==0.0) z=autosize;

	TransformMatrix t(
		x  ,0.0,0.0,0.0,
		0.0,y  ,0.0,0.0,
		0.0,0.0,z  ,0.0,
		0.0,0.0,0.0,1.0);

	result->transform(&t);
#endif
}

void NodeEvaluator::visit(const AlignNode& n)
{
	if(!evaluate(n,Operations::Union)) return;
#ifdef USE_CGAL
	auto* pr=dynamic_cast<CGALPrimitive*>(result);
	CGAL::Cuboid3 b=pr->getBounds();
	CGAL::Scalar cx=0.0;
	CGAL::Scalar cy=0.0;
	CGAL::Scalar cz=0.0;

	if(n.getCenter()) {
		cx=(b.xmin()+b.xmax())/2.0;
		cy=(b.ymin()+b.ymax())/2.0;
		cz=(b.zmin()+b.zmax())/2.0;
	} else {
		bool top=false;
		bool bottom=false;
		bool north=false;
		bool south=false;
		bool west=false;
		bool east=false;
		for(AlignNode::Face_t a: n.getAlign()) {
			switch(a) {
				case AlignNode::Top:
					top=true;
					cz=b.zmax();
					break;
				case AlignNode::Bottom:
					bottom=true;
					cz=b.zmin();
					break;
				case AlignNode::North:
					north=true;
					cx=b.xmax();
					break;
				case AlignNode::South:
					south=true;
					cx=b.xmin();
					break;
				case AlignNode::West:
					west=true;
					cy=b.ymax();
					break;
				case AlignNode::East:
					east=true;
					cy=b.ymin();
					break;
			}
		}
		if(top&&bottom)
			cz=(b.zmin()+b.zmax())/2.0;
		if(north&&south)
			cx=(b.xmin()+b.xmax())/2.0;
		if(east&&west)
			cy=(b.ymin()+b.ymax())/2.0;
	}

	TransformMatrix t(
		1.0,0.0,0.0,-cx,
		0.0,1.0,0.0,-cy,
		0.0,0.0,1.0,-cz,
		0.0,0.0,0.0,1.0);

	result->transform(&t);
#endif
}

void NodeEvaluator::visit(const PointsNode& n)
{
	Primitive* cp=createPrimitive();
	cp->setType(PrimitiveTypes::Points);
	QList<Point> points=n.getPoints();
	cp->createPolygon();
	if(points.isEmpty()) {
		cp->createVertex(Point(0.0,0.0,0.0));
	} else {
		for(const auto& p: points)
			cp->createVertex(p);
	}

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
#ifdef USE_CGAL
	auto* pr=dynamic_cast<CGALPrimitive*>(result);
	CGAL::Cuboid3 b=pr->getBounds();

	const CGAL::Scalar& xmin=b.xmin();
	const CGAL::Scalar& ymin=b.ymin();
	const CGAL::Scalar& xmax=b.xmax();
	const CGAL::Scalar& ymax=b.ymax();

	const CGAL::Scalar& h=n.getHeight();
	const CGAL::Scalar& t=n.getThickness();

	Primitive* cp=new CGALPrimitive();
	CubeModule::createCuboid<CGAL::Point3>(cp,xmin,xmax,ymin,ymax,h,h+t);

	result=result->intersection(cp);
#endif
}

void NodeEvaluator::visit(const ProductNode& p)
{
	Primitive* prim = p.getPrimitive();
	result=prim->copy();
}

void NodeEvaluator::visit(const ProjectionNode& op)
{
	if(!evaluate(op,Operations::Union)) return;

#ifdef USE_CGAL
	CGALExplorer explorer(result);
	CGALPrimitive* cp=explorer.getPrimitive();
	auto* projected=new CGALPrimitive();
	bool base=op.getBase();
	if(base) {
		for(CGALPolygon* pg: explorer.getBase()) {
			projected->createPolygon();
			for(const auto& pt: pg->getPoints())
				projected->appendVertex(pt);
		}
	} else {
		for(CGALPolygon* p: cp->getCGALPolygons()) {
			CGAL::Vector3 normal=p->getNormal();
			if(normal.z()==0.0)
				continue;

			auto* flat=new CGALPrimitive();
			flat->createPolygon();
			for(const auto& pt: p->getPoints()) {
				flat->appendVertex(flatten(pt));
			}
			projected->join(flat);
		}
	}
	projected->appendChild(result);
	delete cp;
	result=projected;
#endif
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
	Polygon* pg=p->createPolygon();

	const int f=90;
	for(auto i=0; i<=f; ++i) {
		CGAL::Scalar phi = (r_tau()*i) / f;
		CGAL::Scalar x = a+r*r_cos(phi);
		CGAL::Scalar y = b+r*r_sin(phi);

		p->createVertex(CGAL::Point3(x,y,0));
		pg->append(i);
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
	Polygon* pg=p->createPolygon();
	p->createVertex(c);
	p->createVertex(tr);
	pg->append(0);
	pg->append(1);
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
