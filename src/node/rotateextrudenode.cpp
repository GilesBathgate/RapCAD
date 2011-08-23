#include "rotateextrudenode.h"

RotateExtrudeNode::RotateExtrudeNode()
{
}

void RotateExtrudeNode::setRadius(double r)
{
	radius=r;
}

double RotateExtrudeNode::getRadius() const
{
	return radius;
}

void RotateExtrudeNode::accept(NodeVisitor& v)
{
	v.visit(this);
}
