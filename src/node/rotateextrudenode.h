#ifndef ROTATEEXTRUDENODE_H
#define ROTATEEXTRUDENODE_H

#include "node.h"

class RotateExtrudeNode : public Node
{
public:
	RotateExtrudeNode();
	void setRadius(double);
	double getRadius() const;
	void setFragments(int);
	int getFragments();
	void accept(NodeVisitor&);
private:
	double radius;
	int fragments;
};

#endif // ROTATEEXTRUDENODE_H
