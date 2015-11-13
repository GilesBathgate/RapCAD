#include "transformationnode.h"

TransformationNode::TransformationNode()
{
	matrix=NULL;
}

void TransformationNode::accept(NodeVisitor& v)
{
	v.visit(this);
}

TransformMatrix* TransformationNode::getMatrix() const
{
	return matrix;
}

void TransformationNode::setMatrix(TransformMatrix* value)
{
	matrix = value;
}
