#ifndef ROTATEEXTRUDENODE_H
#define ROTATEEXTRUDENODE_H

#include "node.h"
#include "fragment.h"
#include "point.h"

class RotateExtrudeNode : public Node
{
public:
	RotateExtrudeNode();
	~RotateExtrudeNode();
	void setRadius(decimal);
	decimal getRadius() const;
	void setFragments(Fragment*);
	Fragment* getFragments();
	Point getAxis() const;
	void setAxis(const Point& value);
	void accept(NodeVisitor&);
private:
	decimal radius;
	Point axis;
	Fragment* fragments;
};

#endif // ROTATEEXTRUDENODE_H
