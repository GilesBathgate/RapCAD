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

#include "nodeprinter.h"
#include "onceonly.h"

NodePrinter::NodePrinter(QTextStream& s) : result(s)
{
}

void NodePrinter::visit(PrimitiveNode* n)
{
	result << "polyhedron([";
	printPrimitive(n->getPrimitive());
	result << "])";
	printChildren(n);
}

void NodePrinter::printPrimitive(Primitive* pr)
{
#if USE_CGAL
	CGALPrimitive* cp=dynamic_cast<CGALPrimitive*>(pr);
	if(cp)
		printPrimitive(cp);
#endif
	Polyhedron* ph=dynamic_cast<Polyhedron*>(pr);
	if(ph)
		printPrimitive(ph);
}

void NodePrinter::printPrimitive(Polyhedron* ph)
{
	OnceOnly first;
	foreach(Point p, ph->getPoints()) {
		if(!first())
			result << ",";
		result << p.toString();
	}
	result << "],[";

	OnceOnly first_pg;
	foreach(Polygon* pg,ph->getPolygons()) {
		if(!first_pg())
			result << ",";
		result << "[";

		OnceOnly first_p;
		foreach(int i,pg->getIndexes()) {
			if(!first_p())
				result << ",";
			result << QString().setNum(i);
		}
		result << "]";
	}
}

#if USE_CGAL

void NodePrinter::printPoint(CGAL::Point3 p, bool trim)
{
	result << "[";
	result << to_string(p.x(),trim);
	result << ",";
	result << to_string(p.y(),trim);
	result << ",";
	result << to_string(p.z(),trim);
	result << "]";
}

void NodePrinter::printPrimitive(CGALPrimitive* pr)
{
	OnceOnly first;
	foreach(CGAL::Point3 p, pr->getCGALPoints()) {
		if(!first())
			result << ",";
		printPoint(p,true);
	}
	result << "],[";

	OnceOnly first_pg;
	foreach(Polygon* pg,pr->getCGALPolygons()) {
		if(!first_pg())
			result << ",";
		result << "[";

		OnceOnly first_p;
		foreach(int i,pg->getIndexes()) {
			if(!first_p())
				result << ",";
			result << QString().setNum(i);
		}
		result << "]";
	}
}
#endif

void NodePrinter::visit(PolylineNode* n)
{
	result << "polyline([";
	printPrimitive(n->getPrimitive());
	result << "]);";
}

void NodePrinter::visit(UnionNode* n)
{
	result << "union()";
	printChildren(n);
}

void NodePrinter::visit(GroupNode* n)
{
	result << "group()";
	printChildren(n);
}

void NodePrinter::visit(DifferenceNode* n)
{
	result << "difference()";
	printChildren(n);
}

void NodePrinter::visit(IntersectionNode* n)
{
	result << "intersection()";
	printChildren(n);
}

void NodePrinter::visit(SymmetricDifferenceNode* n)
{
	result << "symmetric_difference()";
	printChildren(n);
}

void NodePrinter::visit(MinkowskiNode* n)
{
	result << "minkowski()";
	printChildren(n);
}

void NodePrinter::visit(GlideNode* n)
{
	result << "glide()";
	printChildren(n);
}

void NodePrinter::visit(HullNode* n)
{
	if(n->getChain()) {
		result << "chain_hull(";
		if(n->getClosed())
			result << "true";
		result << ")";
	} else {
		result << "hull()";
	}
	printChildren(n);
}

void NodePrinter::visit(LinearExtrudeNode* n)
{
	result << "linear_extrude(";
	result << to_string(n->getHeight());
	result << ")";
	printChildren(n);
}

void NodePrinter::visit(RotateExtrudeNode* n)
{
	result << "rotate_extrude(";
	result << to_string(n->getRadius());
	result << ",";
	result << n->getAxis().toString();
	result << ")";
	printChildren(n);
}

void NodePrinter::visit(BoundsNode* n)
{
	result << "bound$()";
	printChildren(n);
}

void NodePrinter::visit(SubDivisionNode* n)
{
	result << "subdiv()";
	printChildren(n);
}

void NodePrinter::visit(OffsetNode* n)
{
	result << "offset(";
	result << to_string(n->getAmount());
	result << ")";
	printChildren(n);
}

void NodePrinter::visit(OutlineNode* n)
{
	result << "outline()";
	printChildren(n);
}

void NodePrinter::visit(ImportNode*)
{
}

void NodePrinter::printChildren(Node* n)
{
	QList<Node*> children = n->getChildren();
	if(children.length()>0) {
		result << "{";
		foreach(Node* c,children)
			c->accept(*this);
		result << "}";
	} else {
		result << ";";
	}
}

void NodePrinter::printArguments(Point p)
{
	result << "(";
	result << p.toString();
	result << ")";
}

void NodePrinter::printArguments(Polygon pg)
{
	result << "([";
	OnceOnly first;
	foreach(Point p, pg.getPoints()) {
		if(!first())
			result << ",";
		result << p.toString();
	}
	result << "])";
}

void NodePrinter::printArguments(QList<AlignNode::Face_t> t)
{
	result << "(";
	OnceOnly first;
	foreach(AlignNode::Face_t a, t) {
		if(!first())
			result << ",";
		switch(a) {
		case AlignNode::Top:
			result << "top";
			break;
		case AlignNode::Bottom:
			result << "bottom";
			break;
		case AlignNode::North:
			result << "north";
			break;
		case AlignNode::South:
			result << "south";
			break;
		case AlignNode::West:
			result << "west";
			break;
		case AlignNode::East:
			result << "east";
			break;
		}
		result << "=true";
	}
	result << ")";
}

void NodePrinter::visit(ResizeNode* n)
{
	result << "resize";
	printArguments(n->getSize());
	printChildren(n);
}

void NodePrinter::visit(AlignNode* n)
{
	if(n->getCenter()) {
		result << "center()";
	} else {
		result << "align";
		printArguments(n->getAlign());
	}
	printChildren(n);
}

void NodePrinter::visit(PointNode* n)
{
	result << "point";
	printArguments(n->getPoint());
	printChildren(n);
}

void NodePrinter::visit(SliceNode* n)
{
	result << "slice(";
	result << to_string(n->getHeight()) << "," << to_string(n->getThickness());
	result << ")";
	printChildren(n);
}

void NodePrinter::visit(ProductNode*)
{
}

void NodePrinter::visit(ProjectionNode* n)
{
	result << "projection()";
	printChildren(n);
}

void NodePrinter::visit(DecomposeNode* n)
{
	result << "decompose()";
	printChildren(n);
}

void NodePrinter::visit(ComplementNode* n)
{
	result << "complement()";
	printChildren(n);
}

void NodePrinter::visit(RadialsNode* n)
{
	result << "radial$()";
	printChildren(n);
}

void NodePrinter::visit(VolumesNode* n)
{
	result << "volume$()";
	printChildren(n);
}

void NodePrinter::visit(TriangulateNode* n)
{
	result << "triangulate()";
	printChildren(n);
}

void NodePrinter::visit(MaterialNode* n)
{
	result << "material()";
	printChildren(n);
}

void NodePrinter::visit(DiscreteNode* n)
{
	result << "discrete()";
	printChildren(n);
}

void NodePrinter::visit(NormalsNode* n)
{
	result << "normal$()";
	printChildren(n);
}

void NodePrinter::visit(SimplifyNode* n)
{
	result << "simplify()";
	printChildren(n);
}

void NodePrinter::visit(ChildrenNode* n)
{
	result << "children()";
	printChildren(n);
}

void NodePrinter::visit(TransformationNode* n)
{
	TransformMatrix* m=n->getMatrix();
	result << "multmatrix(";
	result << m->toString();
	result << ")";
	printChildren(n);
}
