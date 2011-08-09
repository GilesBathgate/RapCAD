#include "resizenode.h"

ResizeNode::ResizeNode()
{
}

void ResizeNode::setSize(Point p)
{
	size=p;
}

Point ResizeNode::getSize()
{
	return size;
}

void ResizeNode::accept(NodeVisitor& v)
{
	v.visit(this);
}
