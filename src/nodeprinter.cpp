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

#include "nodeprinter.h"

NodePrinter::NodePrinter(QTextStream& s) : result(s)
{
}

void NodePrinter::visit(PrimitiveNode* n)
{
	result << "polyhedron([";
	QList<Point> ptlist;
	QList<Polygon> polygons=n->getPolygons();
	foreach(Polygon pg, polygons) {
		foreach(Point p, pg) {
			if(!ptlist.contains(p))
				ptlist.append(p);
		}
	}

	for(int i=0; i<ptlist.size(); i++) {
		Point p = ptlist.at(i);
		QString pt = p.toString();
		if(i>0)
			result << ",";
		result << pt;
	}
	result << "],[";

	for(int i=0; i<polygons.size(); i++) {
		Polygon pg = polygons.at(i);
		if(i>0)
			result << ",";
		result << "[";
		for(int j=0; j<pg.size(); j++) {
			if(j>0)
				result << ",";
			Point p = pg.at(j);
			int i = ptlist.indexOf(p);
			result << QString().setNum(i);
		}
		result << "]";
	}
	result << "]);";
}

void NodePrinter::visit(PolylineNode* n)
{
	printOperation(n,"polyline");
}

void NodePrinter::visit(UnionNode* n)
{
	printOperation(n,"union");
}

void NodePrinter::visit(DifferenceNode* n)
{
	printOperation(n,"difference");
}

void NodePrinter::visit(IntersectionNode* n)
{
	printOperation(n,"intersection");
}

void NodePrinter::visit(SymmetricDifferenceNode* n)
{
	printOperation(n,"symmetric_difference");
}

void NodePrinter::visit(MinkowskiNode* n)
{
	printOperation(n,"minkowski");
}

void NodePrinter::visit(GlideNode* n)
{
	printOperation(n,"glide");
}

void NodePrinter::visit(HullNode* n)
{
	printOperation(n,"hull");
}

void NodePrinter::visit(LinearExtrudeNode* n)
{
	printOperation(n,"linear_extrude");
}

void NodePrinter::visit(RotateExtrudeNode* n)
{
	printOperation(n,"rotate_extrude");
}

void NodePrinter::visit(BoundsNode* n)
{
	printOperation(n,"bounds");
}

void NodePrinter::visit(SubDivisionNode* n)
{
	printOperation(n,"subdiv");
}

void NodePrinter::visit(OffsetNode* n)
{
	printOperation(n,"offset");
}

void NodePrinter::visit(OutlineNode* n)
{
	printOperation(n,"outline");
}

void NodePrinter::visit(ImportNode* n)
{
	printOperation(n,"import");
}

void NodePrinter::printOperation(Node* n,QString name)
{
	result << name;
	result << "(){";
	foreach(Node* c,n->getChildren())
		c->accept(*this);
	result << "}";
}

void NodePrinter::visit(ResizeNode* n)
{
	printOperation(n,"resize");
}

void NodePrinter::visit(TransformationNode* n)
{
	result << "multmatrix([[";
	for(int i=0; i<16; i++) {
		if(i>0) {
			if(i%4)
				result << ",";
			else
				result << "],[";
		}
		result << n->matrix[i];

	}
	result << "]]){";
	foreach(Node* c,n->getChildren())
		c->accept(*this);
	result << "}";
}
