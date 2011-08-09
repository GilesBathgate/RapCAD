#ifndef RESIZENODE_H
#define RESIZENODE_H

#include "node.h"
#include "point.h"

class ResizeNode : public Node
{
public:
	ResizeNode();
	void accept(NodeVisitor&);
	void setSize(Point);
	Point getSize();
private:
	Point size;
};

#endif // RESIZENODE_H
