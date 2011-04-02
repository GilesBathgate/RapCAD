#ifndef ABSTRACTNODE_H
#define ABSTRACTNODE_H

#include <QString>

class AbstractNode
{
public:
	AbstractNode() {};
	virtual QString toString()=0;
};

#endif // ABSTRACTNODE_H
