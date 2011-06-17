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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTextStream>
#include "syntaxhighlighter.h"
#include "texteditiodevice.h"
#include "backgroundworker.h"
#include "preferencesdialog.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();
	bool load(const QString&);
protected:
	virtual void closeEvent(QCloseEvent*);
private slots:
	void grabFrameBuffer();
	void saveExport();
	void showPreferences();
	void disableRulers(bool);
	void clipboardDataChanged();
	bool maybeSave(bool);
	void newFile();
	bool saveFile();
	bool saveAsFile();
	void openFile();
	void compileAndRender();
	void evaluationDone(CGALPrimitive*);
private:
	void setupLayout();
	void setupActions();
	void setupEditor();
	void setupTreeview();
	void setCurrentFileName(const QString&);
	Ui::MainWindow* ui;
	QStandardItemModel* myModel;
	SyntaxHighlighter* highlighter;
	QTextStream* output;
	TextEditIODevice* console;
	BackgroundWorker* worker;
	QString fileName;
	PreferencesDialog* preferencesDialog;
	CGALPrimitive* primitive;
};

#endif // MAINWINDOW_H
