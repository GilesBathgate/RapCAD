#include "interactive.h"
#include "treeevaluator.h"
#include "script.h"
#include "tokenbuilder.h"

#ifdef USE_READLINE
namespace readline
{
#include "readline/readline.h"
}
#endif

Interactive::Interactive(QTextStream& s,QObject* parent) : QObject(parent),Strategy(s)
{
}

bool Interactive::isExpression(QString s)
{
	TokenBuilder t(NULL,s,false);
	int i;
	while((i=t.nextToken())) {
		if(i==';'||i=='}') {
			return false;
		}
	}
	return true;
}

void Interactive::execCommand(QString s)
{
	if(isExpression(s)) {
		s=QString("writeln(%1);").arg(s);
		/* Use a kludge factor so that the reporter doesn't include the 'write('
		 * characters in its 'at character' output */
		reporter->setKludge(-8);
	} else {
		reporter->setKludge(0);
	}

	Script* sc=parse(s,reporter,false);
	TreeEvaluator e(reporter);
	sc->accept(e);
	output.flush();
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
#endif
	return EXIT_SUCCESS;
}
