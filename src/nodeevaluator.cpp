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

NodeEvaluator::NodeEvaluator()
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
	evaluate(n,Union);
	CGALExplorer explorer(result->getPoly3());
	CGALExplorer::Polygon points = explorer.getPoints();

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
		typedef CGAL::Point3* PointIterator;
		typedef std::pair<PointIterator,PointIterator>  PointRange;
		typedef std::list<PointRange> PolyLine;

		PolyLine poly;
		CGAL::Point3 pl[2] = {
			CGAL::Point3(0,0,0),
			CGAL::Point3(0,0,op->getHeight())
		};
		PointRange p(pl,pl+2);
		poly.push_back(p);
		CGAL::NefPolyhedron3* nefPoly;
		nefPoly=new CGAL::NefPolyhedron3(poly.begin(), poly.end(), CGAL::NefPolyhedron3::Polylines_tag());
		CGALPrimitive* prim = new CGALPrimitive(nefPoly);
		result=result->minkowski(prim);

	} else {
		CGALExplorer explorer(r);
		CGALExplorer::Polygon points = explorer.getPoints();

		PrimitiveNode* n = new PrimitiveNode();
		double z=op->getHeight();
		convert(n,points,0.0);
		/*
		int s=points.size();
		for(int i=0; i<s; i++) {
			int j=(i+1)%s;
			n->createPolygon();
			Point pi1(to_double(points.at(i).x()),to_double(points.at(i).y()),to_double(points.at(i).z()));
			n->appendVertex(pi1);
			Point pi2(to_double(points.at(i).x()),to_double(points.at(i).y()),to_double(points.at(i).z())+z);
			n->appendVertex(pi2);
			Point pj2(to_double(points.at(j).x()),to_double(points.at(j).y()),to_double(points.at(j).z())+z);
			n->appendVertex(pj2);
			Point pj1(to_double(points.at(j).x()),to_double(points.at(j).y()),to_double(points.at(j).z()));
			n->appendVertex(pj1);
		}
		*/
		convert(n,points,z);

		this->setPrimitive(n);
		CGAL::Polyhedron3 poly;
		poly.delegate(*this);
		CGAL::NefPolyhedron3* nefPoly=new CGAL::NefPolyhedron3(poly);
		result=new CGALPrimitive(nefPoly);

	}

}

void NodeEvaluator::convert(PrimitiveNode* n,CGALExplorer::Polygon points,double z)
{
	n->createPolygon();
	//bool first=true;
	//Point fp;
	foreach(CGAL::Point3 pt,points) {
		Point p(to_double(pt.x()),to_double(pt.y()),to_double(pt.z())+z);
		//if(first) {
		//	fp=p;
		//	first=false;
		//}
		n->appendVertex(p);
	}
	//n->prependVertex(fp);
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
