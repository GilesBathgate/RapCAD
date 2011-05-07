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

#ifndef NODEPRINTER_H
#define NODEPRINTER_H

#include <QTextStream>
#include "nodevisitor.h"
#include "primitivenode.h"
#include "unionnode.h"
#include "differencenode.h"
#include "intersectionnode.h"
#include "symmetricdifferencenode.h"
#include "minkowskinode.h"
#include "transformationnode.h"
#include "linearextrudenode.h"
#include "hullnode.h"
#include "boundsnode.h"

class NodePrinter : public NodeVisitor
{
public:
	NodePrinter(QTextStream&);
	void visit(PrimitiveNode*);
	void visit(UnionNode*);
	void visit(DifferenceNode*);
	void visit(IntersectionNode*);
	void visit(SymmetricDifferenceNode*);
	void visit(MinkowskiNode*);
	void visit(HullNode*);
	void visit(LinearExtrudeNode*);
	void visit(BoundsNode*);
	void printOperation(Node*,QString name);
	void visit(TransformationNode*);
private:
	QTextStream& result;
};

#endif // NODEPRINTER_H
