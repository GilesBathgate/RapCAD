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

#ifndef NODEPRINTER_H
#define NODEPRINTER_H

#include <QTextStream>
#include "nodevisitor.h"
#include "polyhedron.h"
#include "node/primitivenode.h"
#include "node/polylinenode.h"
#include "node/unionnode.h"
#include "node/groupnode.h"
#include "node/differencenode.h"
#include "node/intersectionnode.h"
#include "node/symmetricdifferencenode.h"
#include "node/minkowskinode.h"
#include "node/glidenode.h"
#include "node/transformationnode.h"
#include "node/linearextrudenode.h"
#include "node/rotateextrudenode.h"
#include "node/hullnode.h"
#include "node/boundsnode.h"
#include "node/subdivisionnode.h"
#include "node/offsetnode.h"
#include "node/boundarynode.h"
#include "node/importnode.h"
#include "node/resizenode.h"
#include "node/alignnode.h"
#include "node/pointnode.h"
#include "node/slicenode.h"
#include "node/productnode.h"
#include "node/projectionnode.h"
#include "node/decomposenode.h"
#include "node/complementnode.h"
#include "node/radialsnode.h"
#include "node/volumesnode.h"
#include "node/triangulatenode.h"
#include "node/materialnode.h"
#include "node/discretenode.h"
#include "node/normalsnode.h"
#include "node/simplifynode.h"
#include "node/childrennode.h"

#ifdef USE_CGAL
#include "cgalprimitive.h"
#endif

class NodePrinter : public NodeVisitor
{
public:
	NodePrinter(QTextStream&);
	void visit(PrimitiveNode*) override;
	void visit(PolylineNode*) override;
	void visit(UnionNode*) override;
	void visit(GroupNode*) override;
	void visit(DifferenceNode*) override;
	void visit(IntersectionNode*) override;
	void visit(SymmetricDifferenceNode*) override;
	void visit(MinkowskiNode*) override;
	void visit(GlideNode*) override;
	void visit(HullNode*) override;
	void visit(LinearExtrudeNode*) override;
	void visit(RotateExtrudeNode*) override;
	void visit(BoundsNode*) override;
	void visit(SubDivisionNode*) override;
	void visit(OffsetNode*) override;
	void visit(BoundaryNode*) override;
	void visit(ImportNode*) override;
	void visit(TransformationNode*) override;
	void visit(ResizeNode*) override;
	void visit(AlignNode*) override;
	void visit(PointNode*) override;
	void visit(SliceNode*) override;
	void visit(ProductNode*) override;
	void visit(ProjectionNode*) override;
	void visit(DecomposeNode*) override;
	void visit(ComplementNode*) override;
	void visit(RadialsNode*) override;
	void visit(VolumesNode*) override;
	void visit(TriangulateNode*) override;
	void visit(MaterialNode*) override;
	void visit(DiscreteNode*) override;
	void visit(NormalsNode*) override;
	void visit(SimplifyNode*) override;
	void visit(ChildrenNode*) override;
private:
	QTextStream& result;
	void printChildren(Node*);
	void printArguments(Point);
	void printArguments(Polygon);
	void printPrimitive(Primitive* pr);
	void printPrimitive(Polyhedron*);
#ifdef USE_CGAL
	void printPrimitive(CGALPrimitive*);
	void printPoint(CGAL::Point3,bool);
#endif
	void printArguments(QList<AlignNode::Face_t> t);
	void printArguments(QString,bool);
	void printArguments(int);
};

#endif // NODEPRINTER_H
