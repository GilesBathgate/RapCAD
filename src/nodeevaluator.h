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

#ifndef NODEEVALUATOR_H
#define NODEEVALUATOR_H

#include <QString>
#include "primitive.h"
#include "nodevisitor.h"
#include "reporter.h"
#include "node/primitivenode.h"
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
#include "node/boundarynode.h"
#include "node/importnode.h"
#include "node/resizenode.h"
#include "node/alignnode.h"
#include "node/pointsnode.h"
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

#include "cachemanager.h"

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

	explicit NodeEvaluator(Reporter&);

	void visit(const PrimitiveNode&) override;
	void visit(const UnionNode&) override;
	void visit(const GroupNode&) override;
	void visit(const DifferenceNode&) override;
	void visit(const IntersectionNode&) override;
	void visit(const SymmetricDifferenceNode&) override;
	void visit(const MinkowskiNode&) override;
	void visit(const GlideNode&) override;
	void visit(const HullNode&) override;
	void visit(const LinearExtrudeNode&) override;
	void visit(const RotateExtrudeNode&) override;
	void visit(const BoundsNode&) override;
	void visit(const SubDivisionNode&) override;
	void visit(const OffsetNode&) override;
	void visit(const BoundaryNode&) override;
	void visit(const ImportNode&) override;
	void visit(const TransformationNode&) override;
	void visit(const ResizeNode&) override;
	void visit(const AlignNode&) override;
	void visit(const PointsNode&) override;
	void visit(const SliceNode&) override;
	void visit(const ProductNode&) override;
	void visit(const ProjectionNode&) override;
	void visit(const DecomposeNode&) override;
	void visit(const ComplementNode&) override;
	void visit(const RadialsNode&) override;
	void visit(const VolumesNode&) override;
	void visit(const TriangulateNode&) override;
	void visit(const MaterialNode&) override;
	void visit(const DiscreteNode&) override;
	void visit(const NormalsNode&) override;
	void visit(const SimplifyNode&) override;
	void visit(const ChildrenNode&) override;

	Primitive* getResult() const;
private:
	Primitive* createPrimitive();
	bool evaluate(const Node&,Operation_e);
	bool evaluate(const Node&,Operation_e,Primitive*);
	bool evaluate(const QList<Node*>&,Operation_e,Primitive*);

	Reporter& reporter;
	Primitive* result;
	Cache* cache;
};

#endif // NODEEVALUATOR_H
