#ifndef VOLUMESNODE_H
#define VOLUMESNODE_H

#include "node.h"

class VolumesNode : public Node
{
public:
	VolumesNode();
	void accept(NodeVisitor&);
};

#endif // VOLUMESNODE_H
