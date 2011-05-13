#include "insetnode.h"

InsetNode::InsetNode()
{
}

void InsetNode::accept(NodeVisitor& v)
{
	v.visit(this);
}
