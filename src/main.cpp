/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2021 Giles Bathgate
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
#include "builtincreator.h"
#include "valuefactory.h"
#include "worker.h"
#ifdef USE_INTEGTEST
#include "tester.h"
#endif
#include "comparer.h"
#include "stringify.h"
#include "interactive.h"
#include "generator.h"

#ifdef USE_COMMANDLINE_PARSER
#include "contrib/qcommandlineparser.h"
#else
#include "qcommandlineparser.h"
#endif
#include "contrib/qtcompat.h"

#ifdef USE_CGAL
#include "cgal.h"
static void rapcadErrorHandler(const char*,const char*,const char*,int,const char*){}
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
#ifdef USE_CGAL
	CGAL::set_error_handler(rapcadErrorHandler);
#endif
	//Ensure preferences have been initialised early.
	Preferences::getInstance();
}

static void showVersion(QTextStream& out)
{
	out << QCoreApplication::applicationName() << " " << QCoreApplication::applicationVersion() << Qt::endl;
}

static Strategy* parseArguments(int argc,char* argv[],QStringList& inputFiles,Reporter& reporter)
{
	QCommandLineParser p;
	p.setApplicationDescription(QCoreApplication::translate("main","RapCAD the rapid prototyping IDE"));
	p.addHelpOption();
	p.addVersionOption();
	p.addPositionalArgument("filename", QCoreApplication::translate("main","File to open or process."));

#ifdef USE_INTEGTEST
	QCommandLineOption testOption(QStringList() << "t" << "test", QCoreApplication::translate("main","Run through tests in working directory."),"directory");
	p.addOption(testOption);

	QCommandLineOption generateOption(QStringList() << "g" << "generate", QCoreApplication::translate("main","Generate documentation"));
	p.addOption(generateOption);
#endif

	QCommandLineOption compareOption(QStringList() << "c" << "compare", QCoreApplication::translate("main","Compare two files to see if they are identical."),"filename");
	p.addOption(compareOption);

	QCommandLineOption outputOption(QStringList() << "o" << "output",QCoreApplication::translate("main","Create output file <filename>."),"filename");
	p.addOption(outputOption);

#ifdef USE_READLINE
	QCommandLineOption interactOption(QStringList() << "i" << "interactive",QCoreApplication::translate("main","Start an interactive session"));
	p.addOption(interactOption);
#endif
	QCoreApplication a(argc,argv);
	p.process(a);

	inputFiles=p.positionalArguments();
	QString inputFile;
	if(!inputFiles.isEmpty()) {
		inputFile=inputFiles.at(0);
	}

	if(p.isSet(compareOption)) {
		auto* c=new Comparer(reporter);
		c->setup(inputFile,p.value(compareOption));
		return c;
	}
#ifdef USE_INTEGTEST
	if(p.isSet(testOption)) {
		showVersion(reporter.output);
		return new Tester(reporter,p.value(testOption));
	}
	if(p.isSet(generateOption)) {
		return new Generator(reporter);
	}
#endif
	if(p.isSet(outputOption)) {
		auto* w=new Worker(reporter);
		w->setup(inputFile,p.value(outputOption),false);
		return w;
	}
#ifdef USE_READLINE
	if(p.isSet(interactOption)) {
		showVersion(reporter.output);
		return new Interactive(reporter);
	}
#endif
	return nullptr;
}

static int runApplication(Strategy* s,int argc,char* argv[],const QStringList& inputFiles)
{
	if(s)
		return s->evaluate();

	QApplication a(argc,argv);
	MainWindow w;
	w.loadFiles(inputFiles);
	w.show();
	return QApplication::exec();
}

int main(int argc, char* argv[])
{
	setupApplication();

	QStringList inputFiles;
	QTextStream output(stdout);
	Reporter reporter(output);
	Strategy* s=parseArguments(argc,argv,inputFiles,reporter);
	int retcode=runApplication(s,argc,argv,inputFiles);
	delete s;

	return retcode;
}
