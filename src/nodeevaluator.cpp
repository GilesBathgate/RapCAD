/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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
#include <QVector>
#include <QPair>
#include <CGAL/Subdivision_method_3.h>

NodeEvaluator::NodeEvaluator(QTextStream& s) : output(s)
{
}

void NodeEvaluator::visit(PrimitiveNode* n)
{
	this->setPrimitive(n);
	CGAL::Polyhedron3 poly;
	poly.delegate(*this);
	CGAL::NefPolyhedron3* nefPoly=new CGAL::NefPolyhedron3(poly);
	result=new CGALPrimitive(nefPoly);
}

void NodeEvaluator::visit(PolylineNode* n)
{
	result=getPolyLine(n->getPoints());
}

CGALPrimitive* NodeEvaluator::getPolyLine(Polygon points)
{
	typedef CGAL::Point3* PointIterator;
	typedef QPair<PointIterator,PointIterator>  PointRange;
	typedef QList<PointRange> PolyLine;

	QVector<CGAL::Point3> pl;
	foreach(Point p, points) {
		double x,y,z;
		p.getXYZ(x,y,z);
		pl.append(CGAL::Point3(x,y,z));
	}
	PointRange p(pl.begin(),pl.end());

	PolyLine poly;
	poly.push_back(p);
	CGAL::NefPolyhedron3* nefPoly;
	nefPoly=new CGAL::NefPolyhedron3(poly.begin(), poly.end(), CGAL::NefPolyhedron3::Polylines_tag());
	return new CGALPrimitive(nefPoly);
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

void NodeEvaluator::visit(HullNode* n)
{
	QList<CGAL::Point3> points;
	foreach(Node* c,n->getChildren()) {
		c->accept(*this);
		CGALExplorer explorer(result->getPoly3());
		foreach(CGAL::Point3 pt,explorer.getPoints())
			points.append(pt);
	}

	CGAL::Object hull;
	CGAL::convex_hull_3(points.begin(),points.end(),hull);

	CGAL::Polyhedron3 poly=CGAL::object_cast<CGAL::Polyhedron3>(hull);
	CGAL::NefPolyhedron3* nefPoly=new CGAL::NefPolyhedron3(poly);
	result=new CGALPrimitive(nefPoly);
}

void NodeEvaluator::visit(LinearExtrudeNode* op)
{
	evaluate(op,Union);

	CGAL::NefPolyhedron3 r=result->getPoly3();

	if(r.number_of_facets()>1) {
		QList<Point> pl;
		pl.append(Point(0,0,0));
		pl.append(Point(0,0,op->getHeight()));
		CGALPrimitive* prim=getPolyLine(pl);
		result=result->minkowski(prim);

	} else {
		CGALExplorer explorer(r);
		QList<CGAL::Point3> points = explorer.getPoints();

		PrimitiveNode* n = new PrimitiveNode();
		double z=op->getHeight();

		n->createPolygon();
		foreach(CGAL::Point3 pt,points) {
			Point p=convert(pt,0.0);
			n->appendVertex(p);
		}

		int s=points.size();
		for(int i=0; i<s; i++) {
			int j=(i+1)%s;
			n->createPolygon();
			Point pi2=convert(points.at(i),z);
			n->appendVertex(pi2);
			Point pj2=convert(points.at(j),z);
			n->appendVertex(pj2);
			Point pj1=convert(points.at(j),0.0);
			n->appendVertex(pj1);
			Point pi1=convert(points.at(i),0.0);
			n->appendVertex(pi1);
		}

		n->createPolygon();
		foreach(CGAL::Point3 pt,points) {
			Point p = convert(pt,z);
			n->prependVertex(p);
		}

		this->setPrimitive(n);
		CGAL::Polyhedron3 poly;
		poly.delegate(*this);
		CGAL::NefPolyhedron3* nefPoly=new CGAL::NefPolyhedron3(poly);
		result=new CGALPrimitive(nefPoly);
	}

}

Point NodeEvaluator::convert(const CGAL::Point3& p,double z)
{
	return Point(to_double(p.x()),to_double(p.y()),to_double(p.z())+z);
}

void NodeEvaluator::evaluate(Node* op,Operation_e type)
{
	CGALPrimitive* first=NULL;
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

	CGALExplorer explorer(result->getPoly3());
	CGAL::Bbox_3 b=explorer.getBounds();

	//TODO move this warning into gcode generation routines when they exist.
	if(b.zmin()!=0.0) {
		QString where = b.zmin()<0.0?" below ":" above ";
		output << "Warning: The model is " << b.zmin() << where << "the build platform.\n";
	}

	output << "Bounds: ";
	output << "[" << b.xmin() << "," << b.ymin() << "," << b.zmin() << "] ";
	output << "[" << b.xmax() << "," << b.ymax() << "," << b.zmax() << "]\n";
}

void NodeEvaluator::visit(SubDivisionNode* n)
{
	evaluate(n,Union);

	CGAL::Polyhedron3 p;
	result->getPoly3().convert_to_Polyhedron(p);

	CGAL::Subdivision_method_3::CatmullClark_subdivision(p,n->getLevel());

	//TODO For some reason the resulting CGAL::Polyhedron3 will not convert
	//back to a Nef Poly!
	if(p.is_closed()) {
		CGAL::NefPolyhedron3* nefPoly=new CGAL::NefPolyhedron3(p);
		result=new CGALPrimitive(nefPoly);
	} else {
		output << "Polygon is not closed";
	}
}

void NodeEvaluator::visit(InsetNode* n)
{
	evaluate(n,Union);
	output << "Error: Inset not implemented!";
}

void NodeEvaluator::visit(TransformationNode* tr)
{
	evaluate(tr,Union);
	double* m=tr->matrix;
	CGAL::AffTransformation3 t(
		m[0], m[4], m[ 8], m[12],
		m[1], m[5], m[ 9], m[13],
		m[2], m[6], m[10], m[14], m[15]);

	result->transform(t);
}

CGALPrimitive* NodeEvaluator::getResult() const
{
	return result;
}
