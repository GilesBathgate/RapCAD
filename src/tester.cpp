/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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

#include <QDir>
#include "tester.h"
#include "treeevaluator.h"
#include "nodeprinter.h"
#include "booleanvalue.h"
#include "comparer.h"
#include "cachemanager.h"

Tester::Tester(QTextStream& s) : Strategy(s)
{
}

int Tester::evaluate()
{
	reporter->startTiming();

	CacheManager* cm=CacheManager::getInstance();
	cm->disableCaches();

	QString null;
	QTextStream nullout(&null);
	Reporter nullreport(nullout);
	QList<Argument*> args;
	int failcount=0;
	int testcount=0;

	/* This hard coded directory and filters need to be addressed
	 * but it will do for now. */
	QDir cur=QDir::current();
	foreach(QString dir, cur.entryList(QStringList("*_*")))
	foreach(QFileInfo file, QDir(dir).entryInfoList(QStringList("*.rcad"),QDir::Files)) {

		output << "Test #" << QString().setNum(testcount+1).rightJustified(3,'0') << ": ";
		output << file.fileName().leftJustified(62,'.',true);
		output.flush();

		Script* s=parse(file.absoluteFilePath(),NULL,true);

		TreeEvaluator te(&nullreport);

		if(testFunctionExists(s)) {
			//If a test function exists check it returns true
			Callback* c = addCallback("test",s,args);
			s->accept(te);
			BooleanValue* v = dynamic_cast<BooleanValue*>(c->getResult());
			if(v && v->isTrue()) {
				output << " Passed" << endl;
			} else {
				output << " FAILED" << endl;
				failcount++;
			}
			delete v;

			Node* n=te.getRootNode();
			delete n;
		} else {
			QString basename=file.baseName();
			QString examFileName=basename + ".exam.csg";
			QString csgFileName=basename + ".csg";
			QFileInfo examFileInfo(file.absoluteDir(),examFileName);
			QFileInfo csgFileInfo(file.absoluteDir(),csgFileName);
			QFile examFile(examFileInfo.absoluteFilePath());
			s->accept(te);

			//Create exam file
			examFile.open(QFile::WriteOnly);
			QTextStream examout(&examFile);
			NodePrinter p(examout);
			Node* n=te.getRootNode();
			n->accept(p);
			delete n;
			examout.flush();
			examFile.close();

			QFile csgFile(csgFileInfo.absoluteFilePath());
			if(csgFile.exists()) {
				Comparer co(nullout);
				co.setup(examFileInfo.absoluteFilePath(),csgFileInfo.absoluteFilePath());
				if(co.evaluate()==0) {
					output << " Passed" << endl;
				} else {
					output << " FAILED" << endl;
					failcount++;
				}
				examFile.remove();
			} else {
				output << "Created" << endl;
			}

		}
		delete s;
		testcount++;
	}

	reporter->setReturnCode(failcount);

	output << testcount << " tests run " << failcount << " failed" << endl;

	reporter->reportTiming("testing");

	return reporter->getReturnCode();
}

bool Tester::testFunctionExists(Script* s)
{
	foreach(Declaration* d, s->getDeclarations()) {
		Function* func=dynamic_cast<Function*>(d);
		if(func && func->getName()=="test")
			return true;
	}

	return false;
}
