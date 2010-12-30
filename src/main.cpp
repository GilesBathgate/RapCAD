/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010  Giles Bathgate
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
extern int parse(QString,bool);

int main(int argc, char *argv[])
{

	QApplication a(argc, argv);

	//Use QApplication::arguments for now,
	//if we need something more powerful we
	//can use getopt
	QStringList args = QApplication::arguments();

	if(args.length()>1) {
		if(args.at(1)=="-f") {
			QString filename = args.at(2);
			parse(filename,true);
		}
		if(args.at(1)=="-l") {
			QString text = args.at(2);
			parse(text,false);
		}
	} else {
		MainWindow w;
		w.show();

		return a.exec();
	}
}
