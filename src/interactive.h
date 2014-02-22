#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include <QString>
#include <QTextStream>
#include "reporter.h"

class Interactive
{
public:
	Interactive(QTextStream&);
	void execCommand(QString);
	int commandLoop();
private:
	QTextStream& output;
};

#endif // INTERACTIVE_H
