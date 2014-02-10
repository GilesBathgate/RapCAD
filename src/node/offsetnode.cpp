#include "offsetnode.h"

OffsetNode::OffsetNode()
{
}

void OffsetNode::setAmount(decimal a)
{
	amount=a;
}

decimal OffsetNode::getAmount() const
{
	return amount;
}

void OffsetNode::accept(NodeVisitor& v)
{
	v.visit(this);
}
