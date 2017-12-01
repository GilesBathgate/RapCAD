#include "subdivisionnode.h"

SubDivisionNode::SubDivisionNode()
{
	level=1;
}

void SubDivisionNode::setLevel(int l)
{
	level=l;
}

int SubDivisionNode::getLevel() const
{
	return level;
}
void SubDivisionNode::accept(NodeVisitor& v)
{
	v.visit(this);
}
