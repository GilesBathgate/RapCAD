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
	QString getName();
	void setArguments(QVector<Argument*>);
	QVector<Argument*> getArguments();
	void accept(Visitor&);
private:
	QString name;
	QVector<Argument*> arguments;
};

#endif // INVOCATION_H
