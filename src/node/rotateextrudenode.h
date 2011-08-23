#ifndef ROTATEEXTRUDENODE_H
#define ROTATEEXTRUDENODE_H

#include "node.h"

class RotateExtrudeNode : public Node
{
public:
	RotateExtrudeNode();
	void setRadius(double);
	double getRadius() const;
	void accept(NodeVisitor&);
private:
	double radius;
};

#endif // ROTATEEXTRUDENODE_H
