/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2013 Giles Bathgate
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

#include "tester.h"
#include "treeevaluator.h"
#include "booleanvalue.h"

Tester::Tester(QTextStream& s,QObject* parent) : Worker(s,parent)
{
}

void Tester::evaluate()
{
	QDir cur=QDir::current();
	foreach(inputFile, cur.entryList(QStringList("*.rcad"),QDir::Files)) {
		output << inputFile << "...";
		Script* s=parse(inputFile,NULL);
		TreeEvaluator* e = new TreeEvaluator(output);
		QList<Argument*> args;
		Callback* c = addCallback("test",s,args);
		s->accept(*e);
		BooleanValue* v = dynamic_cast<BooleanValue*>(c->getResult());
		if(v && v->isTrue()) {
			output << " PASSED\n";
		} else {
			output << " FAILED\n";
		}
		output.flush();
		delete v;
		delete e;
		delete s;
	}
	finish();
}
