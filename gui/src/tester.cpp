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
#ifdef USE_INTEGTEST

#include "tester.h"
#include "ui/console.h"
#include "preferences.h"

#include <QApplication>
#include <QLineEdit>
#include <QMenu>
#include <QTimer>
#include <QtTest/QTest>
#include <boost/version.hpp>
#include <gmp.h>
#include <mpfr.h>

Tester::Tester(Reporter& r,QObject* parent) :
	QObject(parent),
	Strategy(r),
	ui(nullptr)
{
}

int Tester::evaluate()
{
	output << QString("Qt:\t %1").arg(QT_VERSION_STR) << Qt::endl;
#ifdef USE_CGAL
	output << QString("CGAL:\t %1").arg(CGAL_VERSION_STR)<< Qt::endl;
#endif
	output << QString("Boost:\t %1.%2.%3").arg(BOOST_VERSION / 100000).arg(BOOST_VERSION / 100 % 1000).arg(BOOST_VERSION % 100)<< Qt::endl;
	output << QString("MPFR:\t %1").arg(MPFR_VERSION_STRING)<< Qt::endl;
	output << QString("GMP:\t %1").arg(gmp_version)<< Qt::endl;

	reporter.setReturnCode(EXIT_SUCCESS);

#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS) && !defined(USE_VALGRIND)
	reporter.startTiming();

	auto& p=Preferences::getInstance();
	const bool autosave=p.getAutoSaveOnCompile();
	p.setAutoSaveOnCompile(false);

	int c=0;
	const QApplication a(c,nullptr);
	ui = new MainWindow();
	ui->show();
	QTimer::singleShot(100,this,&Tester::runUiTests);
	QApplication::exec();
	delete ui;

	p.setAutoSaveOnCompile(autosave);

	reporter.stopTiming("ui testing");
#endif
	reporter.reportTimings();
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

	QTimer::singleShot(1000,ui,&MainWindow::close);
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
	QTimer::singleShot(100,this,&Tester::handlePreferencesDialog);
	QTest::keyClick(menuEdit,Qt::Key_Enter);
}

void Tester::handlePreferencesDialog()
{
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
	QTimer::singleShot(100,this,&Tester::handleSaveItemsDialog);
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

#endif
