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

#ifndef NODEEVALUATOR_H
#define NODEEVALUATOR_H

#include <QString>
#include <QTextStream>
#include "cgalbuilder.h"
#include "cgalprimitive.h"
#include "nodevisitor.h"
#include "primitivenode.h"
#include "unionnode.h"
#include "differencenode.h"
#include "intersectionnode.h"
#include "symmetricdifferencenode.h"
#include "minkowskinode.h"
#include "transformationnode.h"
#include "hullnode.h"
#include "linearextrudenode.h"
#include "boundsnode.h"
#include "subdivisionnode.h"
#include "cgalexplorer.h"

class NodeEvaluator : public NodeVisitor, private CGALBuilder
{
public:
	enum Operation_e {
		Union,
		Difference,
		Intersection,
		SymmetricDifference,
		Minkowski
	};

	NodeEvaluator(QTextStream&);
	void visit(PrimitiveNode*);
	void visit(UnionNode*);
	void visit(DifferenceNode*);
	void visit(IntersectionNode*);
	void visit(SymmetricDifferenceNode*);
	void visit(MinkowskiNode*);
	void visit(HullNode*);
	Point convert(const CGAL::Point3&,double);
	void evaluate(Node*,Operation_e);
	void visit(LinearExtrudeNode*);
	void visit(BoundsNode*);
	void visit(SubDivisionNode*);
	void visit(TransformationNode*);
	CGALPrimitive* getResult() const;
private:
	CGALPrimitive* result;
	QTextStream& output;
};

#endif // NODEEVALUATOR_H
