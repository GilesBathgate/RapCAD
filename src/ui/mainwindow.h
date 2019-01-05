/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2019 Giles Bathgate
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
#include "codeeditor.h"
#include "texteditiodevice.h"
#include "backgroundworker.h"
#include "preferencesdialog.h"
#include "interactive.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow() override;
	void loadFiles(const QStringList&);
protected:
	void closeEvent(QCloseEvent*) override;
private slots:
	void getDefaultViewport() const;
	void setDefaultViewport();
	void grabFrameBuffer();
	void showPreferences();
	void disableRulers(bool);
	void enableCaches(bool);
	void clipboardDataChanged();
	bool maybeSave(bool);
	void newFile();
	bool saveFile();
	bool saveAsFile();
	bool saveAllFiles();
	bool closeCurrentFile();
	bool closeFile(int);
	void openFile();
	void compileAndRender();
	void compileAndGenerate();
	void evaluationDone();
	void setTabTitle(const QString&);
	void undo();
	void redo();
	void cut();
	void copy();
	void paste();
	void tabChanged(int);
	void print();
	void showAbout();
	void showAboutQt();
	void showBuiltins();
	void showUserGuide();
	void flushCaches();
	void exportFile(const QString&);
	void preferencesUpdated();
private:
	void compileOrGenerate(bool generate);
	void loadPreferences();
	void savePreferences();
	void setupLayout();
	void setupActions();
	void setupExportActions();
	void setupViewActions();
	void setupEditor(CodeEditor*);
	void setupTabs(QTabWidget*);
	void setupConsole();
	void setupTreeview();
	CodeEditor* currentEditor();
	CodeEditor* getEditor(int i);
	void disableActions(CodeEditor*);
	bool saveSelectedFiles(const QList<QString>&);

	Ui::MainWindow* ui;
	QStandardItemModel* myModel;
	QTextStream* output;
	Reporter* reporter;
	TextEditIODevice* console;
	BackgroundWorker* worker;
	Interactive* interact;
	PreferencesDialog* preferencesDialog;
};

#endif // MAINWINDOW_H
