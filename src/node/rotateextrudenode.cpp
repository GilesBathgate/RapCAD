#include "rotateextrudenode.h"

RotateExtrudeNode::RotateExtrudeNode() : axis(0,0,0)
{
}

RotateExtrudeNode::~RotateExtrudeNode()
{
	delete fragments;
}

decimal RotateExtrudeNode::getRadius() const
{
	return radius;
}

void RotateExtrudeNode::setRadius(const decimal& r)
{
	radius=r;
}

decimal RotateExtrudeNode::getSweep() const
{
	return sweep;
}

void RotateExtrudeNode::setSweep(const decimal& value)
{
	sweep = value;
}

decimal RotateExtrudeNode::getHeight() const
{
	return height;
}

void RotateExtrudeNode::setHeight(const decimal& value)
{
	height = value;
}

Point RotateExtrudeNode::getAxis() const
{
	return axis;
}

void RotateExtrudeNode::setAxis(const Point& value)
{
	axis = value;
}

Fragment* RotateExtrudeNode::getFragments()
{
	return fragments;
}

void RotateExtrudeNode::setFragments(Fragment* f)
{
	fragments=f;
}

void RotateExtrudeNode::accept(NodeVisitor& v)
{
	v.visit(this);
}
