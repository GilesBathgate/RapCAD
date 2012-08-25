/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2012 Giles Bathgate
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

#include <QTime>
#include "worker.h"
#include "script.h"
#include "treeprinter.h"
#include "treeevaluator.h"
#include "nodeprinter.h"
#include "nodeevaluator.h"

#include "CGAL/exceptions.h"

extern Script* parse(QString,Reporter*);

Worker::Worker(QTextStream& s, QObject* parent) :
	QObject(parent),
	output(s)
{
	reporter=new Reporter(output);
}

Worker::~Worker()
{
	delete reporter;
}

void Worker::setup(QString i,QString o,bool p)
{
	inputFile=i;
	outputFile=o;
	print=p;
}

void Worker::evaluate()
{
	doWork();
}

void Worker::doWork()
{
	QTime* t = new QTime();
	t->start();

	Script* s=parse(inputFile,reporter);

	if(print) {
		TreePrinter p(output);
		s->accept(p);
		output.flush();
	}

	TreeEvaluator e(output);
	s->accept(e);
	delete s;
	output.flush();

	Node* n = e.getRootNode();
	if(print) {
		NodePrinter p(output);
		n->accept(p);
		output.flush();
	}

	NodeEvaluator ne(output);
	try {
		n->accept(ne);
		delete n;
	} catch(CGAL::Assertion_exception e) {
		output << "What: " << QString::fromStdString(e.what()) << "\n";
	}

	CGALPrimitive* result=ne.getResult();
	if(!result)
		output << "Warning: No top level object.\n";
	else if(!outputFile.isEmpty()) {
		CGALExport exporter(result);
		exporter.exportResult(outputFile);
	}

	int ticks=t->elapsed();
	int ms=ticks%1000;
	int secs=ticks/1000;
	int mins=secs/60;
	output << QString("Total rendering time: %1m %2s %3ms.\n").arg(mins).arg(secs).arg(ms);
	output.flush();
	delete t; //Need to delete t before finish() call.

	emit done(result);

	finish();
}

void Worker::finish()
{
}

void Worker::exportResult(CGALPrimitive* primitive, QString fn)
{
	CGALExport exporter(primitive);
	exporter.exportResult(fn);
}
