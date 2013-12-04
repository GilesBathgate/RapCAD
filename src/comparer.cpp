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

#include "comparer.h"
#include "script.h"
#include "module/importmodule.h"
#include "instance.h"
#include "treeevaluator.h"
#include "nodeevaluator.h"

#include "treeprinter.h"

Comparer::Comparer(QTextStream& s) : Strategy(s)
{
}

void Comparer::setup(QString a, QString b)
{
	aFile=a;
	bFile=b;
}

void Comparer::evaluate()
{
	reporter->startTiming();
	Script* s=new Script();

	ModuleImport* a=new ModuleImport();
	a->setImport(aFile);
	a->setName("a");

	ModuleImport* b=new ModuleImport();
	b->setImport(bFile);
	b->setName("b");

	Instance* ia=new Instance();
	ia->setName("a");

	Instance* ib=new Instance();
	ib->setName("b");

	QList<Statement*> children;
	children.append(ia);
	children.append(ib);

	Instance* i=new Instance();
	i->setName("symmetric_difference");
	i->setChildren(children);

	s->addDeclaration(i);
	s->addDeclaration(a);
	s->addDeclaration(b);

	TreeEvaluator te(output);
	s->accept(te);
	delete s;

	Node* n=te.getRootNode();

	NodeEvaluator ne(output);
	n->accept(ne);
	delete n;

	Primitive* p=ne.getResult();
	if(p->isEmpty())
		output << "The objects are identical." << endl;
	else
		output << "The objects are different" << endl;

	delete p;
	reporter->reportTiming();
}
