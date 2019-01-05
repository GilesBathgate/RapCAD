/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2019 Giles Bathgate
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

Interactive::Interactive(Reporter& r,QObject* parent) :
	QObject(parent),
	Strategy(r)
{
}

bool Interactive::isExpression(const QString& s)
{
	TokenBuilder t(s);
	int i;
	while((i=t.nextToken())) {
		if(i==';'||i=='}') {
			return false;
		}
	}
	return true;
}

void Interactive::execCommand(const QString& str)
{
	QString s(str);
	if(isExpression(s)) {
		s=QString("writeln(%1);").arg(s);
		/* Use a kludge factor so that the reporter doesn't include the 'write('
		 * characters in its 'at character' output */
		reporter.setKludge(-8);
	} else {
		reporter.setKludge(0);
	}

	Script sc(reporter);
	sc.parse(s);
	TreeEvaluator e(reporter);
	sc.accept(e);
	output.flush();
}

#define PROMPT "\u042F: "

QString Interactive::getPrompt() const
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
	} while(c!=nullptr);
	output << endl;
#endif
	return EXIT_SUCCESS;
}
