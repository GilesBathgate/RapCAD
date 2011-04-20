#include "operationnode.h"

OperationNode::OperationNode(const QString n) : name(n)
{
}

QString OperationNode::getName() const
{
	return name;
}

void OperationNode::accept(NodeVisitor& v)
{
	v.visit(this);
}
