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
#endif

extern Script* parse(QString,Reporter*);

Worker::Worker(QTextStream& s, QObject* parent) :
	QObject(parent),
	output(s)
{
	primitive=NULL;
	render=NULL;
	inputFile="";
	outputFile="";
	print=false;
	generate=false;
	reporter=new Reporter(output);
}

Worker::~Worker()
{
	delete reporter;
}

void Worker::setup(QString i,QString o,bool p,bool g)
{
	inputFile=i;
	outputFile=o;
	print=p;
	generate=g;
}

void Worker::evaluate()
{
	evaluateInternal();
}

void Worker::evaluateInternal()
{

	try {
		reporter->startTiming();

		primary();

		reporter->reportTiming();

		emit done();

		if(generate)
			generation();

#if USE_CGAL
	} catch(CGAL::Assertion_exception e) {
		output << "What: " << QString::fromStdString(e.what()) << "\n";
		output.flush();
		emit done();
	} catch(...) {
		emit done();
	}
#else
	} catch(...) {
		emit done();
	}
#endif

	finish();
}

void Worker::primary()
{
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

	n->accept(ne);
	delete n;


	primitive=ne.getResult();
	if(!primitive)
		output << "Warning: No top level object.\n";
	else if(!outputFile.isEmpty()) {
		exportResult(outputFile);
	}
}

void Worker::generation()
{
	Script* s=parse("reprap.rcam",NULL);

	TreeEvaluator* e = new TreeEvaluator(output);
	Callback* c = addCallback(s);
	s->accept(*e);

	NumberValue* v = dynamic_cast<NumberValue*>(c->getResult());
	if(v) {
		output << "Layers: " << v->getValueString() << "\n";
		output.flush();

		double itterations=v->getNumber();
		Instance* m=addProductInstance(s);
		for(int i=0; i<=itterations; i++) {
			if(i>0) {
				e = new TreeEvaluator(output);
			}
			output << "Manufacturing layer: " << i << "\n";
			output.flush();

			QList<Argument*> args=getArgs(i);
			m->setArguments(args);

			s->accept(*e);
			Node* n=e->getRootNode();

			NodeEvaluator* ne = new NodeEvaluator(output);
			n->accept(*ne);
			primitive=ne->getResult();
			delete ne;

			emit done();
		}
	}
	delete e;
	delete s;
}

double Worker::getBoundsHeight()
{
#if USE_CGAL
	CGALExplorer explorer(primitive);
	CGAL::Bbox_3 b=explorer.getBounds();
	return b.zmax();
#endif
	return 1;
}

Callback* Worker::addCallback(Script* s)
{
	double height=getBoundsHeight();
	Callback* c=new Callback();
	Invocation* l=new Invocation();
	QList<Argument*> args=getArgs(height);
	l->setArguments(args);
	l->setName("layers");
	c->setExpression(l);
	s->addDeclaration(c);

	return c;
}

QList<Argument*> Worker::getArgs(double value)
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

Instance* Worker::addProductInstance(Script* s)
{
	Instance* m = new Instance();
	m->setName("manufacture");
	Product* r=new Product();
	r->setPrimitive(primitive);
	QList<Statement*> children;
	children.append(r);
	m->setChildren(children);
	s->addDeclaration(m);

	return m;
}

void Worker::finish()
{
}

void Worker::exportResult(QString fn)
{
#if USE_CGAL
	CGALPrimitive* p = dynamic_cast<CGALPrimitive*>(primitive);
	if(p) {
		CGALExport exporter(p);
		exporter.exportResult(fn);
	}
#endif
}

bool Worker::resultAvailable()
{
	return (primitive!=NULL);
}

Renderer* Worker::getRenderer()
{
	if(render)
		delete render;

#if USE_CGAL
	CGALPrimitive* p = dynamic_cast<CGALPrimitive*>(primitive);
	if(p)
		render=new CGALRenderer(p);
#endif
	return render;
}
