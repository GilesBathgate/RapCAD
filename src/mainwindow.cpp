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
#include <QMimeData>
#include <QClipboard>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cgalprimitive.h"
#include "cgalrenderer.h"
#include "cgalexport.h"
#include "preferences.h"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QIcon rapcadIcon(":/icons/rapcad-16x16.png");
	this->setWindowIcon(rapcadIcon);
	setupLayout();
	setupActions();
	setupEditor(ui->scriptEditor);
	setupTreeview();
	setupConsole();
	preferencesDialog=NULL;
	loadPreferences();
}

MainWindow::~MainWindow()
{
	delete console;
	delete output;
	delete worker;
	delete preferencesDialog;
	delete ui;
}

void MainWindow::savePreferences()
{
	Preferences* p=Preferences::getInstance();
	p->setShowRulers(ui->actionShowRulers->isChecked());
	p->setShowAxes(ui->actionShowAxes->isChecked());
	p->setShowEdges(ui->actionShowEdges->isChecked());
	p->setSkeleton(ui->actionSkeleton->isChecked());
	p->setShowBase(ui->actionShowBase->isChecked());
	p->setShowPrintArea(ui->actionShowPrintArea->isChecked());
	p->setShowEditor(ui->actionShowEditor->isChecked());
	p->setShowConsole(ui->actionShowConsole->isChecked());
	p->setShowProjects(ui->actionShowProjects->isChecked());
	p->setWindowPosition(pos());
	p->setWindowSize(size());
}

void MainWindow::setDefaultViewport()
{
	Preferences* p=Preferences::getInstance();
	double rx,rz,x,z,d;
	ui->view->getViewport(rx,rz,x,z,d);
	p->setDefaultRotationX(rx);
	p->setDefaultRotationZ(rz);
	p->setDefaultX(x);
	p->setDefaultZ(z);
	p->setDefaultDistance(d);
}

void MainWindow::loadPreferences()
{
	Preferences* p=Preferences::getInstance();

	bool showRulers=p->getShowRulers();
	ui->actionShowRulers->setChecked(showRulers);
	ui->view->setShowRulers(showRulers);

	bool showAxes=p->getShowAxes();
	ui->actionShowAxes->setChecked(showAxes);
	ui->view->setShowAxes(showAxes);
	disableRulers(showAxes);

	bool showEdges=p->getShowEdges();
	ui->actionShowEdges->setChecked(showEdges);
	ui->view->setShowEdges(showEdges);

	bool showSkeleton=p->getSkeleton();
	ui->actionSkeleton->setChecked(showSkeleton);
	ui->view->setSkeleton(showSkeleton);

	bool showBase=p->getShowBase();
	ui->actionShowBase->setChecked(showBase);
	ui->view->setShowBase(showBase);

	bool showPrintArea=p->getShowPrintArea();
	ui->actionShowPrintArea->setChecked(showPrintArea);
	ui->view->setShowPrintArea(showPrintArea);

	bool showEditor=p->getShowEditor();
	ui->actionShowEditor->setChecked(showEditor);
	ui->tabWidget->setVisible(showEditor);

	bool showConsole=p->getShowConsole();
	ui->actionShowConsole->setChecked(showConsole);
	ui->plainTextEdit->setVisible(showConsole);

	bool showProjects=p->getShowProjects();
	ui->actionShowProjects->setChecked(showProjects);
	ui->treeView->setVisible(showProjects);
	move(p->getWindowPosition());
	resize(p->getWindowSize());

	getDefaultViewport();
}

void MainWindow::getDefaultViewport()
{
	Preferences* p=Preferences::getInstance();
	double rx,rz,x,z,d;
	rx=p->getDefaultRotationX();
	rz=p->getDefaultRotationZ();
	x=p->getDefaultX();
	z=p->getDefaultZ();
	d=p->getDefaultDistance();
	ui->view->setViewport(rx,rz,x,z,d);
}

void MainWindow::setupActions()
{
	if (!QIcon::hasThemeIcon("document-open")) {
		QIcon::setThemeName("gnome");
	}

	ui->actionNew->setIcon(QIcon::fromTheme("document-new"));
	ui->actionOpen->setIcon(QIcon::fromTheme("document-open"));
	ui->actionSave->setIcon(QIcon::fromTheme("document-save"));
	ui->actionPrint->setIcon(QIcon::fromTheme("document-print"));
	ui->actionUndo->setIcon(QIcon::fromTheme("edit-undo"));
	ui->actionRedo->setIcon(QIcon::fromTheme("edit-redo"));
	ui->actionCut->setIcon(QIcon::fromTheme("edit-cut"));
	ui->actionCopy->setIcon(QIcon::fromTheme("edit-copy"));
	ui->actionPaste->setIcon(QIcon::fromTheme("edit-paste"));
	ui->actionCompileAndRender->setIcon(QIcon::fromTheme("system-run"));
	ui->actionGenerateGcode->setIcon(QIcon::fromTheme("format-justify-fill"));
	ui->actionPreferences->setIcon(QIcon::fromTheme("document-properties"));

	connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(newFile()));
	connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(openFile()));
	connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(saveFile()));
	connect(ui->actionSaveAll,SIGNAL(triggered()),this,SLOT(saveAllFiles()));
	connect(ui->actionSaveAs,SIGNAL(triggered()),this,SLOT(saveAsFile()));
	connect(ui->actionQuit,SIGNAL(triggered()),this,SLOT(close()));
	connect(ui->actionUndo,SIGNAL(triggered()),this,SLOT(undo()));
	connect(ui->actionRedo,SIGNAL(triggered()),this,SLOT(redo()));
	connect(ui->actionCut,SIGNAL(triggered()),this,SLOT(cut()));
	connect(ui->actionCopy,SIGNAL(triggered()),this,SLOT(copy()));
	connect(ui->actionPaste,SIGNAL(triggered()),this,SLOT(paste()));

	clipboardDataChanged();
	connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));

	//Make sure when axis is toggled rulers are disabled but not vice versa
	connect(ui->actionShowAxes,SIGNAL(triggered(bool)),this,SLOT(disableRulers(bool)));
	connect(ui->actionShowEdges,SIGNAL(triggered(bool)),ui->view,SLOT(setShowEdges(bool)));
	connect(ui->actionSkeleton,SIGNAL(triggered(bool)),ui->view,SLOT(setSkeleton(bool)));
	connect(ui->actionShowAxes,SIGNAL(triggered(bool)),ui->view,SLOT(setShowAxes(bool)));
	connect(ui->actionShowBase,SIGNAL(triggered(bool)),ui->view,SLOT(setShowBase(bool)));
	connect(ui->actionShowPrintArea,SIGNAL(triggered(bool)),ui->view,SLOT(setShowPrintArea(bool)));
	connect(ui->actionShowRulers,SIGNAL(triggered(bool)),ui->view,SLOT(setShowRulers(bool)));
	connect(ui->actionCompileAndRender,SIGNAL(triggered()),this,SLOT(compileAndRender()));
	connect(ui->actionPreferences,SIGNAL(triggered()),this,SLOT(showPreferences()));
	connect(ui->actionExportAsciiSTL,SIGNAL(triggered()),this,SLOT(exportAsciiSTL()));
	connect(ui->actionExportOFF,SIGNAL(triggered()),this,SLOT(exportOFF()));
	connect(ui->actionExportImage,SIGNAL(triggered()),this,SLOT(grabFrameBuffer()));
	connect(ui->actionShowEditor,SIGNAL(triggered(bool)),ui->tabWidget,SLOT(setVisible(bool)));
	connect(ui->actionShowConsole,SIGNAL(triggered(bool)),ui->plainTextEdit,SLOT(setVisible(bool)));
	connect(ui->actionShowProjects,SIGNAL(triggered(bool)),ui->treeView,SLOT(setVisible(bool)));
	connect(ui->actionSetViewport,SIGNAL(triggered()),this,SLOT(setDefaultViewport()));
	connect(ui->actionDefaultView,SIGNAL(triggered()),this,SLOT(getDefaultViewport()));

	connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabChanged(int)));
}

void MainWindow::grabFrameBuffer()
{
	QImage image = ui->view->grabFrameBuffer();
	QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
					QString(), tr("PNG Files (*.png)"));
	image.save(fn);
}

void MainWindow::exportAsciiSTL()
{
	if(primitive) {
		QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
					QString(), tr("STL Files (*.stl);;All Files (*)"));
		CGALExport exp;
		exp.exportAsciiSTL(primitive,fn,true);
	}
}

void MainWindow::exportOFF()
{
	if(primitive) {
		QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
					QString(), tr("OFF Files (*.stl);;All Files (*)"));
		CGALExport exp;
		exp.exportOFF(primitive,fn);
	}
}

void MainWindow::showPreferences()
{
	if(!preferencesDialog)
		preferencesDialog = new PreferencesDialog(this);

	preferencesDialog->show();
}

void MainWindow::disableRulers(bool checked)
{
	if(!checked)
		ui->actionShowRulers->setChecked(checked);

	ui->view->setShowRulers(ui->actionShowRulers->isChecked());
	ui->actionShowRulers->setEnabled(checked);
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

void MainWindow::setupEditor(CodeEditor* editor)
{
	disableActions(editor);

	connect(editor->document(), SIGNAL(modificationChanged(bool)),ui->actionSave, SLOT(setEnabled(bool)));
	connect(editor->document(), SIGNAL(modificationChanged(bool)),ui->actionSaveAll, SLOT(setEnabled(bool)));
	connect(editor->document(), SIGNAL(undoAvailable(bool)),ui->actionUndo, SLOT(setEnabled(bool)));
	connect(editor->document(), SIGNAL(redoAvailable(bool)),ui->actionRedo, SLOT(setEnabled(bool)));
	connect(editor,SIGNAL(copyAvailable(bool)), ui->actionCut, SLOT(setEnabled(bool)));
	connect(editor,SIGNAL(copyAvailable(bool)), ui->actionCopy, SLOT(setEnabled(bool)));
	connect(editor,SIGNAL(fileNameChanged(QString)),this,SLOT(setTabTitle(QString)));
}

void MainWindow::setupConsole()
{
	QTextEdit* c=(QTextEdit*)ui->plainTextEdit;

	QFont font;
	font.setFamily("Courier");
	font.setFixedPitch(true);
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
	if(maybeSave(true)) {
		savePreferences();
		e->accept();
	} else {
		e->ignore();
	}
}

bool MainWindow::loadFile(const QString& f)
{
	return currentEditor()->loadFile(f);
}

bool MainWindow::maybeSave(bool closing)
{
	if(!currentEditor()->document()->isModified())
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
	CodeEditor* e = new CodeEditor(this);
	int i=ui->tabWidget->addTab(e,"[New]");
	ui->tabWidget->setCurrentIndex(i);

	setupEditor(e);
}

bool MainWindow::saveFile()
{
	return currentEditor()->saveFile();
}

bool MainWindow::saveAsFile()
{
	return currentEditor()->saveAsFile();
}

bool MainWindow::saveAllFiles()
{
	//TODO
}

void MainWindow::openFile()
{
	currentEditor()->openFile();
}

void MainWindow::setTabTitle(const QString& fileName)
{
	QWidget* editor=qobject_cast<QWidget*>(QObject::sender());
	QString n=QFileInfo(fileName).fileName();
	int i=ui->tabWidget->indexOf(editor);
	ui->tabWidget->setTabText(i,n);
}

CodeEditor* MainWindow::currentEditor()
{
	return qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
}

void MainWindow::compileAndRender()
{
	//Stop the syntax highlighter to prevent a crash
	//It will start again automatically.
	CodeEditor* e=currentEditor();
	e->stopHighlighting();

	if(maybeSave(false)) {
		worker->evaluate(e->getFileName(),false);
		ui->actionCompileAndRender->setEnabled(false);
	}
}

void MainWindow::evaluationDone(CGALPrimitive* n)
{
	if(n) {
		primitive=n;
		CGALRenderer* r = new CGALRenderer(*n);
		ui->view->setRenderer(r);
	}
	ui->actionCompileAndRender->setEnabled(true);
}

void MainWindow::undo()
{
	currentEditor()->undo();
}

void MainWindow::redo()
{
	currentEditor()->redo();
}

void MainWindow::cut()
{
	currentEditor()->cut();
}

void MainWindow::copy()
{
	currentEditor()->copy();
}

void MainWindow::paste()
{
	currentEditor()->paste();
}

void MainWindow::disableActions(CodeEditor* editor)
{
	ui->actionSave->setEnabled(false);
	ui->actionSaveAll->setEnabled(false);
	ui->actionUndo->setEnabled(editor->document()->isUndoAvailable());
	ui->actionRedo->setEnabled(editor->document()->isRedoAvailable());
	ui->actionCut->setEnabled(false);
	ui->actionCopy->setEnabled(false);
}

void MainWindow::tabChanged(int i)
{
	//block signals from all the other tabs except
	//the new one
	for(int j=0; j<ui->tabWidget->count(); j++)
		ui->tabWidget->widget(j)->blockSignals(true);
	ui->tabWidget->widget(i)->blockSignals(false);

	disableActions(currentEditor());
}
