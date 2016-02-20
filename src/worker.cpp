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

#include "worker.h"
#include "treeprinter.h"
#include "treeevaluator.h"
#include "nodeprinter.h"
#include "nodeevaluator.h"
#include "product.h"
#include "numbervalue.h"

#if USE_CGAL
#include "CGAL/exceptions.h"
#include "cgalexport.h"
#include "cgalrenderer.h"
#include "cgalexplorer.h"
#else
#include "simplerenderer.h"
#endif

Worker::Worker(QTextStream& s) :
	Strategy(s)
{
	primitive=NULL;
	inputFile="";
	outputFile="";
	print=false;
	generate=false;
}

Worker::~Worker()
{
	delete primitive;
}

void Worker::setup(QString i,QString o,bool p,bool g)
{
	inputFile=i;
	outputFile=o;
	print=p;
	generate=g;
}

int Worker::evaluate()
{
	internal();
	return reporter->getReturnCode();
}

void Worker::internal()
{

	try {
		reporter->startTiming();

		primary();

		if(generate) {
			update();
			generation();
		}
		reporter->setReturnCode(EXIT_SUCCESS);

#if USE_CGAL
	} catch(CGAL::Failure_exception e) {
		reporter->reportException(QString::fromStdString(e.what()));
#endif
	} catch(...) {
		reporter->reportException(tr("Unknown error."));
	}

	update();

	finish();
}

void Worker::primary()
{
	Script* s=parse(inputFile,reporter,true);

	if(print) {
		TreePrinter p(output);
		s->accept(p);
		output << endl;
	}

	TreeEvaluator e(reporter);
	s->accept(e);
	delete s;
	output.flush();

	Node* n = e.getRootNode();
	if(print) {
		NodePrinter p(output);
		n->accept(p);
		output << endl;
	}

	NodeEvaluator ne(reporter);
	n->accept(ne);
	delete n;

	updatePrimitive(ne.getResult());
	if(!primitive)
		reporter->reportWarning(tr("no top level object."));
	else if(!outputFile.isEmpty()) {
		exportResult(outputFile);
	}
}

void Worker::generation()
{
	Script* s=parse("reprap.rcam",NULL,true);

	TreeEvaluator* e = new TreeEvaluator(reporter);
	decimal height=getBoundsHeight();
	QList<Argument*> args=getArgs(height);
	Callback* c = addCallback("layers",s,args);
	s->accept(*e);

	NumberValue* v = dynamic_cast<NumberValue*>(c->getResult());
	if(v) {
		reporter->reportMessage(tr("Layers: %1").arg(v->getValueString()));

		int itterations=v->toInteger();
		Instance* m=addProductInstance("manufacture",s);
		for(int i=0; i<=itterations; i++) {
			if(i>0) {
				e = new TreeEvaluator(reporter);
			}
			reporter->reportMessage(tr("Manufacturing layer: %1").arg(i));

			QList<Argument*> args=getArgs(i);
			m->setArguments(args);

			s->accept(*e);
			Node* n=e->getRootNode();

			NodeEvaluator* ne = new NodeEvaluator(reporter);
			n->accept(*ne);
			delete n;

			updatePrimitive(ne->getResult());
			delete ne;

			update();
		}
	}
	delete e;
	delete s;
}

decimal Worker::getBoundsHeight()
{
#if USE_CGAL
	CGALPrimitive* pr=dynamic_cast<CGALPrimitive*>(primitive);
	CGAL::Cuboid3 b=pr->getBounds();
	return b.zmax();
#endif
	return 1;
}

QList<Argument*> Worker::getArgs(decimal value)
{
	QList<Argument*> args;
	Argument* a=new Argument();
	Variable* var=new Variable();
	a->setVariable(var);
	Literal* lit=new Literal();
	lit->setValue(value);
	a->setExpression(lit);
	args.append(a);
	return args;
}

Instance* Worker::addProductInstance(QString name,Script* s)
{
	Instance* m = new Instance();
	m->setName(name);
	Product* r=new Product();
	r->setPrimitive(primitive);
	QList<Statement*> children;
	children.append(r);
	m->setChildren(children);
	s->addDeclaration(m);

	return m;
}

void Worker::exportResult(QString fn)
{
#if USE_CGAL
	try {

		CGALExport exporter(primitive);
		exporter.exportResult(fn);

	} catch(CGAL::Failure_exception e) {
		resultFailed(QString::fromStdString(e.what()));
	}
#endif
}

bool Worker::resultAvailable()
{
	return (primitive!=NULL);
}

void Worker::resultAccepted()
{
	reporter->reportTiming(tr("compiling"));
	delete previous;
}

void Worker::resultFailed(QString error)
{
	reporter->reportException(error);
	updatePrimitive(NULL);
}

void Worker::updatePrimitive(Primitive* pr)
{
	previous=primitive;
	primitive=pr;
}

Renderer* Worker::getRenderer()
{
#if USE_CGAL
	try {

		return new CGALRenderer(primitive);

	} catch(CGAL::Failure_exception e) {
		resultFailed(QString::fromStdString(e.what()));
		return NULL;
	}
#else
	return new SimpleRenderer(primitive);
#endif

}
