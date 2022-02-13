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
#ifdef USE_INTEGTEST
#include <QDir>
#include <QTimer>
#include <QApplication>
#include <QLineEdit>
#include <QtTest/QTest>
#include <QMenu>
#include <boost/version.hpp>
#include <mpfr.h>
#include <gmp.h>
#ifdef USE_CGAL
#include "cgal.h"
#include "cgalexport.h"
#endif
#include "tester.h"
#include "treeevaluator.h"
#include "nodeprinter.h"
#include "booleanvalue.h"
#include "comparer.h"
#include "cachemanager.h"
#include "treeprinter.h"
#include "asciidocprinter.h"
#include "builtincreator.h"
#include "nodeevaluator.h"
#include "ui/codeeditor.h"
#include "ui/console.h"
#include "ui/searchwidget.h"
#include "preferences.h"
#include "contrib/qtcompat.h"

Tester::Tester(Reporter& r,const QString& d,QObject* parent) :
	QObject(parent),
	Strategy(r),
	directory(d),
	nullout(new QString()),
	nullstream(new QTextStream(nullout)),
	nullreport(new Reporter(*nullstream)),
	testcount(0),
	passcount(0),
	failcount(0),
	ui(nullptr)
{
}

Tester::~Tester()
{
	delete nullout;
	delete nullstream;
	delete nullreport;
}

void Tester::writeHeader(const QString& name, int num)
{
	testTimer.start();
	output << "Test #" << QString().setNum(num).rightJustified(3,'0') << ": ";
	output << name.leftJustified(50,'.',true);
	output.flush();
}

void Tester::writeTestTime()
{
	float timeTaken = testTimer.nsecsElapsed()/1000000.0f;
#ifndef Q_OS_WIN
	output << "\e[0;33m";
#endif
	output << QString("%1ms").arg(timeTaken,10,'f',2);
#ifndef Q_OS_WIN
	output << "\e[0m";
#endif
	testTimer.invalidate();
}

void Tester::writePass()
{
#ifdef Q_OS_WIN
	output << " Passed";
#else
	output << " \e[0;32mPassed\e[0m";
#endif
	writeTestTime();
	output << Qt::endl;
}

void Tester::writeFail()
{
#ifdef Q_OS_WIN
	output << " FAILED" << Qt::endl;
#else
	output << " \e[0;31mFAILED\e[0m" << Qt::endl;
#endif
	testTimer.invalidate();
}

void Tester::writeSkip()
{
#ifdef Q_OS_WIN
	output << " Skipped" << Qt::endl;
#else
	output << " \e[0;33mSkipped\e[0m" << Qt::endl;
#endif
	testTimer.invalidate();
}

static bool skipDir(const QString& dir)
{
#ifndef USE_OFFSET
	if(dir=="051_offset") return true;
#endif
#ifndef USE_SIMPLIFY
	if(dir=="087_simplify") return true;
#endif
#ifdef Q_OS_WIN
	if(dir=="063_rands") return true;
#endif
#ifndef USE_SUBDIV
	if(dir=="090_subdiv") return true;
#endif
	return (dir=="");
}

int Tester::evaluate()
{
	reporter.startTiming();

	auto& cm=CacheManager::getInstance();
	cm.disableCaches();

	output << QString("Qt:\t %1").arg(QT_VERSION_STR) << Qt::endl;
#ifdef USE_CGAL
	output << QString("CGAL:\t %1").arg(CGAL_VERSION_STR)<< Qt::endl;
#endif
	output << QString("Boost:\t %1.%2.%3").arg(BOOST_VERSION / 100000).arg(BOOST_VERSION / 100 % 1000).arg(BOOST_VERSION % 100)<< Qt::endl;
	output << QString("MPFR:\t %1").arg(MPFR_VERSION_STRING)<< Qt::endl;
	output << QString("GMP:\t %1").arg(gmp_version)<< Qt::endl;

	writeHeader("000_treeprinter",testcount);

	auto& cr=BuiltinCreator::getInstance(*nullreport);

	TreePrinter nulldocs(*nullstream);
	cr.generateDocs(nulldocs);

	AsciidocPrinter nullasciidocs(*nullstream,*nullstream);
	cr.generateDocs(nullasciidocs);

	writePass();

	QDir testDir(directory);
	/* This hard coded filter need to be addressed
	 * but it will do for now. */
	const auto entries=testDir.entryInfoList(QStringList("*_*"));
	for(const auto& entry: entries) {

		QDir dir(entry.absoluteFilePath());
		QString testDirName=entry.fileName();
		if(testDirName=="061_export") {
#ifndef Q_OS_WIN
			exportTest(dir);
#endif
			continue;
		}

		const auto files=dir.entryInfoList(QStringList("*.rcad"), QDir::Files);
		for(const auto& file: files) {

			writeHeader(file.fileName(),++testcount);

			if(skipDir(testDirName)) {
				writeSkip();
				continue;
			}

			Script s(*nullreport);
			s.parse(file);

			if(testFunctionExists(s)) {
				testFunction(s);
			} else {
				testModule(s,file);
			}
		}
	}
	reporter.setReturnCode(failcount);

	output << "Total: " << testcount << " Passed: " << passcount << " Failed: " << failcount << Qt::endl;

	reporter.reportTiming("testing");
#if !defined(Q_OS_WIN) && !defined(USE_VALGRIND)
	reporter.startTiming();

	auto& p=Preferences::getInstance();
	bool autosave=p.getAutoSaveOnCompile();
	p.setAutoSaveOnCompile(false);

	int c=0;
	QApplication a(c,nullptr);
	ui = new MainWindow();
	ui->show();
	QTimer::singleShot(100,this,SLOT(runUiTests()));
	QApplication::exec();
	delete ui;

	p.setAutoSaveOnCompile(autosave);

	reporter.reportTiming("ui testing");
#endif
	return reporter.getReturnCode();
}

void Tester::runUiTests()
{
	aboutTest();
	preferencesTest();
	renderingTest();
	searchTest();
	consoleTest();
	builtinsTest();

	QTimer::singleShot(1000,ui,SLOT(close()));
}

void Tester::aboutTest()
{
	ui->activateWindow();
	QTest::keyClick(ui,Qt::Key_E,Qt::AltModifier);
	auto* menuHelp = ui->findChild<QMenu*>("menuHelp");
	QTest::keyClick(menuHelp,Qt::Key_Up);
	QTest::keyClick(menuHelp,Qt::Key_Up);
	QTest::keyClick(menuHelp,Qt::Key_Up);
	QTest::keyClick(menuHelp,Qt::Key_Enter);

	auto* about = ui->findChild<QDialog*>("AboutDialog");
	about->activateWindow();
	QTest::keyClick(about,Qt::Key_Enter,Qt::NoModifier,100);
}

void Tester::preferencesTest()
{
	ui->activateWindow();
	QTest::keyClick(ui,Qt::Key_E,Qt::AltModifier);
	auto* menuEdit = ui->findChild<QMenu*>("menuEdit");
	QTest::keyClick(menuEdit,Qt::Key_Up);
	QTest::keyClick(menuEdit,Qt::Key_Enter);

	auto* prefs = ui->findChild<QDialog*>("Preferences");
	prefs->activateWindow();
	QTest::keyClick(prefs,Qt::Key_Enter,Qt::NoModifier,100);
}

void Tester::searchTest()
{
	ui->activateWindow();
	QTest::keyClick(ui,Qt::Key_F,Qt::ControlModifier,100);
	auto* search=ui->findChild<QWidget*>("searchWidget");
	auto* edit=search->findChild<QLineEdit*>("searchLineEdit");
	QTest::keyClicks(edit,"cube",Qt::NoModifier);
	QTest::keyClick(edit,Qt::Key_F3,Qt::NoModifier,100);
	QTest::keyClick(edit,Qt::Key_F3,Qt::ShiftModifier,100);
	QTest::keyClick(ui,Qt::Key_H,Qt::ControlModifier,100);
}

void Tester::renderingTest()
{
	ui->activateWindow();
	QTest::keyClick(ui,Qt::Key_D,Qt::AltModifier,100);
	auto* menuDesign = ui->findChild<QMenu*>("menuDesign");
	QTest::keyClick(menuDesign,Qt::Key_Down,Qt::NoModifier,100);
	QTest::keyClick(menuDesign,Qt::Key_Down,Qt::NoModifier,100);
	QTest::keyClick(menuDesign,Qt::Key_Down,Qt::NoModifier,100);
	QTest::keyClick(menuDesign,Qt::Key_Return,Qt::NoModifier,100);

	QFile f("test.rcad");
	ui->activateWindow();
	auto* edit = ui->findChild<CodeEditor*>("scriptEditor");
	edit->activateWindow();
	QTest::keyClicks(edit,"cube(10);");
	QTest::keyClick(edit,Qt::Key_Tab,Qt::NoModifier,100);
	QTest::keyClick(edit,Qt::Key_Tab,Qt::ControlModifier,100);
	QTimer::singleShot(100,this,SLOT(handleSaveItemsDialog()));
	QTest::keyClick(ui,Qt::Key_F6);
	edit->setFileName(f.fileName());
	edit->saveFile();
	QTest::keyClick(ui,Qt::Key_F6,Qt::NoModifier,100);
	f.remove();
}

void Tester::consoleTest()
{
	ui->activateWindow();
	auto* console = ui->findChild<Console*>("console");
	console->activateWindow();
	QTest::keyClicks(console,"1+2");
	QTest::keyClick(console,Qt::Key_Return,Qt::NoModifier,100);
	QTest::keyClick(console,Qt::Key_Up,Qt::NoModifier,100);
	QTest::keyClicks(console,"+3");
	QTest::keyClick(console,Qt::Key_Return,Qt::NoModifier,100);
}

void Tester::builtinsTest()
{
	ui->activateWindow();
	QTest::keyClick(ui,Qt::Key_D,Qt::AltModifier,100);
	auto* menuDesign = ui->findChild<QMenu*>("menuDesign");
	QTest::keyClick(menuDesign,Qt::Key_B,Qt::NoModifier,100);
}

void Tester::handleSaveItemsDialog()
{
	for(int i=0; i<10; ++i) {
		auto* sd=ui->findChild<QDialog*>("SaveItemsDialog");
		if(sd) {
			sd->activateWindow();
			QTest::keyClick(sd,Qt::Key_C,Qt::AltModifier,100);
			sd->close();
			return;
		}
		QTest::qSleep(100);
	}
}

void Tester::exportTest(const QDir& dir)
{
	Reporter& r=*nullreport;
	const auto files=dir.entryInfoList(QStringList("*.rcad"), QDir::Files);
	for(const auto& file: files) {
		Script s(r);
		s.parse(file);
		TreeEvaluator te(r);
		s.accept(te);
		NodeEvaluator ne(r);
		Node* n=te.getRootNode();
		n->accept(ne);
		Primitive* p=ne.getResult();
#if USE_CGAL
		QDir path(file.absolutePath());

		const QFileInfo origPath(path.filePath(file.baseName()+".csg"));
		const CGALExport e(origPath,p,r);
		e.exportResult();

		exportTest(p,origPath,file,".stl");
		exportTest(p,origPath,file,".obj");
		exportTest(p,origPath,file,".off");
		exportTest(p,origPath,file,".amf");
		exportTest(p,origPath,file,".3mf");
		exportTest(p,origPath,file,".nef");

		QFile::remove(origPath.absoluteFilePath());
		delete p;
		delete n;
#endif
	}
}

#if USE_CGAL
void Tester::exportTest(Primitive* p,const QFileInfo& origPath,const QFileInfo& file,const QString& ext)
{
	QString newName=file.baseName()+ext;

	writeHeader(newName,++testcount);

	QDir path(file.absolutePath());
	const QFileInfo newPath(path.filePath(newName));
	const CGALExport e(newPath,p,*nullreport);
	e.exportResult();
	Comparer c(*nullreport);
	c.setup(origPath.absoluteFilePath(),newPath.absoluteFilePath());
	c.evaluate();
	if(c.evaluate()==EXIT_SUCCESS) {
		writePass();
		passcount++;
	} else {
		writeFail();
		failcount++;
	}

	QFile::remove(newPath.absoluteFilePath());
}
#endif

void Tester::testFunction(Script& s)
{
	TreeEvaluator te(*nullreport);
	//If a test function exists check it returns true
	QList<Argument*> args;
	Callback* c = addCallback("test",s,args);
	s.accept(te);
	auto* v = dynamic_cast<BooleanValue*>(c->getResult());
	if(v && v->isTrue()) {
		writePass();
		passcount++;
	} else {
		writeFail();
		failcount++;
	}
	delete v;

	Node* n=te.getRootNode();
	delete n;
}

void Tester::testModule(Script& s,const QFileInfo& file)
{
#ifdef Q_OS_WIN
	writeSkip();
	return;
#endif
	TreeEvaluator te(*nullreport);

	QString basename=file.baseName();
	QString examFileName=basename + ".exam.csg";
	QString csgFileName=basename + ".csg";
	QFileInfo examFileInfo(file.absoluteDir(),examFileName);
	QFileInfo csgFileInfo(file.absoluteDir(),csgFileName);
	QFile examFile(examFileInfo.absoluteFilePath());
	s.accept(te);

	//Create exam file
	examFile.open(QFile::WriteOnly);
	QTextStream examout(&examFile);
	NodePrinter p(examout);
	Node* n=te.getRootNode();
	n->accept(p);
	delete n;
	examout.flush();
	examFile.close();

	QFile csgFile(csgFileInfo.absoluteFilePath());
	if(csgFile.exists()) {
		Comparer co(*nullreport);
		co.setup(examFileInfo.absoluteFilePath(),csgFileInfo.absoluteFilePath());
		if(co.evaluate()==EXIT_SUCCESS) {
			writePass();
			passcount++;
		} else {
			writeFail();
			failcount++;
		}
		examFile.remove();
	} else {
		output << "Created" << Qt::endl;
	}
}

bool Tester::testFunctionExists(Script& s)
{
	for(Declaration* d: s.getDeclarations()) {
		auto* func=dynamic_cast<Function*>(d);
		if(func && func->getName()=="test")
			return true;
	}

	return false;
}
#endif
