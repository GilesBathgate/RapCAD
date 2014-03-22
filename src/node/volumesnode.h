#ifndef VOLUMESNODE_H
#define VOLUMESNODE_H

#include "node.h"

class VolumesNode : public Node
{
public:
	VolumesNode();
	void accept(NodeVisitor&);
	bool getCalcMass() const;
	void setCalcMass(bool value);
private:
	bool calcMass;
};

#endif // VOLUMESNODE_H
