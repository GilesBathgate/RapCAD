#ifndef INVOCATION_H
#define INVOCATION_H

#include <QString>
#include <QList>
#include "expression.h"
#include "argument.h"

class Invocation : public Expression
{
public:
	Invocation();
	~Invocation();

	void setName(QString);
	QString getName() const;
	void setNamespace(QString);
	QString getNamespace() const;
	void setArguments(QList<Argument*>);
	QList<Argument*> getArguments() const;
	void accept(TreeVisitor&);
private:
	QString name;
	QString nameSpace;
	QList<Argument*> arguments;
};

#endif // INVOCATION_H
