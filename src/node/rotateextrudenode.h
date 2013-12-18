#ifndef ROTATEEXTRUDENODE_H
#define ROTATEEXTRUDENODE_H

#include "node.h"
#include "fragment.h"

class RotateExtrudeNode : public Node
{
public:
	RotateExtrudeNode();
	void setRadius(double);
	double getRadius() const;
	void setFragments(Fragment);
	Fragment getFragments();
	void accept(NodeVisitor&);
private:
	double radius;
	Fragment fragments;
};

#endif // ROTATEEXTRUDENODE_H
