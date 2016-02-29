/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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
#include "ui/mainwindow.h"
#include "preferences.h"
#include "worker.h"
#include "tester.h"
#include "comparer.h"
#include "stringify.h"
#include "interactive.h"
#include "generator.h"

#if USE_COMMANDLINE_PARSER
#include "contrib/qcommandlineparser.h"
#else
#include "qcommandlineparser.h"
#endif

static void setupApplication()
{
#ifdef Q_OS_WIN
	QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
#endif
	QCoreApplication::setOrganizationName("rapcad");
	QCoreApplication::setOrganizationDomain("rapcad.org");
	QCoreApplication::setApplicationName("RapCAD");
	QCoreApplication::setApplicationVersion(STRINGIFY(RAPCAD_VERSION));
}

static void showVersion(QTextStream& out)
{
	out << QCoreApplication::applicationName() << " " << QCoreApplication::applicationVersion() << endl;
}

static int showUi(int argc, char* argv[],QStringList filenames)
{
	QApplication a(argc,argv);
	MainWindow w;
	w.loadFiles(filenames);
	w.show();
	return a.exec();
}

static Strategy* parseArguments(int argc,char* argv[],QStringList& inputFiles,QTextStream& output)
{
	QCoreApplication a(argc,argv);

	QCommandLineParser p;
	p.setApplicationDescription(QCoreApplication::translate("main","RapCAD the rapid prototyping IDE"));
	p.addHelpOption();
	p.addVersionOption();
	p.addPositionalArgument("filename", QCoreApplication::translate("main","File to open or process."));

	QCommandLineOption testOption(QStringList() << "t" << "test", QCoreApplication::translate("main","Run through tests in working directory."));
	p.addOption(testOption);

	QCommandLineOption compareOption(QStringList() << "c" << "compare", QCoreApplication::translate("main","Compare two files to see if they are identical."),"filename");
	p.addOption(compareOption);

	QCommandLineOption printOption(QStringList() << "p" << "print", QCoreApplication::translate("main","Print debugging output."));
	p.addOption(printOption);

	QCommandLineOption generateOption(QStringList() << "g" << "generate", QCoreApplication::translate("main","Generate documentation"));
	p.addOption(generateOption);

	QCommandLineOption outputOption(QStringList() << "o" << "output",QCoreApplication::translate("main","Create output file <filename>."),"filename");
	p.addOption(outputOption);

#ifdef USE_READLINE
	QCommandLineOption interactOption(QStringList() << "i" << "interactive",QCoreApplication::translate("main","Start an interactive session"));
	p.addOption(interactOption);
#endif
	p.process(a);

	inputFiles=p.positionalArguments();
	QString inputFile;
	if(!inputFiles.isEmpty())
		inputFile=inputFiles.at(0);

	QString outputFile;
	if(p.isSet(outputOption))
		outputFile=p.value(outputOption);
	else if(p.isSet(compareOption))
		outputFile=p.value(compareOption);

	if(p.isSet(compareOption)) {
		Comparer* c=new Comparer(output);
		c->setup(inputFile,outputFile);
		return c;
	} else if(p.isSet(testOption)) {
		showVersion(output);
		return new Tester(output);
	} else if(p.isSet(outputOption)||p.isSet(printOption)) {
		Worker* w=new Worker(output);
		bool print = p.isSet(printOption);
		w->setup(inputFile,outputFile,print,false);
		return w;
	} else if(p.isSet(generateOption)) {
		return new Generator(output);
#ifdef USE_READLINE
	} else if(p.isSet(interactOption)) {
		showVersion(output);
		return new Interactive(output);
#endif
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	setupApplication();

	//Ensure preferences have been initialised early.
	Preferences::getInstance();

	QStringList inputFiles;
	QTextStream output(stdout);
	Strategy* s=parseArguments(argc,argv,inputFiles,output);

	int retcode;
	if(s)
		retcode=s->evaluate();
	else
		retcode=showUi(argc,argv,inputFiles);

	delete s;

	//Ensure preferences are saved.
	Preferences::syncDelete();

	return retcode;
}
