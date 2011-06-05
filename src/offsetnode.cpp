#include "offsetnode.h"

OffsetNode::OffsetNode()
{
}

void OffsetNode::setAmount(double a)
{
	amount=a;
}

double OffsetNode::getAmount() const
{
	return amount;
}

void OffsetNode::accept(NodeVisitor& v)
{
	v.visit(this);
}
