/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2017 Giles Bathgate
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

#ifdef USE_CGAL
#include <CGAL/centroid.h>
#include <CGAL/convex_hull_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Alpha_shape_3.h>
#ifdef USE_SUBDIV
#include <CGAL/Subdivision_method_3.h>
#endif
#include "cgalimport.h"
#include "cgalexplorer.h"
#include "cgalprimitive.h"
#include "cgalfragment.h"
#include "cgalbuilder.h"
#endif

NodeEvaluator::NodeEvaluator(Reporter* r)
{
	reporter=r;
	CacheManager* m=CacheManager::getInstance();
	cache=m->getCache();
	result=nullptr;
}

Primitive* NodeEvaluator::createPrimitive()
{
#ifdef USE_CGAL
	return new CGALPrimitive();
#else
	return new Polyhedron();
#endif
}

void NodeEvaluator::visit(PrimitiveNode* n)
{
	Primitive* cp=n->getPrimitive();
	cp=cache->fetch(cp);
	evaluate(n,Union,cp);
}

void NodeEvaluator::visit(PolylineNode* n)
{
	Primitive* cp=n->getPrimitive();
	cp->setType(Primitive::Skeleton);
	evaluate(n,Union,cp);
}

void NodeEvaluator::visit(TriangulateNode* n)
{
	evaluate(n,Union);
	if(result) {
		result=result->triangulate();
		result->setType(Primitive::Volume);
	}
}

void NodeEvaluator::visit(MaterialNode* n)
{
	evaluate(n,Union);
	auto* p=new Polyhedron();
	p->appendChild(result);
	result=p;
}

void NodeEvaluator::visit(DiscreteNode* n)
{
	evaluate(n,Union);
	if(result)
		result->discrete(n->getPlaces());
}

void NodeEvaluator::visit(UnionNode* op)
{
	evaluate(op,Union);
}

void NodeEvaluator::visit(GroupNode* op)
{
	evaluate(op,Group);
}

void NodeEvaluator::visit(DifferenceNode* op)
{
	evaluate(op,Difference);
}

void NodeEvaluator::visit(IntersectionNode* op)
{
	evaluate(op,Intersection);
}

void NodeEvaluator::visit(SymmetricDifferenceNode* op)
{
	evaluate(op,SymmetricDifference);
}

void NodeEvaluator::visit(MinkowskiNode* op)
{
	evaluate(op,Minkowski);
}

void NodeEvaluator::visit(GlideNode* op)
{
	Primitive* first=nullptr;
	for(Node* n: op->getChildren()) {
		n->accept(*this);
		if(!first) {
#ifdef USE_CGAL
			CGALExplorer explorer(result);
			QList<CGAL::Point3> points = explorer.getPoints();

			auto* cp=new CGALPrimitive();
			CGAL::Point3 fp;
			OnceOnly first_p;
			for(const auto& pt: points) {
				if(first_p())
					fp=pt;
				cp->appendVertex(pt);
			}
			if(op->getClosed())
				cp->appendVertex(fp);
			first=cp;
#endif
		} else {
			first=first->minkowski(result);
		}
	}

	result=first;
}

static Primitive* evaluateHull(Primitive* previous, Primitive* next)
{
#ifdef USE_CGAL
	QList<CGAL::Point3> points;
	CGALExplorer p(previous);
	points.append(p.getPoints());

	CGALExplorer n(next);
	points.append(n.getPoints());

	CGAL::Polyhedron3 hull;
	CGAL::convex_hull_3(points.begin(),points.end(),hull);
	return new CGALPrimitive(hull);
#endif
}

void NodeEvaluator::visit(HullNode* n)
{
#ifdef USE_CGAL

	if(n->getChain()) {
		Primitive* first=nullptr;
		Primitive* previous=nullptr;
		for(Node* c: n->getChildren()) {
			c->accept(*this);
			if(!previous) {
				first=result;
			} else {
				Primitive* prim=evaluateHull(previous,result);
				first->add(prim,true);
			}
			previous=result;
		}
		if(first) {
			if(previous && n->getClosed()) {
				Primitive* prim=evaluateHull(first,previous);
				first->add(prim,true);
			}
			result=first->combine();
		}
	} else {
		QList<CGAL::Point3> points;
		for(Node* c: n->getChildren()) {
			c->accept(*this);
			CGALExplorer explorer(result);
			points.append(explorer.getPoints());
		}

		if(!n->getConcave()) {
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

static CGAL::Point3 translate(const CGAL::Point3& p,const CGAL::Scalar& x,const CGAL::Scalar& y,const CGAL::Scalar& z)
{
	CGAL::AffTransformation3 t(
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z, 1);
	return p.transform(t);
}

static CGAL::Point3 rotate(const CGAL::Point3& p,const CGAL::Scalar& a,const CGAL::Scalar& u,const CGAL::Scalar& v,const CGAL::Scalar& w)
{
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

void NodeEvaluator::visit(LinearExtrudeNode* op)
{
	evaluate(op,Union);
#ifdef USE_CGAL
	auto* cp=new CGALPrimitive();
	if(result->isFullyDimentional()) {
		cp->setType(Primitive::Skeleton);
		cp->createPolygon();
		cp->appendVertex(CGAL::ORIGIN);
		cp->appendVertex(CGAL::Point3(0.0,0.0,op->getHeight()));
		result=result->minkowski(cp);
	} else {
		CGAL::Scalar z=op->getHeight();
		CGALExplorer explorer(result);
		CGALPrimitive* peri=explorer.getPerimeters();
		if(!peri) {
			result=cp;
			return;
		}
		CGALPrimitive* prim=explorer.getPrimitive();
		QList<CGALPolygon*> polys=prim->getCGALPolygons();

		for(CGALPolygon* pg: polys) {
			cp->createPolygon();
			bool up=(pg->getNormal().z()>0);
			for(const auto& pt: pg->getPoints())
				cp->addVertex(pt,up);
		}

		for(CGALPolygon* pg: peri->getCGALPolygons()) {
			bool up=(pg->getNormal().z()>0)!=pg->getHole();
			OnceOnly first;
			CGAL::Point3 pn;
			for(const auto& pt: pg->getPoints()) {
				if(!first()) {
					cp->createPolygon();
					cp->addVertex(translate(pn,0,0,z),up);
					cp->addVertex(translate(pt,0,0,z),up);
					cp->addVertex(pt,up);
					cp->addVertex(pn,up);
				}
				pn=pt;
			}
		}

		for(CGALPolygon* pg: polys) {
			cp->createPolygon();
			bool up=(pg->getNormal().z()>0);
			for(const auto& pt: pg->getPoints())
				cp->addVertex(translate(pt,0,0,z),!up);
		}

		result=cp;
	}
#endif
}

void NodeEvaluator::visit(RotateExtrudeNode* op)
{
	evaluate(op,Union);

#ifdef USE_CGAL
	CGAL::Scalar r=op->getRadius();
	CGAL::Scalar height=op->getHeight();
	CGAL::Scalar sweep=op->getSweep();

	CGAL::Point3 pa=op->getAxis();
	CGAL::Scalar x=pa.x(),y=pa.y(),z=pa.z();
	CGAL::Scalar mag = r_sqrt(x*x + y*y + z*z,false);
	CGAL::Scalar ax = x/mag;
	CGAL::Scalar ay = y/mag;
	CGAL::Scalar az = z/mag;

	CGALExplorer explorer(result);
	CGALPrimitive* prim=explorer.getPrimitive();
	QList<CGALPolygon*> polys=prim->getCGALPolygons();
	auto* n = new CGALPrimitive();
	auto* fg = static_cast<CGALFragment*>(op->getFragments());
	CGAL::Cuboid3 b=explorer.getBounds();
	int f=fg->getFragments((b.xmax()-b.xmin())+r);

	bool caps=(sweep!=360.0||height>0.0);

	if(caps) {
		foreach(CGALPolygon* pg,polys) {
			n->createPolygon();
			bool up=(pg->getNormal().z()>0);
			foreach(CGAL::Point3 pt,pg->getPoints()) {
				CGAL::Point3 q=translate(pt,r,0,0);
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
	for(auto i=0; i<f; ++i) {
		int j=caps?i+1:(i+1)%f;
		CGAL::Scalar ang = r_tau()*sweep/360.0;
		phi=ang*i/f;
		nphi=ang*j/f;

		for(CGALPolygon* pg: peri->getCGALPolygons()) {
			bool hole=pg->getHole();
			if(!caps && hole) continue;
			bool up=(pg->getNormal().z()>0)!=hole;
			CGAL::Point3 pn;
			OnceOnly first;
			for(const auto& pt: pg->getPoints()) {
				if(!first()) {
					CGAL::Point3 q=translate(pn,r,0,0);
					CGAL::Point3 p=translate(pt,r,0,0);
					if(q.x()<=0.0&&p.x()<=0.0) {
						pn=pt;
						continue;
					}

					n->createPolygon();
					CGAL::Point3 q1=rotate(q,nphi,ax,ay,az);
					CGAL::Point3 p1=rotate(p,nphi,ax,ay,az);
					CGAL::Point3 p2=rotate(p,phi,ax,ay,az);
					CGAL::Point3 q2=rotate(q,phi,ax,ay,az);
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
			bool up=(pg->getNormal().z()>0);
			foreach(CGAL::Point3 pt,pg->getPoints()) {
				CGAL::Point3 q=translate(pt,r,0,0);
				CGAL::Point3 p=rotate(q,nphi,ax,ay,az);
				n->addVertex(p,!up);
			}
		}
	}

	result=n;
#endif
}

void NodeEvaluator::evaluate(Node* op,Operation_e type)
{
	evaluate(op,type,nullptr);
}

void NodeEvaluator::evaluate(Node* op,Operation_e type,Primitive* first)
{
	evaluate(op->getChildren(),type,first);
}

void NodeEvaluator::evaluate(QList<Node*> children,Operation_e type,Primitive* first)
{
	for(Node* n: children) {
		n->accept(*this);
		if(!first) {
			first=result;
		} else {
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

}

void NodeEvaluator::visit(BoundsNode* n)
{
	evaluate(n,Union);
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
		reporter->reportWarning(tr("the model is %1 %2 the build platform.").arg(pos).arg(where));

		SimpleTextBuilder t;
		t.setText(pos);
		CGAL::Scalar h=t.getHeight()+0.2;
		t.setLocation(CGAL::Point3(xmin,ymin-h,zmin));
		Primitive* c=t.buildPrimitive();
		result->appendChild(c);
	}

	CGAL::Point3 lower(xmin,ymin,zmin);
	CGAL::Point3 upper(xmax,ymax,zmax);

	auto* a=new Polyhedron();
	a->setType(Primitive::Skeleton);
	a->createPolygon();
	a->createVertex(lower); //0
	a->createVertex(CGAL::Point3(xmax,ymin,zmin)); //1
	a->createVertex(CGAL::Point3(xmax,ymax,zmin)); //2
	a->createVertex(CGAL::Point3(xmin,ymax,zmin)); //3
	a->createVertex(CGAL::Point3(xmin,ymin,zmax)); //4
	a->createVertex(CGAL::Point3(xmax,ymin,zmax)); //5
	a->createVertex(upper); //6
	a->createVertex(CGAL::Point3(xmin,ymax,zmax)); //7

	//Top
	Polygon* pg=a->createPolygon();
	pg->append(0);
	pg->append(1);
	pg->append(2);
	pg->append(3);
	pg->append(0);

	pg=a->createPolygon();
	pg->append(4);
	pg->append(0);

	pg=a->createPolygon();
	pg->append(5);
	pg->append(1);

	pg=a->createPolygon();
	pg->append(6);
	pg->append(2);

	pg=a->createPolygon();
	pg->append(7);
	pg->append(3);

	//Bottom
	pg=a->createPolygon();
	pg->append(7);
	pg->append(6);
	pg->append(5);
	pg->append(4);
	pg->append(7);

	result->appendChild(a);

	reporter->reportMessage(tr("Bounds: [%1],[%2]").arg(to_string(lower)).arg(to_string(upper)));
#endif
}

void NodeEvaluator::visit(SubDivisionNode* n)
{
	evaluate(n,Union);
#if defined(USE_CGAL) && defined(USE_SUBDIV)
	auto* cp=static_cast<CGALPrimitive*>(result);
	CGAL::Polyhedron3& p=*cp->getPolyhedron();
	CGAL::Subdivision_method_3::Loop_subdivision(p,n->getLevel());
	result=new CGALPrimitive(p);
#endif
}

void NodeEvaluator::visit(NormalsNode* n)
{
	evaluate(n,Union);
#ifdef USE_CGAL
	CGALExplorer e(result);
	CGALPrimitive* prim=e.getPrimitive();

	auto* a=new Polyhedron();
	a->setType(Primitive::Skeleton);
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
		CGAL::Point3 p1=c+b;
		CGAL::Point3 p2=c-b;

		Polygon* np=a->createPolygon();
		a->createVertex(CGAL::Point3(p1.x(),p1.y(),p1.z()));
		a->createVertex(CGAL::Point3(n.x(),n.y(),n.z()));
		a->createVertex(CGAL::Point3(p2.x(),p2.y(),p2.z()));
		np->append(i++);
		np->append(i++);
		np->append(i++);
	}

	result->appendChild(a);
#endif
}

void NodeEvaluator::visit(SimplifyNode* n)
{
	evaluate(n,Union);
	if(result)
		result=result->simplify(n->getStopLevel());
}

void NodeEvaluator::visit(ChildrenNode* n)
{
	if(n->getIndexes().count()<=0) {
		evaluate(n,Union);
	} else {
		QList<Node*> allChildren=n->getChildren();
		QList<Node*> children;
		for(auto i: n->getIndexes()) {
			if(i<allChildren.count())
				children.append(allChildren.at(i));
		}
		evaluate(children,Union,nullptr);
	}
}

void NodeEvaluator::visit(OffsetNode* n)
{
	evaluate(n,Union);

	result=result->inset(n->getAmount());
}

void NodeEvaluator::visit(BoundaryNode* op)
{
	evaluate(op,Union);

#ifdef USE_CGAL
	if(result->isFullyDimentional()) {
		result=result->boundary();
	} else {
		CGALExplorer explorer(result);
		result=explorer.getPerimeters();
	}
#endif
}

void NodeEvaluator::visit(ImportNode* op)
{
#ifdef USE_CGAL
	CGALImport i(reporter);
	result=i.import(op->getImport());
#else
	result=nullptr;
#endif
}

void NodeEvaluator::visit(TransformationNode* tr)
{
	evaluate(tr,Union);
	if(result)
		result->transform(tr->getMatrix());
}

void NodeEvaluator::visit(ResizeNode* n)
{
	evaluate(n,Union);
#ifdef USE_CGAL
	auto* pr=static_cast<CGALPrimitive*>(result);
	CGAL::Cuboid3 b=pr->getBounds();
	CGAL::Point3 s=n->getSize();
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
	if(!n->getAutoSize())
		autosize=1.0;

	if(x==0.0) x=autosize;
	if(y==0.0) y=autosize;
	if(z==0.0) z=autosize;

	TransformMatrix* t = new TransformMatrix(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1);

	result->transform(t);
#endif
}

void NodeEvaluator::visit(AlignNode* n)
{
	evaluate(n,Union);
#ifdef USE_CGAL
	auto* pr=static_cast<CGALPrimitive*>(result);
	CGAL::Cuboid3 b=pr->getBounds();
	CGAL::Scalar cx=0.0,cy=0.0,cz=0.0;
	CGAL::Scalar two(2.0);
	if(n->getCenter()) {
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
		for(AlignNode::Face_t a: n->getAlign()) {
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

	auto* t = new TransformMatrix(
		1, 0, 0, -cx,
		0, 1, 0, -cy,
		0, 0, 1, -cz,
		0, 0, 0,  1);

	result->transform(t);
#endif
}

void NodeEvaluator::visit(PointNode* n)
{
	Primitive* cp=createPrimitive();
	cp->setType(Primitive::Single);
	cp->createPolygon();
	cp->createVertex(n->getPoint());
	result=cp;
}

void NodeEvaluator::visit(SliceNode* n)
{
	evaluate(n,Union);
#ifdef USE_CGAL
	auto* pr=static_cast<CGALPrimitive*>(result);
	CGAL::Cuboid3 b=pr->getBounds();

	auto* cp=new CGALPrimitive();
	const CGAL::Scalar& h=n->getHeight();
	const CGAL::Scalar& xmin=b.xmin();
	const CGAL::Scalar& ymin=b.ymin();
	const CGAL::Scalar& xmax=b.xmax();
	const CGAL::Scalar& ymax=b.ymax();

	CGALBuilder bd(cp);
	bd.makeSideZ(xmin,xmax,ymin,ymax,h);

	CGAL::Scalar t=n->getThickness();
	if(t>0.0) {
		const CGAL::Scalar& z=h+t;
		bd.makeSideY(xmax,xmin,ymin,h,z);
		bd.makeSideX(xmax,ymax,ymin,h,z);
		bd.makeSideY(xmin,xmax,ymax,h,z);
		bd.makeSideX(xmin,ymin,ymax,h,z);

		bd.makeSideZ(xmin,xmax,ymax,ymin,z);
	}

	result=result->intersection(cp);
#endif
}

void NodeEvaluator::visit(ProductNode* p)
{
	Primitive* prim = p->getPrimitive();
	result=prim->copy();
}

void NodeEvaluator::visit(ProjectionNode* op)
{
	evaluate(op,Union);

#ifdef USE_CGAL
	CGALExplorer explorer(result);

	bool base=op->getBase();
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

void NodeEvaluator::visit(DecomposeNode* n)
{
	evaluate(n,Union);
	result=result->decompose();
}

void NodeEvaluator::visit(ComplementNode* n)
{
	evaluate(n,Union);
	result=result->complement();
}

void NodeEvaluator::visit(RadialsNode* n)
{
	evaluate(n,Union);
#ifdef USE_CGAL
	auto* pr=static_cast<CGALPrimitive*>(result);
	CGAL::Circle3 circle=pr->getRadius();
	CGAL::Scalar r=r_sqrt(circle.squared_radius());
	QString rs=to_string(r);
	reporter->reportMessage(tr("Radius: %1").arg(rs));

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
	p->setType(Primitive::Skeleton);
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

void NodeEvaluator::visit(VolumesNode* n)
{
	evaluate(n,Union);
#ifdef USE_CGAL
	auto* pr=static_cast<CGALPrimitive*>(result);
	bool calcMass = n->getCalcMass();
	CGALVolume v=pr->getVolume(calcMass);

	CGAL::Scalar vn=v.getSize();
	QString vs=to_string(vn);
	reporter->reportMessage(tr("Volume: %1").arg(vs));

	CGAL::Point3 c=v.getCenter();

	if(calcMass)
		reporter->reportMessage(tr("Center of Mass: %1").arg(to_string(c)));

	CGAL::Cuboid3 b=v.getBounds();
	CGAL::Scalar x,y,z;
	x=b.xmax()+((b.xmax()-b.xmin())/10);
	y=b.ymax()+((b.ymax()-b.ymin())/10);
	z=b.zmax()+((b.zmax()-b.zmin())/10);
	CGAL::Point3 tr(x,y,z);

	auto* p = new Polyhedron();
	p->setType(Primitive::Skeleton);
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
