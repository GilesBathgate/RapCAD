#ifndef OFFSETNODE_H
#define OFFSETNODE_H

#include "node.h"

class OffsetNode : public Node
{
public:
	OffsetNode();
	void setAmount(decimal);
	decimal getAmount() const;
	void accept(NodeVisitor&) override;
private:
	decimal amount;
};

#endif // OFFSETNODE_H
