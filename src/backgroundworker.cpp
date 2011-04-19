/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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
#include "backgroundworker.h"
#include "script.h"
#include "treeprinter.h"
#include "solidpython.h"
#include "evaluator.h"
#include "nodeprinter.h"
#include "nodeevaluator.h"

extern Script* parse(QString);

BackgroundWorker::BackgroundWorker(QTextStream& s,QObject* parent) :
	QObject(parent)
	, output(s)
{
}

CGAL::NefPolyhedron3 BackgroundWorker::evaluate(QString path, bool print, QString format)
{
	QTime t;
	t.start();

	Script* s=parse(path);

	if(format =="solidpython") {
		SolidPython p;
		s->accept(p);
	} else if(print) {
		TreePrinter p(output);
		s->accept(p);
	}

	Evaluator e(output);
	s->accept(e);
	delete s;

	Node* n = e.getRootNode();
	if(print) {
		NodePrinter p(output);
		n->accept(p);
	}

	NodeEvaluator ne;
	n->accept(ne);
	delete n;

	output << QString("Total rendering time: %1ms").arg(t.elapsed());;

	return ne.getResult();
}
