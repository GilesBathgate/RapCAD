#ifndef ROTATEEXTRUDENODE_H
#define ROTATEEXTRUDENODE_H

#include "node.h"
#include "fragment.h"
#include "point.h"

class RotateExtrudeNode : public Node
{
public:
	RotateExtrudeNode();
	~RotateExtrudeNode() override;
	decimal getRadius() const;
	void setRadius(const decimal&);
	decimal getSweep() const;
	void setSweep(const decimal&);
	decimal getHeight() const;
	void setHeight(const decimal&);
	Fragment* getFragments();
	void setFragments(Fragment*);
	Point getAxis() const;
	void setAxis(const Point&);
	void accept(NodeVisitor&) override;

private:
	decimal radius;
	decimal height;
	decimal sweep;
	Point axis;
	Fragment* fragments;
};

#endif // ROTATEEXTRUDENODE_H
