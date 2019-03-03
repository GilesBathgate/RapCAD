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
	CacheManager* m=CacheManager::getInstance();
	cache=m->getCache();
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
	if(!evaluate(n,Union,cp)) return;
}

void NodeEvaluator::visit(const TriangulateNode& n)
{
	if(!evaluate(n,Union)) return;

	result=result->triangulate();
	if(result)
		result->setType(Primitive::Surface);
}

void NodeEvaluator::visit(const MaterialNode& n)
{
	if(!evaluate(n,Union)) return;
	auto* p=new Polyhedron();
	p->appendChild(result);
	result=p;
}

void NodeEvaluator::visit(const DiscreteNode& n)
{
	if(!evaluate(n,Union)) return;

	result->discrete(n.getPlaces());
}

void NodeEvaluator::visit(const UnionNode& op)
{
	if(!evaluate(op,Union)) return;
}

void NodeEvaluator::visit(const GroupNode& op)
{
	if(!evaluate(op,Group)) return;
}

void NodeEvaluator::visit(const DifferenceNode& op)
{
	if(!evaluate(op,Difference)) return;
}

void NodeEvaluator::visit(const IntersectionNode& op)
{
	if(!evaluate(op,Intersection)) return;
}

void NodeEvaluator::visit(const SymmetricDifferenceNode& op)
{
	if(!evaluate(op,SymmetricDifference)) return;
}

void NodeEvaluator::visit(const MinkowskiNode& op)
{
	if(!evaluate(op,Minkowski)) return;
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
			if(result->getType()==Primitive::Lines) {
				points = result->getPoints();
			} else {
				CGALExplorer explorer(result);
				CGALPrimitive* peri=explorer.getPerimeters();
				if(!peri) return;

				for(CGALPolygon* pg: peri->getCGALPolygons()) {
					points = pg->getPoints();
					break;
				}
			}
			bool closed=false;
			auto* cp=new CGALPrimitive();
			cp->setType(Primitive::Lines);
			CGAL::Point3 fp,np;
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
			if(!closed) {
				result=first->minkowski(cp);
			} else {
				Primitive* next=first->copy();
				result=first->minkowski(cp);
				cp=new CGALPrimitive();
				cp->setType(Primitive::Lines);
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
	if(previous) {
		CGALExplorer p(previous);
		points.append(p.getPoints());
	}

	if(next) {
		CGALExplorer n(next);
		points.append(n.getPoints());
	}

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
		for(Node* c: n.getChildren()) {
			c->accept(*this);
			if(result) {
				CGALExplorer explorer(result);
				points.append(explorer.getPoints());
			}
		}
		if(points.count()<3) return;

		if(!n.getConcave()) {
			CGAL::Polyhedron3 hull;
			CGAL::convex_hull_3(points.begin(),points.end(),hull);
			result=new CGALPrimitive(hull);
			return;
		}

		typedef CGAL::Alpha_shape_vertex_base_3<CGAL::Kernel3>     Vb;
		typedef CGAL::Alpha_shape_cell_base_3<CGAL::Kernel3>       Fb;
		typedef CGAL::Triangulation_data_structure_3<Vb,Fb>        Tds;
		typedef CGAL::Delaunay_triangulation_3<CGAL::Kernel3, Tds> Triangulation_3;
		typedef CGAL::Alpha_shape_3<Triangulation_3>               Alpha_shape_3;
		typedef Alpha_shape_3::Alpha_iterator                      Alpha_iterator;
		typedef Alpha_shape_3::Facet                               Facet;

		Alpha_shape_3 as(points.begin(), points.end(),0.001,Alpha_shape_3::GENERAL);
		Alpha_iterator opt = as.find_optimal_alpha(1);
		if(opt == as.alpha_end()) {
			result=nullptr;
			return;
		}
		as.set_alpha(*opt);

		QList<Facet> facets;
		as.get_alpha_shape_facets(std::back_inserter(facets),Alpha_shape_3::REGULAR);

		auto* cp = new CGALPrimitive();
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
		result=cp;
	}
#endif
}

#ifdef USE_CGAL
static CGAL::Point3 flatten(const CGAL::Point3& p)
{
	return CGAL::Point3(p.x(),p.y(),0);
}

static CGAL::Point3 translate(const CGAL::Point3& p,const CGAL::Vector3& v)
{
	CGAL::AffTransformation3 t(
		1, 0, 0, v.x(),
		0, 1, 0, v.y(),
		0, 0, 1, v.z(), 1);
	return p.transform(t);
}

static CGAL::Point3 rotate(const CGAL::Point3& p,const CGAL::Scalar& a,const CGAL::Vector3& axis)
{
	CGAL::Scalar u=axis.x(),v=axis.y(),w=axis.z();
	CGAL::Scalar c=r_cos(a);
	CGAL::Scalar s=r_sin(a);

	CGAL::Scalar c1=1-c;

	CGAL::AffTransformation3 t(
		u*u*c1+c,u*v*c1-w*s,u*w*c1+v*s,0,
		u*v*c1+w*s,v*v*c1+c,v*w*c1-u*s,0,
		u*w*c1-v*s,v*w*c1+u*s,w*w*c1+c,0, 1
	);

	return p.transform(t);
}
#endif

void NodeEvaluator::visit(const LinearExtrudeNode& op)
{
	if(!evaluate(op,Union)) return;
#ifdef USE_CGAL
	auto* cp=new CGALPrimitive();
	CGAL::Scalar z=op.getHeight();
	CGAL::Point3 a=op.getAxis();
	CGAL::Vector3 axis(CGAL::ORIGIN,a);
	CGAL::Vector3 t=axis*z;

	if(result->isFullyDimentional()) {
		cp->setType(Primitive::Lines);
		cp->createPolygon();
		cp->appendVertex(CGAL::ORIGIN);
		cp->appendVertex(CGAL::Point3(t.x(),t.y(),t.z()));
		result=result->minkowski(cp);
	} else {

		CGAL::Direction3 d=t.direction();

		CGALExplorer explorer(result);
		CGALPrimitive* peri=explorer.getPerimeters();
		if(!peri) return;

		CGALPrimitive* prim=explorer.getPrimitive();
		QList<CGALPolygon*> polys=prim->getCGALPolygons();

		for(CGALPolygon* pg: polys) {
			cp->createPolygon();
			bool up=(pg->getDirection()==d);
			for(const auto& pt: pg->getPoints())
				cp->addVertex(pt,up);
		}

		for(CGALPolygon* pg: peri->getCGALPolygons()) {
			bool up=(pg->getDirection()==d)!=pg->getHole();
			OnceOnly first;
			CGAL::Point3 pn;
			for(const auto& pt: pg->getPoints()) {
				if(!first()) {
					cp->createPolygon();
					cp->addVertex(translate(pn,t),up);
					cp->addVertex(translate(pt,t),up);
					cp->addVertex(pt,up);
					cp->addVertex(pn,up);
				}
				pn=pt;
			}
		}

		for(CGALPolygon* pg: polys) {
			cp->createPolygon();
			bool up=(pg->getDirection()==d);
			for(const auto& pt: pg->getPoints())
				cp->addVertex(translate(pt,t),!up);
		}

		result=cp;
	}
#endif
}

void NodeEvaluator::visit(const RotateExtrudeNode& op)
{
	if(!evaluate(op,Union)) return;

	if(result->isFullyDimentional()) {
		reporter.reportWarning(tr("Rotate extrude for volume not implemented"));
		return;
	}
#ifdef USE_CGAL
	CGAL::Scalar r=op.getRadius();
	CGAL::Scalar height=op.getHeight();
	CGAL::Scalar sweep=op.getSweep();
	CGAL::Point3 a=op.getAxis();

	CGAL::Vector3 axis(CGAL::ORIGIN,a);
	CGAL::Plane3 pn(CGAL::ORIGIN,axis);
	CGAL::Direction3 d=pn.base2().direction();
	CGAL::Scalar mag=r_sqrt(axis.squared_length(),false);
	if(mag==0)
		return;

	axis=axis/mag;

	CGALExplorer explorer(result);
	CGALPrimitive* prim=explorer.getPrimitive();
	QList<CGALPolygon*> polys=prim->getCGALPolygons();
	auto* n = new CGALPrimitive();
	auto* fg = static_cast<CGALFragment*>(op.getFragments());
	CGAL::Cuboid3 b=explorer.getBounds();
	int f=fg->getFragments((b.xmax()-b.xmin())+r);
	CGAL::Vector3 t(r,0.0,0.0);

	bool caps=(sweep!=360.0||height>0.0);

	if(caps) {
		foreach(CGALPolygon* pg,polys) {
			n->createPolygon();
			bool up=(pg->getDirection()==d);
			foreach(CGAL::Point3 pt,pg->getPoints()) {
				CGAL::Point3 q=translate(pt,t);
				n->addVertex(q,up);
			}
		}
	}

	if(sweep==0.0) {
		result = n;
		return;
	}

	CGAL::Scalar phi,nphi;
	CGALPrimitive* peri=explorer.getPerimeters();
	if(!peri) return;

	for(auto i=0; i<f; ++i) {
		int j=caps?i+1:(i+1)%f;
		CGAL::Scalar ang = r_tau()*sweep/360.0;
		phi=ang*i/f;
		nphi=ang*j/f;

		for(CGALPolygon* pg: peri->getCGALPolygons()) {
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

					n->createPolygon();
					CGAL::Point3 q1=rotate(q,nphi,axis);
					CGAL::Point3 p1=rotate(p,nphi,axis);
					CGAL::Point3 p2=rotate(p,phi,axis);
					CGAL::Point3 q2=rotate(q,phi,axis);
					n->addVertex(q1,up);
					n->addVertex(p1,up);
					if(p2!=p1)
						n->addVertex(p2,up);
					if(q2!=q1)
						n->addVertex(q2,up);
				}
				pn=pt;
			}
		}
	}


	if(caps) {
		foreach(CGALPolygon* pg,polys) {
			n->createPolygon();
			bool up=(pg->getDirection()==d);
			foreach(CGAL::Point3 pt,pg->getPoints()) {
				CGAL::Point3 q=translate(pt,t);
				CGAL::Point3 p=rotate(q,nphi,axis);
				n->addVertex(p,!up);
			}
		}
	}

	result=n;
#endif
}

bool NodeEvaluator::evaluate(const Node& op,Operation_e type)
{
	return evaluate(op,type,nullptr);
}

bool NodeEvaluator::evaluate(const Node& op,Operation_e type,Primitive* first)
{
	return evaluate(op.getChildren(),type,first);
}

bool NodeEvaluator::evaluate(const QList<Node*>& children, Operation_e type, Primitive* first)
{
	for(Node* n: children) {
		n->accept(*this);
		if(!first) {
			first=result;
		} else if(result) {
			switch(type) {
				case Group:
					first->add(result,false);
					break;
				case Union:
					first->add(result,true);
					break;
				case Difference:
					first=first->difference(result);
					break;
				case Intersection:
					first=first->intersection(result);
					break;
				case SymmetricDifference:
					first=first->symmetric_difference(result);
					break;
				case Minkowski:
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
	if(!evaluate(n,Union)) return;
#ifdef USE_CGAL
	auto* pr=static_cast<CGALPrimitive*>(result);
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
	a->setType(Primitive::Lines);
	CubeModule::createCuboid<CGAL::Point3>(a,xmin,xmax,ymin,ymax,zmin,zmax);

	result->appendChild(a);

	CGAL::Point3 lower(xmin,ymin,zmin);
	CGAL::Point3 upper(xmax,ymax,zmax);
	reporter.reportMessage(tr("Bounds: [%1],[%2]").arg(to_string(lower)).arg(to_string(upper)));
#endif
}

void NodeEvaluator::visit(const SubDivisionNode& n)
{
	if(!evaluate(n,Union)) return;
#ifdef USE_CGAL
	auto* cp=static_cast<CGALPrimitive*>(result);
	CGAL::Polyhedron3& p=*cp->getPolyhedron();
	CGAL::Subdivision_method_3::Loop_subdivision(p,n.getLevel());
	result=new CGALPrimitive(p);
#endif
}

void NodeEvaluator::visit(const NormalsNode& n)
{
	if(!evaluate(n,Union)) return;
#ifdef USE_CGAL
	CGALExplorer e(result);
	CGALPrimitive* prim=e.getPrimitive();

	auto* a=new Polyhedron();
	a->setType(Primitive::Lines);
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

	result->appendChild(a);
#endif
}

void NodeEvaluator::visit(const SimplifyNode& n)
{
	if(!evaluate(n,Union)) return;

	result=result->simplify(n.getRatio());
}

void NodeEvaluator::visit(const ChildrenNode& n)
{
	if(n.getIndexes().count()<=0) {
		if(!evaluate(n,Union)) return;
	} else {
		QList<Node*> allChildren=n.getChildren();
		QList<Node*> children;
		for(auto i: n.getIndexes()) {
			if(i>=0&&i<allChildren.count())
				children.append(allChildren.at(i));
		}
		if(!evaluate(children,Union,nullptr)) return;
	}
}

void NodeEvaluator::visit(const OffsetNode& n)
{
	if(!evaluate(n,Union)) return;

	result=result->inset(n.getAmount());
}

void NodeEvaluator::visit(const BoundaryNode& op)
{
	if(!evaluate(op,Union)) return;

#ifdef USE_CGAL
	if(result->isFullyDimentional()) {
		result=result->boundary();
	} else {
		CGALExplorer explorer(result);
		result=explorer.getPerimeters();
	}
#endif
}

void NodeEvaluator::visit(const ImportNode& op)
{
#ifdef USE_CGAL
	CGALImport i(reporter);
	result=i.import(op.getImport());
#else
	result=nullptr;
#endif
}

void NodeEvaluator::visit(const TransformationNode& tr)
{
	if(!evaluate(tr,Union)) return;

	result->transform(tr.getMatrix());
}

void NodeEvaluator::visit(const ResizeNode& n)
{
	if(!evaluate(n,Union)) return;
#ifdef USE_CGAL
	auto* pr=static_cast<CGALPrimitive*>(result);
	CGAL::Cuboid3 b=pr->getBounds();
	CGAL::Point3 s=n.getSize();
	CGAL::Scalar x=s.x(),y=s.y(),z=s.z();
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
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1);

	result->transform(&t);
#endif
}

void NodeEvaluator::visit(const AlignNode& n)
{
	if(!evaluate(n,Union)) return;
#ifdef USE_CGAL
	auto* pr=static_cast<CGALPrimitive*>(result);
	CGAL::Cuboid3 b=pr->getBounds();
	CGAL::Scalar cx=0.0,cy=0.0,cz=0.0;
	CGAL::Scalar two(2.0);
	if(n.getCenter()) {
		cx=(b.xmin()+b.xmax())/two;
		cy=(b.ymin()+b.ymax())/two;
		cz=(b.zmin()+b.zmax())/two;
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
			cz=(b.zmin()+b.zmax())/two;
		if(north&&south)
			cx=(b.xmin()+b.xmax())/two;
		if(east&&west)
			cy=(b.ymin()+b.ymax())/two;
	}

	TransformMatrix t(
		1, 0, 0, -cx,
		0, 1, 0, -cy,
		0, 0, 1, -cz,
		0, 0, 0,  1);

	result->transform(&t);
#endif
}

void NodeEvaluator::visit(const PointsNode& n)
{
	Primitive* cp=createPrimitive();
	cp->setType(Primitive::Points);
	QList<Point> points=n.getPoints();
	cp->createPolygon();
	if(points.count()==0)
		cp->createVertex(Point(0,0,0));
	for(Point p: points)
		cp->createVertex(p);
	result=cp;
}

void NodeEvaluator::visit(const SliceNode& n)
{
	if(!evaluate(n,Union)) return;
#ifdef USE_CGAL
	auto* pr=static_cast<CGALPrimitive*>(result);
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
	if(!evaluate(op,Union)) return;

#ifdef USE_CGAL
	CGALExplorer explorer(result);

	bool base=op.getBase();
	if(base) {
		auto* r=new CGALPrimitive();
		for(CGALPolygon* pg: explorer.getBase()) {
			r->createPolygon();
			for(const auto& pt: pg->getPoints())
				r->appendVertex(pt);
		}
		result=r;
	} else {
		CGALPrimitive* cp=explorer.getPrimitive();

		Primitive* r=new CGALPrimitive();
		for(CGALPolygon* p: cp->getCGALPolygons()) {
			CGAL::Vector3 normal=p->getNormal();
			if(normal.z()==0)
				continue;

			auto* t=new CGALPrimitive();
			t->createPolygon();
			for(const auto& pt: p->getPoints()) {
				t->appendVertex(flatten(pt));
			}
			r=r->join(t);
		}
		result=r;
	}
#endif
}

void NodeEvaluator::visit(const DecomposeNode& n)
{
	if(!evaluate(n,Union)) return;

	result=result->decompose();
}

void NodeEvaluator::visit(const ComplementNode& n)
{
	if(!evaluate(n,Union)) return;

	result=result->complement();
}

void NodeEvaluator::visit(const RadialsNode& n)
{
	if(!evaluate(n,Union)) return;
#ifdef USE_CGAL
	auto* pr=static_cast<CGALPrimitive*>(result);
	CGAL::Circle3 circle=pr->getRadius();
	CGAL::Scalar r=r_sqrt(circle.squared_radius());
	QString rs=to_string(r);
	reporter.reportMessage(tr("Radius: %1").arg(rs));

	CGAL::Point3 c=circle.center();
	CGAL::Scalar a,b;
	a=c.x();
	b=c.y();

	SimpleTextBuilder t;
	t.setText(rs);
	t.setLocation(CGAL::Point3(a,b,0));
	Primitive* cp=t.buildPrimitive();
	result->appendChild(cp);

	auto* p = new Polyhedron();
	p->setType(Primitive::Lines);
	Polygon* pg=p->createPolygon();

	const int f=90;
	for(auto i=0; i<=f; ++i) {
		CGAL::Scalar phi = (r_tau()*i) / f;
		CGAL::Scalar x,y;
		x = a+r*r_cos(phi);
		y = b+r*r_sin(phi);

		p->createVertex(CGAL::Point3(x,y,0));
		pg->append(i);
	}

	result->appendChild(p);
#endif
}

void NodeEvaluator::visit(const VolumesNode& n)
{
	if(!evaluate(n,Union)) return;
#ifdef USE_CGAL
	auto* pr=static_cast<CGALPrimitive*>(result);
	bool calcMass = n.getCalcMass();
	CGALVolume v=pr->getVolume(calcMass);

	CGAL::Scalar vn=v.getSize();
	QString vs=to_string(vn);
	reporter.reportMessage(tr("Volume: %1").arg(vs));

	CGAL::Point3 c=v.getCenter();

	if(calcMass)
		reporter.reportMessage(tr("Center of Mass: %1").arg(to_string(c)));

	CGAL::Cuboid3 b=v.getBounds();
	CGAL::Scalar x,y,z;
	x=b.xmax()+((b.xmax()-b.xmin())/10);
	y=b.ymax()+((b.ymax()-b.ymin())/10);
	z=b.zmax()+((b.zmax()-b.zmin())/10);
	CGAL::Point3 tr(x,y,z);

	auto* p = new Polyhedron();
	p->setType(Primitive::Lines);
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
