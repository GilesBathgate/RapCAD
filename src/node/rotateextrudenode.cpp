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

void RotateExtrudeNode::setFragments(int f)
{
	fragments=f;
}

int RotateExtrudeNode::getFragments()
{
	return fragments;
}

void RotateExtrudeNode::accept(NodeVisitor& v)
{
	v.visit(this);
}
