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
	if(!s.contains(';')) {
		s=QString("write(%1);").arg(s);
		/* Use a kludge factor so that the reporter doesn't include the 'write('
		 * characters in its 'at character' output */
		reporter->setKludge(-6);
	}
	Script* sc=parse(s,reporter,false);
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
