#include "volumesnode.h"

VolumesNode::VolumesNode()
{
	calcMass=false;
}

void VolumesNode::accept(NodeVisitor& v)
{
	v.visit(this);
}

bool VolumesNode::getCalcMass() const
{
    return calcMass;
}

void VolumesNode::setCalcMass(bool value)
{
    calcMass = value;
}
