#ifndef INSETNODE_H
#define INSETNODE_H

#include "node.h"

class InsetNode : public Node
{
public:
	InsetNode();
	void accept(NodeVisitor&);
};

#endif // INSETNODE_H
