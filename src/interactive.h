#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include <QString>
#include <QTextStream>
#include "strategy.h"
#include "reporter.h"

class Interactive : public Strategy
{
public:
	Interactive(QTextStream&);
	void execCommand(QString);
	int evaluate();
};

#endif // INTERACTIVE_H
