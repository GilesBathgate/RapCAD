#ifndef OFFSETNODE_H
#define OFFSETNODE_H

#include "node.h"

class OffsetNode : public Node
{
public:
	OffsetNode();
	void setAmount(double);
	double getAmount() const;
	void accept(NodeVisitor&);
private:
	double amount;
};

#endif // OFFSETNODE_H
