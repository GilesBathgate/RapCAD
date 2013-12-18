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

void RotateExtrudeNode::setFragments(Fragment f)
{
	fragments=f;
}

Fragment RotateExtrudeNode::getFragments()
{
	return fragments;
}

void RotateExtrudeNode::accept(NodeVisitor& v)
{
	v.visit(this);
}
