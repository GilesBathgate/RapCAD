#ifndef INSETNODE_H
#define INSETNODE_H

#include "node.h"

class InsetNode : public Node
{
public:
	InsetNode();
	void setAmount(double);
	double getAmount() const;
	void accept(NodeVisitor&);
private:
	double amount;
};

#endif // INSETNODE_H
