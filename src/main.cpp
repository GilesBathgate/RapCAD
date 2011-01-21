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
extern int parse(QString,bool);

int main(int argc, char* argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "f:l:")) != -1)
    {
	switch(opt)
	{
	case 'f':
	    parse(QString(optarg),true);
	    break;
	case 'l':
	    parse(QString(optarg),false);
	    break;
	default:
	    {
	    QApplication a(argc, argv);
	    MainWindow w;
	    w.show();

	    return a.exec();
	    }
	}
    }
}
