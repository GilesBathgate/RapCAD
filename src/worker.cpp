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

#include "worker.h"
#include "treeprinter.h"
#include "treeevaluator.h"
#include "nodeprinter.h"
#include "nodeevaluator.h"
#include "product.h"
#include "numbervalue.h"

#ifdef USE_CGAL
#include "CGAL/exceptions.h"
#include "cgalexport.h"
#include "cgalrenderer.h"
#include "cgalexplorer.h"
#else
#include "simplerenderer.h"
#endif

Worker::Worker(Reporter& r) :
	Strategy(r),
	inputFile(""),
	outputFile(""),
	print(false),
	generate(false),
	primitive(nullptr),
	previous(nullptr)
{
}

Worker::~Worker()
{
	delete primitive;
}

void Worker::setup(const QString& i,const QString& o,bool p,bool g)
{
	inputFile=i;
	outputFile=o;
	print=p;
	generate=g;
}

int Worker::evaluate()
{
	internal();
	return reporter.getReturnCode();
}

void Worker::internal()
{

	try {
		reporter.startTiming();

		primary();

		if(generate) {
			update();
			generation();
		}
		reporter.setReturnCode(EXIT_SUCCESS);

#ifdef USE_CGAL
	} catch(const CGAL::Failure_exception& e) {
		resultFailed(QString::fromStdString(e.what()));
#endif
	} catch(...) {
		resultFailed(tr("Unknown error."));
	}

	update();

	finish();
}

void Worker::primary()
{
	Script s(reporter);
	s.parse(inputFile);

	if(print) {
		TreePrinter p(output);
		s.accept(p);
		output << endl;
	}

	TreeEvaluator e(reporter);
	s.accept(e);
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
		reporter.reportWarning(tr("no top level object."));
	else if(!outputFile.isEmpty()) {
		exportResult(outputFile);
	}
}

void Worker::generation()
{
	Script s(reporter);
	s.parse(QFileInfo("reprap.rcam"));

	auto* e = new TreeEvaluator(reporter);
	decimal height=getBoundsHeight();
	QList<Argument*> args=getArgs(height);
	Callback* c = addCallback("layers",s,args);
	s.accept(*e);

	auto* v = dynamic_cast<NumberValue*>(c->getResult());
	if(v) {
		reporter.reportMessage(tr("Layers: %1").arg(v->getValueString()));

		int itterations=v->toInteger();
		Instance* m=addProductInstance("manufacture",s);
		for(auto i=0; i<=itterations; ++i) {
			if(i>0) {
				e = new TreeEvaluator(reporter);
			}
			reporter.reportMessage(tr("Manufacturing layer: %1").arg(i));

			QList<Argument*> args=getArgs(i);
			m->setArguments(args);

			s.accept(*e);
			Node* n=e->getRootNode();

			auto* ne = new NodeEvaluator(reporter);
			n->accept(*ne);
			delete n;

			updatePrimitive(ne->getResult());
			delete ne;

			update();
		}
	}
	delete e;
}

decimal Worker::getBoundsHeight() const
{
#ifdef USE_CGAL
	auto* pr=dynamic_cast<CGALPrimitive*>(primitive);
	if(!pr) return 1;
	CGAL::Cuboid3 b=pr->getBounds();
	return b.zmax();
#else
	return 1;
#endif
}

QList<Argument*> Worker::getArgs(decimal value)
{
	QList<Argument*> args;
	auto* a=new Argument();
	auto* var=new Variable();
	a->setVariable(var);
	auto* lit=new Literal();
	lit->setValue(value);
	a->setExpression(lit);
	args.append(a);
	return args;
}

Instance* Worker::addProductInstance(const QString& name,Script& s)
{
	auto* m = new Instance();
	m->setName(name);
	auto* r=new Product();
	r->setPrimitive(primitive);
	QList<Statement*> children;
	children.append(r);
	m->setChildren(children);
	s.addDeclaration(m);

	return m;
}

void Worker::exportResult(const QString& fn)
{
#ifdef USE_CGAL
	reporter.startTiming();

	try {
		CGALExport exporter(primitive,reporter);
		exporter.exportResult(fn);
	} catch(const CGAL::Failure_exception& e) {
		resultFailed(QString::fromStdString(e.what()));
	}

	reporter.reportTiming(tr("export"));
#endif
}

bool Worker::resultAvailable()
{
	return (primitive!=nullptr);
}

void Worker::resultAccepted()
{
	reporter.reportTiming(tr("compiling"));
	delete previous;
}

void Worker::resultFailed(const QString& error)
{
	reporter.reportException(error);
	updatePrimitive(nullptr);
}

void Worker::updatePrimitive(Primitive* pr)
{
	previous=primitive;
	primitive=pr;
}

Renderer* Worker::getRenderer()
{
#ifdef USE_CGAL
	try {

		return new CGALRenderer(primitive);

	} catch(const CGAL::Failure_exception& e) {
		resultFailed(QString::fromStdString(e.what()));
		return nullptr;
	}
#else
	return new SimpleRenderer(primitive);
#endif

}
