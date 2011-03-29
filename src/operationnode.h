#ifndef OPERATIONNODE_H
#define OPERATIONNODE_H

#include <QString>
#include "abstractnode.h"

class OperationNode : public AbstractNode
{
public:
	OperationNode();
	void setName(QString);
private:
	QString name;
};

#endif // OPERATIONNODE_H
