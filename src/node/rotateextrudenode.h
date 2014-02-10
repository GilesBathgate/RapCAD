#ifndef ROTATEEXTRUDENODE_H
#define ROTATEEXTRUDENODE_H

#include "node.h"
#include "fragment.h"

class RotateExtrudeNode : public Node
{
public:
	RotateExtrudeNode();
	void setRadius(decimal);
	decimal getRadius() const;
	void setFragments(Fragment);
	Fragment getFragments();
	void accept(NodeVisitor&);
private:
	decimal radius;
	Fragment fragments;
};

#endif // ROTATEEXTRUDENODE_H
