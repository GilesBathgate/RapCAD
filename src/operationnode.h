#ifndef OPERATIONNODE_H
#define OPERATIONNODE_H

#include <QString>
#include <QVector>
#include "node.h"

class OperationNode : public Node
{
public:
	OperationNode();
	void setName(QString);
	QString getName() const;
	void accept(NodeVisitor&);
private:
	QString name;
};

#endif // OPERATIONNODE_H
