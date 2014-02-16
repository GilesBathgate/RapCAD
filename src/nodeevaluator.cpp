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
#include "decimal.h"
#include "polyhedron.h"

#if USE_CGAL
#include "cgalimport.h"
#include "cgalexplorer.h"
#include "cgalprimitive.h"
#include "cgalfragment.h"
#endif

NodeEvaluator::NodeEvaluator(QTextStream& s) : output(s)
{
}

Primitive* NodeEvaluator::createPrimitive()
{
#if USE_CGAL
	return new CGALPrimitive();
#else
	return new Polyhedron();
#endif
}

void NodeEvaluator::visit(PrimitiveNode* n)
{
	Primitive* cp=createPrimitive();

	Primitive* pr=n->getPrimitive();
	foreach(Polygon* p, pr->getPolygons()) {
		cp->createPolygon();
		foreach(Point pt,p->getPoints()) {
			cp->appendVertex(pt);
		}
	}
	result=cp;
}

void NodeEvaluator::visit(PolylineNode* n)
{
	Primitive* cp=createPrimitive();
	cp->setType(Primitive::Skeleton);

	Primitive* pr=n->getPrimitive();
	foreach(Polygon* p, pr->getPolygons()) {
		cp->createPolygon();
		foreach(Point pt,p->getPoints()) {
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

static CGAL::Point3 translate(const CGAL::Point3& p,CGAL::FT x,CGAL::FT y,CGAL::FT z)
{
	CGAL::AffTransformation3 t(
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z, 1);
	return p.transform(t);
}

static CGAL::Point3 rotate_y(const CGAL::Point3& p,decimal phi)
{
	decimal c=cos(phi);
	decimal s=sin(phi);
	CGAL::AffTransformation3 t(
		 c, 0, s, 0,
		 0, 1, 0, 0,
		-s, 0, c, 0, 1);

	return p.transform(t);
}

static void makeSideZ(CGALPrimitive* p,const CGAL::FT& x1,const CGAL::FT& x2,const CGAL::FT& y1,const CGAL::FT& y2,const CGAL::FT& z)
{
	p->createPolygon(); // sideZ
	p->appendVertex(CGAL::Point3(x1, y1, z));
	p->appendVertex(CGAL::Point3(x2, y1, z));
	p->appendVertex(CGAL::Point3(x2, y2, z));
	p->appendVertex(CGAL::Point3(x1, y2, z));
}

static void makeSideY(CGALPrimitive* p,const CGAL::FT& x1,const CGAL::FT& x2,const CGAL::FT& y,const CGAL::FT& z1,const CGAL::FT& z2)
{
	p->createPolygon(); // sideY
	p->appendVertex(CGAL::Point3(x1, y, z1));
	p->appendVertex(CGAL::Point3(x2, y, z1));
	p->appendVertex(CGAL::Point3(x2, y, z2));
	p->appendVertex(CGAL::Point3(x1, y, z2));
}

static void makeSideX(CGALPrimitive* p,const CGAL::FT& x,const CGAL::FT& y1,const CGAL::FT& y2,const CGAL::FT& z1,const CGAL::FT& z2)
{
	p->createPolygon(); // sideX
	p->appendVertex(CGAL::Point3(x, y1, z1));
	p->appendVertex(CGAL::Point3(x, y2, z1));
	p->appendVertex(CGAL::Point3(x, y2, z2));
	p->appendVertex(CGAL::Point3(x, y1, z2));
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
		QList<CGALPolygon*> polys=prim->getCGALPolygons();
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
						n->appendVertex(translate(p,0,0,z));
						n->appendVertex(translate(q,0,0,z));
						n->appendVertex(q);
						n->appendVertex(p);
					} else {
						n->prependVertex(translate(p,0,0,z));
						n->prependVertex(translate(q,0,0,z));
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
					n->prependVertex(translate(pt,0,0,z));
				else
					n->appendVertex(translate(pt,0,0,z));
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
		decimal phi=(M_TAU*i)/f;
		decimal nphi=(M_TAU*j)/f;

		foreach(CGALPolygon* pg,explorer.getPerimeters()) {
			OnceOnly first;
			CGAL::Point3 pn;
			foreach(CGAL::Point3 pt, pg->getPoints()) {
				if(!first()) {
					CGAL::Point3 q=translate(pn,r,0,0);
					CGAL::Point3 p=translate(pt,r,0,0);
					if(q.x()<=0.0&&p.x()<=0.0)
						continue;

					n->createPolygon();
					CGAL::Point3 q1=rotate_y(q,nphi);
					CGAL::Point3 p1=rotate_y(p,nphi);
					CGAL::Point3 p2=rotate_y(p,phi);
					CGAL::Point3 q2=rotate_y(q,phi);
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
#if USE_CGAL
	CGALExplorer explorer(result);
	CGAL::Cuboid3 b=explorer.getBounds();



	decimal xmin=to_decimal(b.xmin());
	decimal ymin=to_decimal(b.ymin());
	decimal xmax=to_decimal(b.xmax());
	decimal ymax=to_decimal(b.ymax());
	decimal zmin=to_decimal(b.zmin());
	decimal zmax=to_decimal(b.zmax());

	//TODO move this warning into gcode generation routines when they exist.
	if(zmin!=0.0) {
		QString where = zmin<0.0?" below ":" above ";
		output << "Warning: The model is " << to_string(zmin) << where << "the build platform.\n";
	}

	Point lower(xmin,ymin,zmin);
	Point upper(xmax,ymax,zmax);

	Polyhedron* a=new Polyhedron();
	a->setType(Primitive::Skeleton);
	a->createPolygon();
	a->appendVertex(lower);
	a->appendVertex(Point(xmin,ymax,zmin));
	a->appendVertex(Point(xmax,ymax,zmin));
	a->appendVertex(Point(xmax,ymin,zmin));
	a->appendVertex(lower);

	a->createPolygon();
	a->appendVertex(lower);
	a->appendVertex(Point(xmin,ymin,zmax));

	a->createPolygon();
	a->appendVertex(Point(xmin,ymax,zmin));
	a->appendVertex(Point(xmin,ymax,zmax));

	a->createPolygon();
	a->appendVertex(Point(xmax,ymin,zmin));
	a->appendVertex(Point(xmax,ymin,zmax));

	a->createPolygon();
	a->appendVertex(Point(xmax,ymax,zmin));
	a->appendVertex(upper);

	a->createPolygon();
	a->appendVertex(upper);
	a->appendVertex(Point(xmax,ymin,zmax));
	a->appendVertex(Point(xmin,ymin,zmax));
	a->appendVertex(Point(xmin,ymax,zmax));
	a->appendVertex(upper);

	result->appendChild(a);

	output << "Bounds: ";
	output << "[" << lower.toString();
	output << "," << upper.toString() << "]" << endl;
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
	cp->setType(Primitive::Skeleton);

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
	decimal* m=tr->matrix;
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
	decimal x1,y1,z1;
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
	cp->setType(Primitive::Single);
	cp->createPolygon();
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
	const CGAL::FT& h = n->getHeight();
	const CGAL::FT& xmin=b.xmin();
	const CGAL::FT& ymin=b.ymin();
	const CGAL::FT& xmax=b.xmax();
	const CGAL::FT& ymax=b.ymax();

	makeSideZ(cp,xmin,xmax,ymin,ymax,h);

	decimal t = n->getThickness();
	if(t>0.0) {
		const CGAL::FT& z=h+t;
		makeSideY(cp,xmax,xmin,ymin,h,z);
		makeSideX(cp,xmax,ymax,ymin,h,z);
		makeSideY(cp,xmin,xmax,ymax,h,z);
		makeSideX(cp,xmin,ymin,ymax,h,z);

		makeSideZ(cp,xmin,xmax,ymax,ymin,z);
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

#if USE_CGAL
	CGALExplorer explorer(result);

	bool base=op->getBase();
	if(base) {
		CGALPrimitive* r=new CGALPrimitive();
		foreach(CGALPolygon* pg,explorer.getBase()) {
			r->createPolygon();
			foreach(CGAL::Point3 pt,pg->getPoints())
				r->appendVertex(pt);
		}
		result=r;
	} else {
		CGALPrimitive* cp=explorer.getPrimitive();

		Primitive* r=new CGALPrimitive();
		foreach(CGALPolygon* p, cp->getCGALPolygons()) {
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
	}
#endif
}

void NodeEvaluator::visit(DecomposeNode* n)
{
	evaluate(n,Union);
	result->decompose();
}

Primitive* NodeEvaluator::getResult() const
{
	return result;
}
