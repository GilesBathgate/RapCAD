/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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
#include "tau.h"
#include "onceonly.h"

#if USE_CGAL
#include "cgalimport.h"
#include "cgalexplorer.h"
#include "cgalprimitive.h"
#include "cgalfragment.h"
#else
#include "nullprimitive.h"
#endif

NodeEvaluator::NodeEvaluator(QTextStream& s) : output(s)
{
}

Primitive* NodeEvaluator::createPrimitive()
{
#if USE_CGAL
	return new CGALPrimitive();
#else
	return new NullPrimitive();
#endif
}

void NodeEvaluator::visit(PrimitiveNode* n)
{
	Primitive* cp=createPrimitive();
	foreach(Polygon p, n->getPolygons()) {
		cp->createPolygon();
		foreach(Point pt, p.getPoints()) {
			cp->appendVertex(pt);
		}
	}
	result=cp;
}

void NodeEvaluator::visit(PolylineNode* n)
{
	Primitive* cp=createPrimitive();
	cp->setSkeleton(true);

	foreach(Polygon p, n->getPolygons()) {
		cp->createPolygon();
		foreach(Point pt,p.getPoints()) {
			cp->appendVertex(pt);
		}
	}

	result=cp;
}

void NodeEvaluator::visit(UnionNode* op)
{
	evaluate(op,Union);
}

void NodeEvaluator::visit(GroupNode* op)
{
	QList<Primitive*> primitives;
	foreach(Node* n, op->getChildren()) {
		n->accept(*this);
		primitives.append(result);
	}

	/*TODO check if bounding boxes of primitives
	 * intersect and if they do fall back to union
	 */
	if(primitives.count()>1)
	{
		CGALExplorer e(primitives);
		result=e.getPrimitive();
	}
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
	Primitive* first=NULL;
	foreach(Node* n, op->getChildren()) {
		n->accept(*this);
		if(!first) {
#if USE_CGAL
			CGALExplorer explorer(result);
			QList<CGAL::Point3> points = explorer.getPoints();

			CGALPrimitive* cp=new CGALPrimitive();
			CGAL::Point3 fp;
			OnceOnly first_p;
			foreach(CGAL::Point3 pt,points) {
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

void NodeEvaluator::visit(HullNode* n)
{
#if USE_CGAL
	QList<CGAL::Point3> points;
	foreach(Node* c,n->getChildren()) {
		c->accept(*this);
		CGALExplorer explorer(result);
		points.append(explorer.getPoints());
	}

	CGAL::Polyhedron3 hull;
	CGAL::convex_hull_3(points.begin(),points.end(),hull);

	result=new CGALPrimitive(hull);
#endif
}

#if USE_CGAL
static CGAL::Point3 flatten(const CGAL::Point3& p)
{
	return CGAL::Point3(p.x(),p.y(),0);
}

static CGAL::Point3 translate_z(const CGAL::Point3& p,CGAL::FT z)
{
	z+=p.z();
	return CGAL::Point3(p.x(),p.y(),z);
}

static CGAL::Point3 translate_x(const CGAL::Point3& p,CGAL::FT x)
{
	x+=p.x();
	return CGAL::Point3(x,p.y(),p.z());
}

static CGAL::Point3 translate_zx(const CGAL::Point3& p,CGAL::FT x,CGAL::FT z)
{
	x+=p.x();
	z+=p.z();
	return CGAL::Point3(x,p.y(),z);
}

static CGAL::Point3 rotate(const CGAL::Point3& p,double phi)
{
	CGAL::FT h,x,z;
	x=p.x();
	z=sin(phi)*x;
	x=(cos(phi)*x)-x;
	return translate_zx(p,x,z);
}
#endif

void NodeEvaluator::visit(LinearExtrudeNode* op)
{
	evaluate(op,Union);

	if(result->isFullyDimentional()) {
		Primitive* cp=createPrimitive();
		cp->appendVertex(Point());
		cp->appendVertex(Point(0,0,op->getHeight()));
		result=result->minkowski(cp);
	} else {
#if USE_CGAL
		CGAL::FT z=op->getHeight();
		CGALExplorer explorer(result);
		CGALPrimitive* prim=explorer.getPrimitive();
		QList<CGALPolygon*> polys=prim->getPolygons();
		CGALPrimitive* n = new CGALPrimitive();

		bool up;
		foreach(CGALPolygon* pg,polys) {
			n->createPolygon();
			up=(pg->getNormal().z()>0);
			foreach(CGAL::Point3 pt,pg->getPoints()) {
				if(up)
					n->appendVertex(pt);
				else
					n->prependVertex(pt);
			}
		}

		foreach(CGALPolygon* pg, explorer.getPerimeters()) {
			up=(pg->getNormal().z()>0);
			OnceOnly first;
			CGAL::Point3 pn;
			foreach(CGAL::Point3 pt, pg->getPoints()) {
				if(!first()) {
					n->createPolygon();
					CGAL::Point3 p=pn;
					CGAL::Point3 q=pt;
					if(up) {
						n->appendVertex(translate_z(p,z));
						n->appendVertex(translate_z(q,z));
						n->appendVertex(q);
						n->appendVertex(p);
					} else {
						n->prependVertex(translate_z(p,z));
						n->prependVertex(translate_z(q,z));
						n->prependVertex(q);
						n->prependVertex(p);
					}
				}
				pn=pt;
			}
			break;
		}

		foreach(CGALPolygon* pg,polys) {
			n->createPolygon();
			up=(pg->getNormal().z()>0);
			foreach(CGAL::Point3 pt,pg->getPoints()) {
				if(up)
					n->prependVertex(translate_z(pt,z));
				else
					n->appendVertex(translate_z(pt,z));
			}
		}

		result=n;
#endif
	}
}

void NodeEvaluator::visit(RotateExtrudeNode* op)
{
	evaluate(op,Union);

#if USE_CGAL
	CGAL::FT r=op->getRadius();
	CGALExplorer explorer(result);
	//CGALPrimitive* prim=explorer.getPrimitive();
	//QList<CGALPolygon*> polys=prim->getPolygons();
	CGALPrimitive* n = new CGALPrimitive();
	CGALFragment fg=op->getFragments();
	CGAL::Cuboid3 b=explorer.getBounds();
	int f=fg.getFragments((b.xmax()-b.xmin())+r);

	for(int i=0; i<f; i++) {
		int j=(i+1)%f;
		double phi=(M_TAU*i)/f;
		double nphi=(M_TAU*j)/f;

		foreach(CGALPolygon* pg,explorer.getPerimeters()) {
			OnceOnly first;
			CGAL::Point3 pn;
			foreach(CGAL::Point3 pt, pg->getPoints()) {
				if(!first()) {
					CGAL::Point3 q=translate_x(pn,r);
					CGAL::Point3 p=translate_x(pt,r);
					if(q.x()<=0.0&&p.x()<=0.0)
						continue;

					n->createPolygon();
					CGAL::Point3 q1=rotate(q,nphi);
					CGAL::Point3 p1=rotate(p,nphi);
					CGAL::Point3 p2=rotate(p,phi);
					CGAL::Point3 q2=rotate(q,phi);
					n->appendVertex(q1);
					n->appendVertex(p1);
					if(p2!=p1)
						n->appendVertex(p2);
					if(q2!=q1)
						n->appendVertex(q2);
				}
				pn=pt;
			}
			break;
		}
	}

	result=n;
#endif
}

void NodeEvaluator::evaluate(Node* op,Operation_e type)
{
	Primitive* first=NULL;
	foreach(Node* n, op->getChildren()) {
		n->accept(*this);
		if(!first) {
			first=result;
		} else {
			switch(type) {
			case Union:
				first->add(result);
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

	result=first->join();
}

void NodeEvaluator::visit(BoundsNode* n)
{
	evaluate(n,Union);
#if USE_CGAL
	CGALExplorer explorer(result);
	CGAL::Cuboid3 b=explorer.getBounds();

	//TODO move this warning into gcode generation routines when they exist.
	if(b.zmin()!=0.0) {
		QString where = b.zmin()<0.0?" below ":" above ";
		output << "Warning: The model is " << to_double(b.zmin()) << where << "the build platform.\n";
	}

	output << "Bounds: ";
	output << "[" << to_double(b.xmin()) << "," << to_double(b.ymin()) << "," << to_double(b.zmin()) << "] ";
	output << "[" << to_double(b.xmax()) << "," << to_double(b.ymax()) << "," << to_double(b.zmax()) << "]\n";
#endif
}

void NodeEvaluator::visit(SubDivisionNode* n)
{
	evaluate(n,Union);
	//TODO
}

void NodeEvaluator::visit(OffsetNode* n)
{
	evaluate(n,Union);

	result=result->inset(n->getAmount());
}

void NodeEvaluator::visit(OutlineNode* op)
{
	evaluate(op,Union);

#if USE_CGAL
	CGALExplorer explorer(result);
	CGALPrimitive* cp=new CGALPrimitive();
	cp->setSkeleton(true);

	foreach(CGALPolygon* pg,explorer.getPerimeters()) {
		cp->createPolygon();
		foreach(CGAL::Point3 pt, pg->getPoints()) {
			cp->appendVertex(pt);
		}
	}

	result=cp;
#endif
}

void NodeEvaluator::visit(ImportNode* op)
{
#if USE_CGAL
	CGALImport i(output);
	result=i.import(op->getImport());
#else
	result=NULL;
#endif
}

void NodeEvaluator::visit(TransformationNode* tr)
{
	evaluate(tr,Union);
#if USE_CGAL
	double* m=tr->matrix;
	CGAL::AffTransformation3 t(
		m[ 0], m[ 1], m[ 2], m[ 3],
		m[ 4], m[ 5], m[ 6], m[ 7],
		m[ 8], m[ 9], m[10], m[11],
	  /*m[12], m[13], m[14]*/m[15]);

	CGALPrimitive* pr=dynamic_cast<CGALPrimitive*>(result);
	if(pr)
		pr->transform(t);
#endif
}

void NodeEvaluator::visit(ResizeNode* n)
{
	evaluate(n,Union);
#if USE_CGAL
	CGALExplorer e(result);
	CGAL::Cuboid3 b=e.getBounds();
	Point s=n->getSize();
	double x1,y1,z1;
	s.getXYZ(x1,y1,z1);
	CGAL::FT x=x1,y=y1,z=z1,autosize=1.0;

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

	CGAL::AffTransformation3 t(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0, 1);

	CGALPrimitive* pr=dynamic_cast<CGALPrimitive*>(result);
	if(pr)
		pr->transform(t);
#endif
}

void NodeEvaluator::visit(CenterNode* n)
{
	evaluate(n,Union);
#if USE_CGAL
	CGALExplorer e(result);
	CGAL::Cuboid3 b=e.getBounds();
	CGAL::Point3 c(
	(b.xmin()+b.xmax())/2,
	(b.ymin()+b.ymax())/2,
	(b.zmin()+b.zmax())/2);


	CGAL::AffTransformation3 t(
		1, 0, 0, -c.x(),
		0, 1, 0, -c.y(),
		0, 0, 1, -c.z(), 1);

	CGALPrimitive* pr=dynamic_cast<CGALPrimitive*>(result);
	if(pr)
		pr->transform(t);
#endif
}

void NodeEvaluator::visit(PointNode* n)
{
	Primitive* cp=createPrimitive();
	cp->setSkeleton(true);
	cp->appendVertex(n->getPoint());
	result=cp;
}

void NodeEvaluator::visit(SliceNode* n)
{
	evaluate(n,Union);
#if USE_CGAL
	CGALExplorer e(result);
	CGAL::Cuboid3 b=e.getBounds();

	CGALPrimitive* cp = new CGALPrimitive();
	cp->createPolygon();
	double h = n->getHeight();
	cp->appendVertex(CGAL::Point3(b.xmin(),b.ymin(),h));
	cp->appendVertex(CGAL::Point3(b.xmin(),b.ymax(),h));
	cp->appendVertex(CGAL::Point3(b.xmax(),b.ymax(),h));
	cp->appendVertex(CGAL::Point3(b.xmax(),b.ymin(),h));

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

#if USE_CGAL
	CGALExplorer explorer(result);
	CGALPrimitive* cp=explorer.getPrimitive();

	Primitive* r=new CGALPrimitive();
	foreach(CGALPolygon* p, cp->getPolygons()) {
		CGAL::Vector3 normal=p->getNormal();
		if(normal.z()==0)
			continue;


		CGALPrimitive* t=new CGALPrimitive();
		t->createPolygon();
		foreach(CGAL::Point3 pt,p->getPoints()) {
			t->appendVertex(flatten(pt));
		}
		r=r->join(t);
	}
	result=r;
#endif
}

Primitive* NodeEvaluator::getResult() const
{
	return result;
}
