/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2012 Giles Bathgate
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
#include "CodeEditor.h"
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
	bool loadFile(const QString&);
protected:
	virtual void closeEvent(QCloseEvent*);
private slots:
	void getDefaultViewport();
	void setDefaultViewport();
	void grabFrameBuffer();
	void exportAsciiSTL();
	void exportOFF();
	void exportAMF();
	void showPreferences();
	void disableRulers(bool);
	void clipboardDataChanged();
	bool maybeSave(bool);
	void newFile();
	bool saveFile();
	bool saveAsFile();
	bool saveAllFiles();
	bool closeFile();
	void openFile();
	void compileAndRender();
	void evaluationDone(Primitive*);
	void setTabTitle(const QString&);
	void undo();
	void redo();
	void cut();
	void copy();
	void paste();
	void tabChanged(int);
	void print();
	void showAbout();
private:
	void loadPreferences();
	void savePreferences();
	void setupLayout();
	void setupActions();
	void setupEditor(CodeEditor*);
	void setupTabs(QTabWidget*);
	void setupConsole();
	void setupTreeview();
	CodeEditor* currentEditor();
	void disableActions(CodeEditor*);
	bool saveSelectedFiles(QList<QString>);

	Ui::MainWindow* ui;
	QStandardItemModel* myModel;
	QTextStream* output;
	TextEditIODevice* console;
	BackgroundWorker* worker;
	PreferencesDialog* preferencesDialog;
	Primitive* primitive;
};

#endif // MAINWINDOW_H
