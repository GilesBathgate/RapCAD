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

#include "nodeprinter.h"
#include "onceonly.h"

NodePrinter::NodePrinter(QTextStream& s) : result(s)
{
}

void NodePrinter::visit(const PrimitiveNode& n)
{
	Primitive* pr=n.getPrimitive();
	switch(pr->getType())
	{
		case Primitive::Lines:
			result << "polyline([";
			break;
		default:
			result << "polyhedron([";
	}

	printPrimitive(pr);
	result << "])";
	printChildren(n);
}

void NodePrinter::printPrimitive(Primitive* pr)
{
	OnceOnly first;
	for(const auto& p: pr->getPoints()) {
		if(!first())
			result << ",";
		result << to_string(p);
	}
	result << "],[";

	OnceOnly first_pg;
#ifdef USE_CGAL
	auto* cp=dynamic_cast<CGALPrimitive*>(pr);
	if(cp) {
		for(CGALPolygon* pg: cp->getCGALPolygons()) {
			if(!first_pg())
				result << ",";
			printPolygon(*pg);
		}
	}
#endif
	auto* ph=dynamic_cast<Polyhedron*>(pr);
	if(ph) {
		for(Polygon* pg: ph->getPolygons()) {
			if(!first_pg())
				result << ",";
			printPolygon(*pg);
		}
	}
}

void NodePrinter::printPolygon(const Polygon& pg)
{
	result << "[";
	OnceOnly first;
	for(auto i: pg.getIndexes()) {
		if(!first())
			result << ",";
		result << QString().setNum(i);
	}
	result << "]";
}

void NodePrinter::visit(const UnionNode& n)
{
	result << "union()";
	printChildren(n);
}

void NodePrinter::visit(const GroupNode& n)
{
	result << "group()";
	printChildren(n);
}

void NodePrinter::visit(const DifferenceNode& n)
{
	result << "difference()";
	printChildren(n);
}

void NodePrinter::visit(const IntersectionNode& n)
{
	result << "intersection()";
	printChildren(n);
}

void NodePrinter::visit(const SymmetricDifferenceNode& n)
{
	result << "symmetric_difference()";
	printChildren(n);
}

void NodePrinter::visit(const MinkowskiNode& n)
{
	result << "minkowski()";
	printChildren(n);
}

void NodePrinter::visit(const GlideNode& n)
{
	result << "glide()";
	printChildren(n);
}

void NodePrinter::visit(const HullNode& n)
{
	if(n.getChain()) {
		result << "chain_hull(";
		if(n.getClosed())
			result << "true";
		result << ")";
	} else {
		result << "hull(";
		if(n.getConcave())
			result << "concave=true";
		result << ")";
	}
	printChildren(n);
}

void NodePrinter::visit(const LinearExtrudeNode& n)
{
	result << "linear_extrude(";
	result << to_string(n.getHeight());
	result << ")";
	printChildren(n);
}

void NodePrinter::visit(const RotateExtrudeNode& n)
{
	result << "rotate_extrude(";
	result << to_string(n.getSweep());
	result << ",";
	result << to_string(n.getAxis());
	result << ",";
	result << to_string(n.getRadius());
	result << ",";
	result << to_string(n.getHeight());
	result << ")";
	printChildren(n);
}

void NodePrinter::visit(const BoundsNode& n)
{
	result << "bound$()";
	printChildren(n);
}

void NodePrinter::visit(const SubDivisionNode& n)
{
	result << "subdiv";
	printArguments(n.getLevel());
	printChildren(n);
}

void NodePrinter::visit(const OffsetNode& n)
{
	result << "offset(";
	result << to_string(n.getAmount());
	result << ")";
	printChildren(n);
}

void NodePrinter::visit(const BoundaryNode& n)
{
	result << "boundary()";
	printChildren(n);
}

void NodePrinter::visit(const ImportNode& im)
{
	result << "import(\"";
	result << im.getImport();
	result << "\");";
}

void NodePrinter::printChildren(const Node& n)
{
	QList<Node*> children = n.getChildren();
	if(children.length()>0) {
		result << "{";
		for(Node* c: children)
			c->accept(*this);
		result << "}";
	} else {
		result << ";";
	}
}

void NodePrinter::printArguments(const QString &name, bool a)
{
	result << "(";
	if(a) {
		result << name << "=true";
	}
	result << ")";
}

void NodePrinter::printArguments(int a)
{
	result << "(";
	result << a;
	result << ")";
}

void NodePrinter::printArguments(const decimal& a)
{
	result << "(";
	result << to_string(a);
	result << ")";
}

void NodePrinter::printArguments(const QList<Point>& pts)
{
	result << "(";
	if(pts.count()>1)
		result << "[";
	OnceOnly first;
	for(const auto& p: pts) {
		if(!first())
			result << ",";
		result << to_string(p);
	}
	if(pts.count()>1)
		result << "]";
	result << ")";
}

void NodePrinter::printArguments(const Polygon& pg)
{
	result << "([";
	OnceOnly first;
	for(const auto& p: pg.getPoints()) {
		if(!first())
			result << ",";
		result << to_string(p);
	}
	result << "])";
}

void NodePrinter::printArguments(const QList<int>& list)
{
	if(list.count()==0) {
		result << "()";
		return;
	}

	result << "([";
	OnceOnly first;
	for(const auto& i: list) {
		if(!first())
			result << ",";
		result << i;
	}
	result << "])";
}

void NodePrinter::printArguments(const QList<AlignNode::Face_t>& t)
{
	result << "(";
	OnceOnly first;
	for(AlignNode::Face_t a: t) {
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

void NodePrinter::visit(const ResizeNode& n)
{
	result << "resize(";
	result << to_string(n.getSize());
	if(n.getAutoSize())
		result << ",auto=true";
	result << ")";
	printChildren(n);
}

void NodePrinter::visit(const AlignNode& n)
{
	if(n.getCenter()) {
		result << "center()";
	} else {
		result << "align";
		printArguments(n.getAlign());
	}
	printChildren(n);
}

void NodePrinter::visit(const PointsNode& n)
{
	result << "points";
	printArguments(n.getPoints());
	printChildren(n);
}

void NodePrinter::visit(const SliceNode& n)
{
	result << "slice(";
	result << to_string(n.getHeight()) << "," << to_string(n.getThickness());
	result << ")";
	printChildren(n);
}

void NodePrinter::visit(const ProductNode&)
{
}

void NodePrinter::visit(const ProjectionNode& n)
{
	result << "projection";
	printArguments("base",n.getBase());
	printChildren(n);
}

void NodePrinter::visit(const DecomposeNode& n)
{
	result << "decompose()";
	printChildren(n);
}

void NodePrinter::visit(const ComplementNode& n)
{
	result << "complement()";
	printChildren(n);
}

void NodePrinter::visit(const RadialsNode& n)
{
	result << "radial$()";
	printChildren(n);
}

void NodePrinter::visit(const VolumesNode& n)
{
	result << "volume$()";
	printChildren(n);
}

void NodePrinter::visit(const TriangulateNode& n)
{
	result << "triangulate()";
	printChildren(n);
}

void NodePrinter::visit(const MaterialNode& n)
{
	result << "material()";
	printChildren(n);
}

void NodePrinter::visit(const DiscreteNode& n)
{
	result << "discrete";
	printArguments(n.getPlaces());
	printChildren(n);
}

void NodePrinter::visit(const NormalsNode& n)
{
	result << "normal$()";
	printChildren(n);
}

void NodePrinter::visit(const SimplifyNode& n)
{
	result << "simplify";
	printArguments(n.getRatio());
	printChildren(n);
}

void NodePrinter::visit(const ChildrenNode& n)
{
	result << "children";
	QList<int> idx=n.getIndexes();
	if(idx.count()==1)
		printArguments(idx[0]);
	else
		printArguments(idx);

	printChildren(n);
}

void NodePrinter::visit(const TransformationNode& n)
{
	TransformMatrix* m=n.getMatrix();
	result << "multmatrix(";
	if(m)
		result << m->toString();
	result << ")";
	printChildren(n);
}
