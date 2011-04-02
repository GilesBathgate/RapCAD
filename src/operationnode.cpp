#include "operationnode.h"

OperationNode::OperationNode()
{
}

void OperationNode::setName(QString n)
{
	name=n;
}

void OperationNode::setChildren(QVector<AbstractNode*> childs)
{
	children=childs;
}

QString OperationNode::toString()
{
	QString result;
	result.append(name);
	result.append("(");
	foreach(AbstractNode* n,children)
		result.append(n->toString());
	result.append(")");
	return result;
}
