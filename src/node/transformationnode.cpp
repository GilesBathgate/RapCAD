#include "transformationnode.h"

TransformationNode::TransformationNode()
{
	for(int i=0; i<16; i++)
		matrix[i]=0.0;

	matrix[15]=1.0;
}

void TransformationNode::accept(NodeVisitor& v)
{
	v.visit(this);
}
