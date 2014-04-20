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

#ifndef NODEEVALUATOR_H
#define NODEEVALUATOR_H

#include <QString>
#include "primitive.h"
#include "nodevisitor.h"
#include "reporter.h"
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
#include "node/hullnode.h"
#include "node/linearextrudenode.h"
#include "node/rotateextrudenode.h"
#include "node/boundsnode.h"
#include "node/subdivisionnode.h"
#include "node/offsetnode.h"
#include "node/outlinenode.h"
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

class NodeEvaluator : public NodeVisitor
{
	Q_DECLARE_TR_FUNCTIONS(NodeEvaluator)
public:
	enum Operation_e {
		Group,
		Union,
		Difference,
		Intersection,
		SymmetricDifference,
		Minkowski
	};

	NodeEvaluator(Reporter*);

	void visit(PrimitiveNode*);
	void visit(PolylineNode*);
	void visit(UnionNode*);
	void visit(GroupNode*);
	void visit(DifferenceNode*);
	void visit(IntersectionNode*);
	void visit(SymmetricDifferenceNode*);
	void visit(MinkowskiNode*);
	void visit(GlideNode*);
	void visit(HullNode*);
	void visit(LinearExtrudeNode*);
	void visit(RotateExtrudeNode*);
	void visit(BoundsNode*);
	void visit(SubDivisionNode*);
	void visit(OffsetNode*);
	void visit(OutlineNode*);
	void visit(ImportNode*);
	void visit(TransformationNode*);
	void visit(ResizeNode*);
	void visit(AlignNode*);
	void visit(PointNode*);
	void visit(SliceNode*);
	void visit(ProductNode*);
	void visit(ProjectionNode*);
	void visit(DecomposeNode*);
	void visit(ComplementNode*);
	void visit(RadialsNode*);
	void visit(VolumesNode*);

	Primitive* getResult() const;
private:
	Primitive* createPrimitive();
	void convert(Primitive* pr, Primitive* cp);
	void evaluate(Node*,Operation_e);
	Reporter* reporter;
	Primitive* result;
};

#endif // NODEEVALUATOR_H
