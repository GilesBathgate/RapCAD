#ifndef INVOCATION_H
#define INVOCATION_H

#include <QString>
#include <QVector>
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
	void setArguments(QVector<Argument*>);
	QVector<Argument*> getArguments() const;
	void accept(TreeVisitor&);
private:
	QString name;
	QString nameSpace;
	QVector<Argument*> arguments;
};

#endif // INVOCATION_H
