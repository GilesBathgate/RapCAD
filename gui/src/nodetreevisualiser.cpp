#include "nodetreevisualiser.h"

NodeTreeVisualiser::NodeTreeVisualiser(QObject* parent) : QStandardItemModel(parent)
{
	reset();
}

NodeTreeVisualiser::~NodeTreeVisualiser()
{
}

void NodeTreeVisualiser::reset()
{
	const QStringList headers(tr("Name"));
	clear();
	setHorizontalHeaderLabels(headers);
}

void NodeTreeVisualiser::visit(const AlignNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const BoundaryNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const BoundsNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const ChildrenNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const ComplementNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const DecomposeNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const DifferenceNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const DiscreteNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const GlideNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const GroupNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const HullNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const ImportNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const IntersectionNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const LinearExtrudeNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const MaterialNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const MinkowskiNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const NormalsNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const OffsetNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const PointsNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const PrimitiveNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const ProductNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const ProjectionNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const RadialsNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const ResizeNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const RotateExtrudeNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const SimplifyNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const SliceNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const SolidNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const SubDivisionNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const TaperNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const SymmetricDifferenceNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const TransformationNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const TriangulateNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const UnionNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::visit(const VolumesNode& n)
{
	addNode(n);
}

void NodeTreeVisualiser::addNode(const Node& node)
{
	const auto& name=node.getModuleName();
	auto* item=new QStandardItem(name);
	if(parentStack.isEmpty()) {
		appendRow(item);
	} else {
		parentStack.top()->appendRow(item);
	}

	parentStack.push(item);
	for(auto* child : node.getChildren())
		child->accept(*this);

	parentStack.pop();
	if(parentStack.isEmpty())
		emit expandNodes();
}
