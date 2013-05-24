/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2013 Giles Bathgate
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

#if USE_CGAL
#include "cgalimport.h"
#include "cgalexplorer.h"
#include "cgalprimitive.h"
#else
#include "nullprimitive.h"
#endif

NodeEvaluator::NodeEvaluator(QTextStream& s) : output(s)
{
}

NodeEvaluator::~NodeEvaluator()
{
	Node::cleanup();
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
		foreach(Point pt, p) {
			cp->appendVertex(pt);
		}
	}
	result=cp->buildPrimitive();
}

void NodeEvaluator::visit(PolylineNode* n)
{
	Primitive* cp=createPrimitive();
	foreach(Point p,n->getPoints()) {
		cp->appendVertex(p);
	}
	result=cp->buildPrimitive();
}

void NodeEvaluator::visit(UnionNode* op)
{
	evaluate(op,Union);
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
			for(int i=0; i<points.size(); i++) {
				if(i==0)
					fp=points.at(i);
				cp->appendVertex(points.at(i));
			}
			if(op->getClosed())
				cp->appendVertex(fp);
			first=cp->buildPrimitive();
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
static CGAL::Point3 offset(const CGAL::Point3& p,CGAL::Kernel3::FT z)
{
	z+=p.z();
	return CGAL::Point3(p.x(),p.y(),z);
}
#endif

void NodeEvaluator::visit(LinearExtrudeNode* op)
{
	evaluate(op,Union);

	if(result->isFullyDimentional()) {
		Primitive* cp=createPrimitive();
		cp->appendVertex(Point());
		cp->appendVertex(Point(0,0,op->getHeight()));
		cp->buildPrimitive();
		result=result->minkowski(cp);
	} else {
#if USE_CGAL
		CGAL::Kernel3::FT z=op->getHeight();
		CGALExplorer explorer(result);
		CGALPrimitive* prim=explorer.getPrimitive();
		QList<CGALPolygon*> polys=prim->getPolygons();
		CGALPrimitive* n = new CGALPrimitive();

		foreach(CGALPolygon* pg,polys) {
			n->createPolygon();
			bool up =(pg->getNormal().z()>0);
			foreach(CGAL::Point3 pt,pg->getPoints()) {
				if(up)
					n->appendVertex(pt);
				else
					n->prependVertex(pt);
			}
		}

		foreach(CGALExplorer::HalfEdgeHandle h, explorer.getPerimeter()) {
			n->createPolygon();
			n->appendVertex(offset(h->source()->point(),z));
			n->appendVertex(offset(h->target()->point(),z));
			n->appendVertex(h->target()->point());
			n->appendVertex(h->source()->point());
		}

		foreach(CGALPolygon* pg,polys) {
			n->createPolygon();
			bool up =(pg->getNormal().z()>0);
			foreach(CGAL::Point3 pt,pg->getPoints()) {
				if(up)
					n->prependVertex(offset(pt,z));
				else
					n->appendVertex(offset(pt,z));
			}
		}

		result=n->buildPrimitive();
#endif
	}
}

void NodeEvaluator::visit(RotateExtrudeNode*)
{
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
				first=first->join(result);
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

	result=first;
}

void NodeEvaluator::visit(BoundsNode* n)
{
	evaluate(n,Union);
#if USE_CGAL
	CGALExplorer explorer(result);
	CGAL::Bbox_3 b=explorer.getBounds();

	//TODO move this warning into gcode generation routines when they exist.
	if(b.zmin()!=0.0) {
		QString where = b.zmin()<0.0?" below ":" above ";
		output << "Warning: The model is " << b.zmin() << where << "the build platform.\n";
	}

	output << "Bounds: ";
	output << "[" << b.xmin() << "," << b.ymin() << "," << b.zmin() << "] ";
	output << "[" << b.xmax() << "," << b.ymax() << "," << b.zmax() << "]\n";
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
	QList<CGAL::Point3> points = explorer.getPoints();

	CGALPrimitive* cp=new CGALPrimitive();
	CGAL::Point3 fp;
	for(int i=0; i<points.size(); i++) {
		if(i==0)
			fp=points.at(i);
		cp->appendVertex(points.at(i));
	}
	cp->appendVertex(fp);

	result=cp->buildPrimitive();
#endif
}

void NodeEvaluator::visit(ImportNode* op)
{
#if USE_CGAL
	CGALImport i(output);
	result=i.import(op->getImport());
#endif
}

void NodeEvaluator::visit(TransformationNode* tr)
{
	evaluate(tr,Union);
#if USE_CGAL
	double* m=tr->matrix;
	CGAL::AffTransformation3 t(
		m[0], m[4], m[ 8], m[12],
		m[1], m[5], m[ 9], m[13],
		m[2], m[6], m[10], m[14], m[15]);

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
	CGAL::Bbox_3 b=e.getBounds();
	Point s=n->getSize();
	double x,y,z;
	s.getXYZ(x,y,z);

	double autosize=1.0;
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
	CGAL::Bbox_3 b=e.getBounds();
	double x,y,z;
	x=(b.xmin()+b.xmax())/2;
	y=(b.ymin()+b.ymax())/2;
	z=(b.zmin()+b.zmax())/2;


	CGAL::AffTransformation3 t(
		1, 0, 0, -x,
		0, 1, 0, -y,
		0, 0, 1, -z, 1);

	CGALPrimitive* pr=dynamic_cast<CGALPrimitive*>(result);
	if(pr)
		pr->transform(t);
#endif
}

void NodeEvaluator::visit(PointNode* n)
{
	Primitive* cp=createPrimitive();
	cp->appendVertex(n->getPoint());
	result=cp->buildPrimitive();
}

void NodeEvaluator::visit(SliceNode* n)
{
	evaluate(n,Union);
#if USE_CGAL
	CGALExplorer e(result);
	CGAL::Bbox_3 b=e.getBounds();

	CGALPrimitive* cp = new CGALPrimitive();
	cp->createPolygon();
	double h = n->getHeight();
	cp->appendVertex(CGAL::Point3(b.xmin(),b.ymin(),h));
	cp->appendVertex(CGAL::Point3(b.xmin(),b.ymax(),h));
	cp->appendVertex(CGAL::Point3(b.xmax(),b.ymax(),h));
	cp->appendVertex(CGAL::Point3(b.xmax(),b.ymin(),h));

	cp->buildPrimitive();

	result=result->intersection(cp);
#endif
}

Primitive* NodeEvaluator::getResult() const
{
	return result;
}
