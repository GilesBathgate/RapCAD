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

#include <QtGui/QApplication>
#include <QTextStream>
#include "mainwindow.h"
#include "script.h"
#include "treeprinter.h"
#include "solidpython.h"
#include "evaluator.h"
#include "nodeprinter.h"

extern Script* parse(QString);

void evaluate(QString path, bool print, QString format)
{
	QTextStream out(stdout);

	Script* s=parse(path);

	if(format =="solidpython") {
		SolidPython p;
		s->accept(p);
	} else if(print) {
		TreePrinter p(out);
		s->accept(p);
	}

	Evaluator e;
	s->accept(e);

	Node* n = e.getRootNode();
	NodePrinter p(out);
	n->accept(p);

	delete n;
	delete s;
}

int main(int argc, char* argv[])
{
	int opt;
	QString filename;
	bool print=false;
	QString printformat;
	bool useGUI=true;

	while((opt = getopt(argc, argv, "f:p::")) != -1) {
		switch(opt) {
		case 'f':
			useGUI=false;
			filename=QString(optarg);
			break;
		case 'p':
			print=true;
			printformat=QString(optarg);
			break;
		}
	}

	if(!useGUI) {
		evaluate(filename,print,printformat);
		return 0;
	} else {
		QApplication a(argc, argv);
		MainWindow w;
		w.show();

		return a.exec();
	}
}
