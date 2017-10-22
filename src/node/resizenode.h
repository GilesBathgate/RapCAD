#ifndef RESIZENODE_H
#define RESIZENODE_H

#include "node.h"
#include "point.h"

class ResizeNode : public Node
{
public:
	ResizeNode();
	void accept(NodeVisitor&) override;
	void setSize(Point);
	Point getSize();
	void setAutoSize(bool);
	bool getAutoSize();
private:
	Point size;
	bool autosize;
};

#endif // RESIZENODE_H
