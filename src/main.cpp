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
#include "mainwindow.h"
#include "script.h"
#include "prettyprinter.h"
#include "evaluator.h"

extern Script* parse(QString);

void evaluate(QString path)
{
	Script* s=parse(path);
	PrettyPrinter p;
	s->accept(p);

	Evaluator e;
	s->accept(e);

	//TODO just delete the script for now
	delete s;
}

int main(int argc, char* argv[])
{
	int opt;
	while((opt = getopt(argc, argv, "f:")) != -1) {
		switch(opt) {
		case 'f':
			evaluate(QString(optarg));
			return 0;
		}
	}

	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
