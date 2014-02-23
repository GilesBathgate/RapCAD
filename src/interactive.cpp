#include "interactive.h"
#include "treeevaluator.h"
#include "script.h"

#ifdef USE_READLINE
namespace readline
{
#include "readline/readline.h"
}
#endif

Interactive::Interactive(QTextStream& s,QObject* parent) : QObject(parent),Strategy(s)
{
}

void Interactive::execCommand(QString s)
{
	Script* sc=parse(QString("write(%1);").arg(s),NULL,false);
	TreeEvaluator e(output);
	sc->accept(e);
	output << endl;
	delete sc;
}

#define PROMPT "\u042F: "

QString Interactive::getPrompt()
{
	return PROMPT;
}

int Interactive::evaluate()
{
#ifdef USE_READLINE
	const char* prompt=PROMPT;
	char* c;
	do {
		c=readline::readline(prompt);
		execCommand(c);
	} while(c!=NULL);
	output << endl;
	return 0;
#endif
}
