#ifndef NODETREEVISUALISER_H
#define NODETREEVISUALISER_H

#include "node/alignnode.h"
#include "node/boundarynode.h"
#include "node/boundsnode.h"
#include "node/childrennode.h"
#include "node/complementnode.h"
#include "node/decomposenode.h"
#include "node/differencenode.h"
#include "node/discretenode.h"
#include "node/glidenode.h"
#include "node/groupnode.h"
#include "node/hullnode.h"
#include "node/importnode.h"
#include "node/intersectionnode.h"
#include "node/linearextrudenode.h"
#include "node/materialnode.h"
#include "node/minkowskinode.h"
#include "node/normalsnode.h"
#include "node/offsetnode.h"
#include "node/pointsnode.h"
#include "node/primitivenode.h"
#include "node/productnode.h"
#include "node/projectionnode.h"
#include "node/radialsnode.h"
#include "node/resizenode.h"
#include "node/rotateextrudenode.h"
#include "node/simplifynode.h"
#include "node/slicenode.h"
#include "node/solidnode.h"
#include "node/subdivisionnode.h"
#include "node/symmetricdifferencenode.h"
#include "node/transformationnode.h"
#include "node/triangulatenode.h"
#include "node/unionnode.h"
#include "node/volumesnode.h"
#include "nodevisitor.h"
#include <QStandardItemModel>
#include <QStack>

class NodeTreeVisualiser : public QStandardItemModel, public NodeVisitor
{
	Q_OBJECT
public:
	explicit NodeTreeVisualiser(QObject* parent=nullptr);
	~NodeTreeVisualiser() override;
	void reset();
	void visit(const AlignNode&) override;
	void visit(const BoundaryNode&) override;
	void visit(const BoundsNode&) override;
	void visit(const ChildrenNode&) override;
	void visit(const ComplementNode&) override;
	void visit(const DecomposeNode&) override;
	void visit(const DifferenceNode&) override;
	void visit(const DiscreteNode&) override;
	void visit(const GlideNode&) override;
	void visit(const GroupNode&) override;
	void visit(const HullNode&) override;
	void visit(const ImportNode&) override;
	void visit(const IntersectionNode&) override;
	void visit(const LinearExtrudeNode&) override;
	void visit(const MaterialNode&) override;
	void visit(const MinkowskiNode&) override;
	void visit(const NormalsNode&) override;
	void visit(const OffsetNode&) override;
	void visit(const PointsNode&) override;
	void visit(const PrimitiveNode&) override;
	void visit(const ProductNode&) override;
	void visit(const ProjectionNode&) override;
	void visit(const RadialsNode&) override;
	void visit(const ResizeNode&) override;
	void visit(const RotateExtrudeNode&) override;
	void visit(const SimplifyNode&) override;
	void visit(const SliceNode&) override;
	void visit(const SolidNode&) override;
	void visit(const SubDivisionNode&) override;
	void visit(const SymmetricDifferenceNode&) override;
	void visit(const TransformationNode&) override;
	void visit(const TriangulateNode&) override;
	void visit(const UnionNode&) override;
	void visit(const VolumesNode&) override;
	Primitive* getResult() const override { return nullptr; }
signals:
	void expandNodes();
private:
	QStack<QStandardItem*> parentStack;
	void addNode(const Node&);
};

#endif // NODETREEVISUALISER_H
