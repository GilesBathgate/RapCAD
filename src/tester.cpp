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
#include "nodeprinter.h"
#include "booleanvalue.h"

Tester::Tester(QTextStream& s,QObject* parent) : Worker(s,parent)
{
}

void Tester::evaluate()
{
	reporter->startTiming();

	QString null;
	QTextStream nullout(&null);
	QList<Argument*> args;
	int failcount=0;
	int testcount=0;
	QDir cur=QDir::current();
	foreach(inputFile, cur.entryList(QStringList("*.rcad"),QDir::Files)) {
		output << inputFile.leftJustified(32,'.',true);
		Script* s=parse(inputFile,NULL);

		TreeEvaluator* te = new TreeEvaluator(nullout);

		if(testFunctionExists(s)) {
			//If a test function exists check it returns true
			Callback* c = addCallback("test",s,args);
			s->accept(*te);
			BooleanValue* v = dynamic_cast<BooleanValue*>(c->getResult());
			if(v && v->isTrue()) {
				output << " Passed" << endl;
			} else {
				output << " FAILED" << endl;
				failcount++;
			}
			delete v;
		} else {
			QFile examFile(QFileInfo(inputFile).baseName() + ".exam");
			s->accept(*te);

			if(examFile.exists()) {
				QString result;
				QTextStream resultout(&result);
				NodePrinter* p = new NodePrinter(resultout);
				Node* n=te->getRootNode();
				n->accept(*p);

				examFile.open(QFile::ReadOnly);
				QTextStream examdata(&examFile);

				//Could probably be more efficient here
				//but this will do for now.
				QString exam = examdata.readAll();
				if(result==exam) {
					output << " Passed" << endl;
				} else {
					output << " FAILED" << endl;
				}
			} else {
				//Create exam file
				examFile.open(QFile::WriteOnly);
				QTextStream examout(&examFile);
				NodePrinter* p = new NodePrinter(examout);
				Node* n=te->getRootNode();
				n->accept(*p);
				examout.flush();
				examFile.close();
				output << "Created" << endl;
			}
		}
		delete te;
		delete s;
		testcount++;
	}
	output << testcount << " tests run " << failcount << " failed" << endl;
	reporter->reportTiming();
	finish();
}

bool Tester::testFunctionExists(Script* s)
{
	//return true;

	foreach(Declaration* d, s->getDeclarations()) {
		Function* func=dynamic_cast<Function*>(d);
		if(func && func->getName()=="test")
			return true;
	}

	return false;
}
