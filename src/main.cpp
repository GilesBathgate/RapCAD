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

#include <QApplication>
#include <QTextStream>
#include "mainwindow.h"
#include "getopt.h"
#include "preferences.h"
#include "worker.h"
#include "tester.h"
#include "comparer.h"
#include "stringify.h"

static int version()
{
	QTextStream output(stdout);
	output << "RapCAD version: " << STRINGIFY(RAPCAD_VERSION) << "\n";
	output.flush();
	return 0;
}

static int showUi(QApplication& a,QString filename)
{
	QCoreApplication::setOrganizationName("rapcad");
	QCoreApplication::setOrganizationDomain("rapcad.org");
	QCoreApplication::setApplicationName("RapCAD");
	QCoreApplication::setApplicationVersion(STRINGIFY(RAPCAD_VERSION));

	MainWindow w;

	if(!filename.isEmpty())
		w.loadFile(filename);

	w.show();

	int retcode=a.exec();
	Preferences::syncDelete();
	return retcode;
}

int main(int argc, char* argv[])
{
	int opt;
	bool print=false;
	bool useGUI=true;
	bool test=false;
	bool compare=false;

	QString outputFile;
	QString inputFile;

	while((opt = getopt(argc, argv, "o:pvtc:")) != -1) {
		switch(opt) {
		case 'v':
			return version();
		case 't':
			useGUI=false;
			test=true;
			break;
		case 'c':
			useGUI=false;
			compare=true;
			outputFile=QString(optarg);
			break;
		case 'p':
			print=true;
		case 'o':
			useGUI=false;
			outputFile=QString(optarg);
		}
	}
	inputFile=QString(argv[optind]);

	if(useGUI) {
		QApplication a(argc,argv);
		return showUi(a,inputFile);
	}

	QTextStream output(stdout);
	Strategy* s;
	if(compare) {
		Comparer* c=new Comparer(output);
		c->setup(inputFile,outputFile);
		s=c;
	} else if(test) {
		s=new Tester(output);
	} else {
		Worker* w=new Worker(output);
		w->setup(inputFile,outputFile,print,false);
		s=w;
	}

	QCoreApplication a(argc,argv);
	int retcode=s->evaluate();
	delete s;
	a.quit();
	return retcode;
}
