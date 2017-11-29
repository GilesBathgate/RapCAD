#include "resizenode.h"

ResizeNode::ResizeNode() : size(0,0,0)
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

void ResizeNode::setAutoSize(bool b)
{
	autosize=b;
}

bool ResizeNode::getAutoSize()
{
	return autosize;
}

void ResizeNode::accept(NodeVisitor& v)
{
	v.visit(this);
}
