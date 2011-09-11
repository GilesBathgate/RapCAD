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
#include "worker.h"
#include "getopt.h"
#include "preferences.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
static void version(QTextStream& output)
{
	output << "RapCAD version: " << TOSTRING(RAPCAD_VERSION) << "\n";
	output.flush();
	exit(1);
}

static int showUi(int argc, char* argv[],QString filename)
{
	QCoreApplication::setOrganizationName("rapcad");
	QCoreApplication::setOrganizationDomain("rapcad.org");
	QCoreApplication::setApplicationName("RapCAD");
	QCoreApplication::setApplicationVersion(TOSTRING(RAPCAD_VERSION));

	QApplication a(argc, argv);
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
	QString outputFile;
	bool print=false;
	bool useGUI=true;
	QTextStream out(stdout);

	while((opt = getopt(argc, argv, "o:p::v")) != -1) {
		switch(opt) {
		case 'o':
			useGUI=false;
			outputFile=QString(optarg);
			break;
		case 'p':
			print=true;
			break;
		case 'v':
			version(out);
			break;
		}
	}

	QString inputFile;
	inputFile=QString(argv[optind]);

	if(!useGUI) {
		Worker b(out);
		b.setup(inputFile,outputFile,print);
		b.evaluate();
		return 0;
	} else {
		return showUi(argc,argv,inputFile);
	}
}
