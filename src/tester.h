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
#ifndef TESTER_H
#define TESTER_H

#include "strategy.h"
#include "ui/mainwindow.h"

class Tester : public QObject,public Strategy
{
	Q_OBJECT
public:
	Tester(Reporter&,const QString&,QObject* parent=nullptr);
	~Tester() override;
	int evaluate() override;
private slots:
	void runUiTests();
	void handleSaveItemsDialog();
private:
	void writeHeader(const QString& name, int num);
	void writePass();
	void writeFail();
	void writeSkip();
	static bool testFunctionExists(Script&);
	void testModule(Script&,const QFileInfo&);
	void testFunction(Script&);
	void exportTest(const QDir&);
#if USE_CGAL
	void exportTest(Primitive* p,const QFileInfo&,const QFileInfo&,const QString&);
#endif
	void builtinsTest();
	void consoleTest();
	void renderingTest();
	void searchTest();
	void preferencesTest();

	QString directory;
	QString* nullout;
	QTextStream* nullstream;
	Reporter* nullreport;
	int testcount;
	int passcount;
	int failcount;
	MainWindow* ui;
};

#endif // TESTER_H
#endif
