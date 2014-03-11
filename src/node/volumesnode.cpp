#include "volumesnode.h"

VolumesNode::VolumesNode()
{
}

void VolumesNode::accept(NodeVisitor& v)
{
	v.visit(this);
}
