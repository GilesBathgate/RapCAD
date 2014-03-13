#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include <QString>
#include <QTextStream>
#include "strategy.h"
#include "reporter.h"

class Interactive : public QObject,public Strategy
{
	Q_OBJECT
public:
	Interactive(QTextStream&,QObject* parent=0);
	QString getPrompt();
	int evaluate();
public slots:
	void execCommand(QString);
private:
	bool isExpression(QString s);
};

#endif // INTERACTIVE_H
