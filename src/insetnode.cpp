#include "insetnode.h"

InsetNode::InsetNode()
{
}

void InsetNode::setAmount(double a)
{
	amount=a;
}

double InsetNode::getAmount() const
{
	return amount;
}

void InsetNode::accept(NodeVisitor& v)
{
	v.visit(this);
}
