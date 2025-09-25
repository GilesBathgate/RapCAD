/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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
#include "application.h"

#include "comparer.h"
#ifdef USE_INTEGTEST
#include "generator.h"
#include "tester.h"
#endif
#include "interactive.h"
#include "preferences.h"
#include "stringify.h"
#include "ui/mainwindow.h"
#include "worker.h"
#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <contrib/qtcompat.h>

Application::Application() :
	output(stdout),
	messages(stderr),
	reporter(output,messages),
	strategy(nullptr),
	redirectFile(nullptr)
{
	QCoreApplication::setOrganizationName("rapcad");
	QCoreApplication::setOrganizationDomain("rapcad.org");
	QCoreApplication::setApplicationName("RapCAD");
	QCoreApplication::setApplicationVersion(STRINGIFY(RAPCAD_VERSION));
#ifdef USE_CGAL
	CGAL::set_error_handler([](auto...){});
#endif
	//Ensure preferences have been initialised early.
	auto& p=Preferences::getInstance();
	if(p.getSoftwareOpenGL()) {
		QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
	}
}

Application::~Application()
{
	delete redirectFile;
	delete strategy;
}

#if defined (USE_INTEGTEST) || defined (USE_READLINE)
static void showVersion(QTextStream& out)
{
	out << QCoreApplication::applicationName() << " " << QCoreApplication::applicationVersion() << Qt::endl;
}
#endif

#ifdef Q_OS_WIN
static QStringList getArguments(int argc,char*[])
{
	QStringList list;
	if(wchar_t** argv=CommandLineToArgvW(GetCommandLineW(),&argc)) {
		for(int a=0; a<argc; ++a) {
			list << QString::fromWCharArray(argv[a]);
		}
		LocalFree(argv);
	}
	return list;
}
#else
static QStringList getArguments(int argc,char* argv[])
{
	QStringList list;
	for(int a=0; a<argc; ++a) {
		list << QString::fromLocal8Bit(argv[a]);
	}
	return list;
}
#endif

int Application::run(int argc,char* argv[])
{
	strategy=parseArguments(argc,argv);
	if(strategy)
		return strategy->evaluate();

	return runUserInterface(argc,argv);
}

Strategy* Application::parseArguments(int argc,char* argv[])
{
	const QStringList& arguments=getArguments(argc,argv);

	QCommandLineParser p;
	p.setApplicationDescription(QCoreApplication::translate("main","RapCAD the rapid prototyping IDE."));
	const QCommandLineOption& helpOption=p.addHelpOption();
	const QCommandLineOption& versionOption=p.addVersionOption();
	p.addPositionalArgument("filename", QCoreApplication::translate("main","File to open or process."));

#ifdef USE_INTEGTEST
	const QCommandLineOption testOption(QStringList() << "t" << "test", QCoreApplication::translate("main","Run through tests in working directory."),"directory");
	p.addOption(testOption);

	const QCommandLineOption generateOption(QStringList() << "g" << "generate", QCoreApplication::translate("main","Generate documentation."));
	p.addOption(generateOption);
#endif

	const QCommandLineOption compareOption(QStringList() << "c" << "compare", QCoreApplication::translate("main","Compare two files to see if they are identical."),"filename");
	p.addOption(compareOption);

	const QCommandLineOption outputOption(QStringList() << "o" << "output",QCoreApplication::translate("main","Create output geometry <filename> filename must end with known extension (.stl/.amf/.3mf/...)."),"filename");
	p.addOption(outputOption);

	const QCommandLineOption preferenceOption(QStringList() << "p" << "preference",QCoreApplication::translate("main","Set a preference value"),"name-value");
	p.addOption(preferenceOption);

	const QCommandLineOption redirectOption(QStringList() << "r" << "redirect",QCoreApplication::translate("main","Redirect text output to file <filename>."),"filename");
	p.addOption(redirectOption);

	const QCommandLineOption viewAllOption("viewall",QCoreApplication::translate("main","adjust camera to fit object"));
	p.addOption(viewAllOption);

#ifdef USE_READLINE
	const QCommandLineOption interactOption(QStringList() << "i" << "interactive",QCoreApplication::translate("main","Start an interactive session."));
	p.addOption(interactOption);
#endif

	// Work around for Qt dependency
	// on QCoreApplication in showHelp
	if(!p.parse(arguments)) {
		p.process(arguments); // exits
	} else {
		if(p.isSet(helpOption)) {
			const QCoreApplication a(argc,argv);
			p.showHelp(); // exits
		} else if(p.isSet(versionOption)) {
			p.showVersion(); // exits
		}
	}

	if(p.isSet(redirectOption)) {
		redirectFile=new QFile(p.value(redirectOption));
		if(redirectFile->open(QFile::WriteOnly))
			reporter.output.setDevice(redirectFile);
	}

	inputFiles=p.positionalArguments();
	QString inputFile;
	if(!inputFiles.isEmpty()) {
		inputFile=inputFiles.at(0);
	}

	if(p.isSet(preferenceOption)) {
		auto& preferences=Preferences::getInstance();
		for(const auto& nameValue : p.values(preferenceOption)) {
			preferences.setNamedPreference(nameValue);
		}
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


int Application::runUserInterface(int argc,char* argv[])
{
	const QApplication a(argc,argv);
	MainWindow w;
	w.loadFiles(inputFiles);
	w.show();
	return QApplication::exec();
}
