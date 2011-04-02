#ifndef OPERATIONNODE_H
#define OPERATIONNODE_H

#include <QString>
#include <QVector>
#include "abstractnode.h"

class OperationNode : public AbstractNode
{
public:
	OperationNode();
	void setName(QString);
	void setChildren(QVector<AbstractNode*>);
	QString toString();
private:
	QString name;
	QVector<AbstractNode*> children;
};

#endif // OPERATIONNODE_H
