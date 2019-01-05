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

#include "comparer.h"
#include "script.h"
#include "node/symmetricdifferencenode.h"
#include "node/importnode.h"
#include "nodeevaluator.h"

Comparer::Comparer(Reporter& r) : Strategy(r)
{
}

void Comparer::setup(const QString& a, const QString& b)
{
	aFile=a;
	bFile=b;
}

int Comparer::evaluate()
{
	reporter.startTiming();

	ImportNode* a=new ImportNode(aFile);
	ImportNode* b=new ImportNode(bFile);

	QList<Node*> children;
	children.append(a);
	children.append(b);

	auto* d=new SymmetricDifferenceNode();
	d->setChildren(children);

	NodeEvaluator ne(reporter);
	d->accept(ne);
	delete d;

	Primitive* p=ne.getResult();
	if(p) {
		if(p->isEmpty()) {
			reporter.reportMessage(tr("The objects are identical."));
			reporter.setReturnCode(EXIT_SUCCESS);
		} else {
			reporter.reportMessage(tr("The objects are different."));
			reporter.setReturnCode(EXIT_FAILURE);
		}
	} else {
		reporter.reportMessage(tr("The objects are both empty."));
		reporter.setReturnCode(EXIT_SUCCESS);
	}

	delete p;
	reporter.reportTiming(tr("comparison"));

	return reporter.getReturnCode();
}
