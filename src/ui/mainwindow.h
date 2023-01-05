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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "aboutdialog.h"
#include "backgroundworker.h"
#include "codeeditor.h"
#include "interactive.h"
#include "preferencesdialog.h"
#include "project.h"
#include "texteditiodevice.h"
#include <QMainWindow>
#include <QTemporaryFile>
#include <QTextStream>

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
	static QString getSaveFileName(QWidget*,const QString&,const QString&,const QString&,const QString&);
protected:
	void closeEvent(QCloseEvent*) override;
private slots:
	void getDefaultViewport() const;
	void setDefaultViewport();
	void grabFrameBuffer();
	void showPreferences();
	void disableRulers(bool);
	static void enableCaches(bool);
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
	void examplesListClicked(const QModelIndex&);
	void setTabTitle(const QString&);
	void undo();
	void redo();
	void increaseSelectionIndent();
	void decreaseSelectionIndent();
	void cut();
	void copy();
	void paste();
	void tabChanged(int);
	static void print();
	void showAbout();
	void showAboutQt();
	void showBuiltins();
	static void showUserGuide();
	static void flushCaches();
	void sendToCAM();
	void exportFile(const QString&);
	void preferencesUpdated();
	void newProject();
private:
	void setTheme();
	void compileOrGenerate(bool generate);
	void loadPreferences();
	void savePreferences();
	void setupLayout();
	void setupActions();
	void setupExportActions();
	void setupViewActions();
	void setupEditor(CodeEditor*);
	void setupTabs(QTabWidget*) const;
	void setupConsole();
	void setupExamples();
	void setupTreeview();
	CodeEditor* currentEditor();
	CodeEditor* getEditor(int i);
	void disableActions(CodeEditor*);
	bool saveSelectedFiles(const QList<QString>&);
	void deleteTempFiles();

	Ui::MainWindow* ui;
	Project* projectModel;
	TextEditIODevice* console;
	QTextStream* output;
	Reporter* reporter;
	BackgroundWorker* worker;
	Interactive* interact;
	AboutDialog* aboutDialog;
	PreferencesDialog* preferencesDialog;
	QList<QTemporaryFile*> temporyFiles;
};

#endif // MAINWINDOW_H
