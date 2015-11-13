#include "rotateextrudenode.h"

RotateExtrudeNode::RotateExtrudeNode()
{
}

RotateExtrudeNode::~RotateExtrudeNode()
{
	delete fragments;
}

void RotateExtrudeNode::setRadius(decimal r)
{
	radius=r;
}

decimal RotateExtrudeNode::getRadius() const
{
	return radius;
}

void RotateExtrudeNode::setFragments(Fragment* f)
{
	fragments=f;
}

Fragment* RotateExtrudeNode::getFragments()
{
	return fragments;
}

void RotateExtrudeNode::accept(NodeVisitor& v)
{
	v.visit(this);
}

Point RotateExtrudeNode::getAxis() const
{
	return axis;
}

void RotateExtrudeNode::setAxis(const Point& value)
{
	axis = value;
}

