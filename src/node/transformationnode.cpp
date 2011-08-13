#include "transformationnode.h"

TransformationNode::TransformationNode()
{
}

void TransformationNode::accept(NodeVisitor& v)
{
	v.visit(this);
}
