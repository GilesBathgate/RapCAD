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

#include <QFileDialog>
#include <QMessageBox>
#include <QTextDocumentWriter>
#include <QMimeData>
#include <QClipboard>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cgalprimitive.h"
#include "cgalrenderer.h"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setupLayout();

	setupActions();

	setupTreeview();

	setupEditor();
}

MainWindow::~MainWindow()
{
	delete highlighter;
	delete console;
	delete output;
	delete worker;
	delete ui;
}

void MainWindow::setupActions()
{
	ui->actionNew->setIcon(QIcon::fromTheme("document-new"));
	connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(newFile()));

	ui->actionOpen->setIcon(QIcon::fromTheme("document-open"));
	connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(openFile()));

	ui->actionSave->setIcon(QIcon::fromTheme("document-save"));
	ui->actionSave->setEnabled(false);
	connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(saveFile()));
	connect(ui->scriptEditor->document(), SIGNAL(modificationChanged(bool)),ui->actionSave, SLOT(setEnabled(bool)));

	ui->actionPrint->setIcon(QIcon::fromTheme("document-print"));

	connect(ui->actionSaveAs,SIGNAL(triggered()),this,SLOT(saveAsFile()));
	connect(ui->actionQuit,SIGNAL(triggered()),this,SLOT(close()));

	ui->actionUndo->setIcon(QIcon::fromTheme("edit-undo"));
	ui->actionUndo->setEnabled(ui->scriptEditor->document()->isUndoAvailable());
	connect(ui->actionUndo,SIGNAL(triggered()),ui->scriptEditor,SLOT(undo()));
	connect(ui->scriptEditor->document(), SIGNAL(undoAvailable(bool)),ui->actionUndo, SLOT(setEnabled(bool)));

	ui->actionRedo->setIcon(QIcon::fromTheme("edit-redo"));
	ui->actionRedo->setEnabled(ui->scriptEditor->document()->isRedoAvailable());
	connect(ui->actionRedo,SIGNAL(triggered()),ui->scriptEditor,SLOT(redo()));
	connect(ui->scriptEditor->document(), SIGNAL(redoAvailable(bool)),ui->actionRedo, SLOT(setEnabled(bool)));

	ui->actionCut->setIcon(QIcon::fromTheme("edit-cut"));
	ui->actionCut->setEnabled(false);
	connect(ui->scriptEditor, SIGNAL(copyAvailable(bool)), ui->actionCut, SLOT(setEnabled(bool)));
	connect(ui->actionCut,SIGNAL(triggered()),ui->scriptEditor,SLOT(cut()));

	ui->actionCopy->setIcon(QIcon::fromTheme("edit-copy"));
	ui->actionCopy->setEnabled(false);
	connect(ui->scriptEditor, SIGNAL(copyAvailable(bool)), ui->actionCopy, SLOT(setEnabled(bool)));
	connect(ui->actionCopy,SIGNAL(triggered()),ui->scriptEditor,SLOT(copy()));

	ui->actionPaste->setIcon(QIcon::fromTheme("edit-paste"));
	connect(ui->actionPaste,SIGNAL(triggered()),ui->scriptEditor,SLOT(paste()));
	connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));
	clipboardDataChanged();

	ui->actionCompileAndRender->setIcon(QIcon::fromTheme("system-run"));
	connect(ui->actionCompileAndRender,SIGNAL(triggered()),this,SLOT(compileAndRender()));

	ui->actionGenerateGcode->setIcon(QIcon::fromTheme("format-justify-fill"));
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
	QPlainTextEdit* editor = ui->scriptEditor;

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
	output=new QTextStream(console);
	worker=new BackgroundWorker(*output);
	connect(worker,SIGNAL(done(CGALPrimitive*)),this,SLOT(evaluationDone(CGALPrimitive*)));
}

void MainWindow::clipboardDataChanged()
{
	if(const QMimeData* md = QApplication::clipboard()->mimeData())
		ui->actionPaste->setEnabled(md->hasText());
}


void MainWindow::closeEvent(QCloseEvent* e)
{
	if(maybeSave(true))
		e->accept();
	else
		e->ignore();
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

bool MainWindow::maybeSave(bool closing)
{
	if(!ui->scriptEditor->document()->isModified())
		return true;

	int buttons=QMessageBox::Save | QMessageBox::Cancel;
	if(closing)
		buttons |= QMessageBox::Discard;

	QMessageBox::StandardButton ret;
	ret = QMessageBox::warning(this, tr("RapCAD"),
		tr("The document has been modified.\n"
		"Do you want to save your changes?"),
		(QMessageBox::StandardButton)buttons);
	if(ret == QMessageBox::Save)
		return saveFile();
	else if(ret == QMessageBox::Cancel)
		return false;
	return true;
}


void MainWindow::newFile()
{
	if(maybeSave(true)) {
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

	if(maybeSave(false)) {
		worker->evaluate(fileName,false,"");
		ui->actionCompileAndRender->setEnabled(false);
	}
}

void MainWindow::evaluationDone(CGALPrimitive* n)
{
	if(n) {
		CGALRenderer* r = new CGALRenderer(*n);
		ui->view->setRenderer(r);
	}
	ui->actionCompileAndRender->setEnabled(true);
}
