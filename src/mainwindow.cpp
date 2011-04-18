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

#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextDocumentWriter>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "backgroundworker.h"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setupLayout();

	setupToolbar();

	setupTreeview();

	setupEditor();
}

MainWindow::~MainWindow()
{
	delete highlighter;
	delete console;
	delete ui;
}

void MainWindow::setupToolbar()
{
	ui->actionNew->setIcon(QIcon::fromTheme("document-new"));
	ui->actionOpen->setIcon(QIcon::fromTheme("document-open"));
	ui->actionSave->setIcon(QIcon::fromTheme("document-save"));
	ui->actionPrint->setIcon(QIcon::fromTheme("document-print"));

	connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(newFile()));
	connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(openFile()));
	connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(saveFile()));
	connect(ui->actionSaveAs,SIGNAL(triggered()),this,SLOT(saveAsFile()));

	ui->actionUndo->setIcon(QIcon::fromTheme("edit-undo"));
	ui->actionRedo->setIcon(QIcon::fromTheme("edit-redo"));

	connect(ui->actionUndo,SIGNAL(triggered()),ui->scriptEditor,SLOT(undo()));
	connect(ui->actionRedo,SIGNAL(triggered()),ui->scriptEditor,SLOT(redo()));

	ui->actionCut->setIcon(QIcon::fromTheme("edit-cut"));
	ui->actionCopy->setIcon(QIcon::fromTheme("edit-copy"));
	ui->actionPaste->setIcon(QIcon::fromTheme("edit-paste"));

	connect(ui->actionCut,SIGNAL(triggered()),ui->scriptEditor,SLOT(cut()));
	connect(ui->actionCopy,SIGNAL(triggered()),ui->scriptEditor,SLOT(copy()));
	connect(ui->actionPaste,SIGNAL(triggered()),ui->scriptEditor,SLOT(paste()));

	ui->actionCompileAndRender->setIcon(QIcon::fromTheme("system-run"));
	ui->actionGenerateGcode->setIcon(QIcon::fromTheme("format-justify-fill"));

	connect(ui->actionCompileAndRender,SIGNAL(triggered()),this,SLOT(compileAndRender()));
}

void MainWindow::setupLayout()
{
	//TODO there must be a better way than this
	QList<int> hSizes,vSizes;
	hSizes << 160 << 540 << 300;
	ui->vSplitter->setSizes(hSizes);
	vSizes << 150 << 10;
	ui->hSplitter->setSizes(vSizes);
}

void MainWindow::setupTreeview()
{
	myModel = new QStandardItemModel();
	QStringList headers;
	headers << "Projects";
	myModel->setHorizontalHeaderLabels(headers);
	QStandardItem* parentItem = myModel->invisibleRootItem();

	QStandardItem* item = new QStandardItem("New Project.rpro");
	parentItem->appendRow(item);
	item->appendRow(new QStandardItem("New.rcad"));

	ui->treeView->setModel(myModel);
	ui->treeView->expandAll();
}

void MainWindow::setupEditor()
{
	QTextEdit* editor = ui->scriptEditor;

	QFont font;
	font.setFamily("Courier");
	font.setFixedPitch(true);
	font.setPointSize(10);

	editor->setFont(font);

	highlighter = new SyntaxHighlighter(editor->document());

	QTextEdit* c=(QTextEdit*)ui->plainTextEdit;
	font.setPointSize(8);
	c->setFont(font);
	console=new TextEditIODevice(c,this);
}

bool MainWindow::load(const QString& f)
{
	if(!QFile::exists(f))
		return false;
	QFile file(f);
	if(!file.open(QFile::ReadOnly))
		return false;

	QByteArray data=file.readAll();
	QString str=QString::fromLocal8Bit(data);
	ui->scriptEditor->setPlainText(str);

	setCurrentFileName(f);
	return true;
}

bool MainWindow::maybeSave()
{
	if(!ui->scriptEditor->document()->isModified())
		return true;
	QMessageBox::StandardButton ret;
	ret = QMessageBox::warning(this, tr("RapCAD"),
		tr("The document has been modified.\n"
		"Do you want to save your changes?"),
		QMessageBox::Save | QMessageBox::Discard
		| QMessageBox::Cancel);
	if(ret == QMessageBox::Save)
		return saveFile();
	else if(ret == QMessageBox::Cancel)
		return false;
	return true;
}


void MainWindow::newFile()
{
	if(maybeSave()) {
		ui->scriptEditor->clear();
		setCurrentFileName(QString());
	}
}


bool MainWindow::saveFile()
{
	if(fileName.isEmpty())
		return saveAsFile();

	QTextDocumentWriter writer(fileName);
	writer.setFormat("plaintext");
	bool success = writer.write(ui->scriptEditor->document());
	if(success)
		ui->scriptEditor->document()->setModified(false);
	return success;
}

bool MainWindow::saveAsFile()
{
	QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
		QString(), tr("RapCAD Files (*.rcad);;All Files (*)"));
	if(fn.isEmpty())
		return false;
	if(!fn.endsWith(".rcad", Qt::CaseInsensitive))
		fn += ".rcad"; // default
	setCurrentFileName(fn);
	return saveFile();
}

void MainWindow::openFile()
{
	QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
		QString(), tr("RapCAD Files (*.rcad);;All Files (*)"));
	if(!fn.isEmpty())
		load(fn);
}

void MainWindow::setCurrentFileName(const QString& f)
{
	fileName=f;
	ui->scriptEditor->document()->setModified(false);
}

void MainWindow::compileAndRender()
{
	//Stop the syntax highlighter to prevent a crash
	//It will start again automatically.
	highlighter->stop();

	if(maybeSave()) {
		QTextStream out(console);
		BackgroundWorker b(out);
		b.evaluate(fileName,false,"");
	}
}
