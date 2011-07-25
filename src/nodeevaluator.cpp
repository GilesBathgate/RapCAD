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

#include <QVector>
#include <CGAL/Subdivision_method_3.h>
#include "nodeevaluator.h"
#include "cgalimport.h"

NodeEvaluator::NodeEvaluator(QTextStream& s) : output(s)
{
}

void NodeEvaluator::visit(PrimitiveNode* n)
{
	CGALPrimitive* cp = new CGALPrimitive();
	foreach(Polygon p, n->getPolygons()) {
		cp->createPolygon();
		foreach(Point pt, p) {
			double x,y,z;
			pt.getXYZ(x,y,z);
			cp->appendVertex(CGAL::Point3(x,y,z));
		}
	}
	result=cp->buildVolume();
}

void NodeEvaluator::visit(PolylineNode* n)
{
	QVector<CGAL::Point3> pl;
	foreach(Point p,n->getPoints()) {
		double x,y,z;
		p.getXYZ(x,y,z);
		pl.append(CGAL::Point3(x,y,z));
	}
	result=new CGALPrimitive(pl);
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
	CGALPrimitive* first=NULL;
	foreach(Node* n, op->getChildren()) {
		n->accept(*this);
		if(!first) {
			CGALExplorer explorer(result->getNefPolyhedron());
			QList<CGAL::Point3> points = explorer.getPoints();

			QVector<CGAL::Point3> pl;
			CGAL::Point3 fp;
			for(int i=0; i<points.size(); i++) {
				if(i==0)
					fp=points.at(i);
				pl.append(points.at(i));
			}
			if(op->getClosed())
				pl.append(fp);

			first=new CGALPrimitive(pl);
		} else {
			first=first->minkowski(result);
		}
	}

	result=first;
}

void NodeEvaluator::visit(HullNode* n)
{
	QList<CGAL::Point3> points;
	foreach(Node* c,n->getChildren()) {
		c->accept(*this);
		CGALExplorer explorer(result->getNefPolyhedron());
		foreach(CGAL::Point3 pt,explorer.getPoints())
			points.append(pt);
	}

	CGAL::Object hull;
	CGAL::convex_hull_3(points.begin(),points.end(),hull);

	CGAL::Polyhedron3 poly=CGAL::object_cast<CGAL::Polyhedron3>(hull);
	result=new CGALPrimitive(poly);
}

void NodeEvaluator::visit(LinearExtrudeNode* op)
{
	evaluate(op,Union);

	CGAL::NefPolyhedron3 r=result->getNefPolyhedron();

	//For fully dimentional polyhedra there are always two volumes the outer
	//volume and the inner volume. So check volumes > 1
	if(r.number_of_volumes()>1) {
		QVector<CGAL::Point3> pl;
		pl.append(CGAL::Point3(0,0,0));
		pl.append(CGAL::Point3(0,0,op->getHeight()));
		CGALPrimitive* prim=new CGALPrimitive(pl);
		result=result->minkowski(prim);

	} else {
		CGAL::Kernel3::FT z=op->getHeight();
		CGALExplorer explorer(r);
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

		result=n->buildVolume();
	}

}

CGAL::Point3 NodeEvaluator::offset(const CGAL::Point3& p,CGAL::Kernel3::FT z)
{
	z+=p.z();
	return CGAL::Point3(p.x(),p.y(),z);
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

	CGALExplorer explorer(result->getNefPolyhedron());
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
	/*
		CGAL::Polyhedron3 p;
		result->getPoly3().convert_to_Polyhedron(p);

		CGAL::Subdivision_method_3::CatmullClark_subdivision(p,n->getLevel());

		//TODO For some reason the resulting CGAL::Polyhedron3 will not convert
		//back to a Nef Poly!
		if(p.is_closed()) {
			result=new CGALPrimitive(p);
		} else {
			output << "Polygon is not closed";
		}
	*/
}

void NodeEvaluator::visit(OffsetNode* n)
{
	evaluate(n,Union);

	result=result->inset(n->getAmount());
}

void NodeEvaluator::visit(OutlineNode* op)
{
	evaluate(op,Union);

	CGALExplorer explorer(result->getNefPolyhedron());
	QList<CGAL::Point3> points = explorer.getPoints();

	QVector<CGAL::Point3> pl;
	CGAL::Point3 fp;
	for(int i=0; i<points.size(); i++) {
		if(i==0)
			fp=points.at(i);
		pl.append(points.at(i));
	}
	pl.append(fp);

	result=new CGALPrimitive(pl);
}

void NodeEvaluator::visit(ImportNode* op)
{
	CGALImport i;
	result=i.importOFF(op->getImport());
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
