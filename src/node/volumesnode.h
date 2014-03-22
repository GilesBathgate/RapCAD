#ifndef VOLUMESNODE_H
#define VOLUMESNODE_H

#include "auxilarynode.h"

class VolumesNode : public AuxilaryNode
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
