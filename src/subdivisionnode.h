#ifndef SUBDIVISIONNODE_H
#define SUBDIVISIONNODE_H

#include "node.h"

class SubDivisionNode : public Node
{
public:
	SubDivisionNode();
	void setLevel(int);
	int getLevel() const;
	void accept(NodeVisitor&);
private:
	int level;
};

#endif // SUBDIVISIONNODE_H
