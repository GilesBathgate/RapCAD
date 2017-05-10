/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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
#include <QMimeData>
#include <QClipboard>
#include <QScrollBar>
#include <QMessageBox>
#include <QDesktopServices>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "renderer.h"
#include "preferences.h"
#include "saveitemsdialog.h"
#include "printconsole.h"
#include "aboutdialog.h"
#include "builtincreator.h"
#include "stringify.h"
#include "cachemanager.h"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	if(!QIcon::hasThemeIcon("document-open")) {
		QIcon::setThemeName("gnome");
	}

	ui->setupUi(this);
	QIcon rapcadIcon(":/icons/rapcad-16x16.png");
	this->setWindowIcon(rapcadIcon);
	setupLayout();
	setupActions();
	setupExportActions();
	setupViewActions();
	setupEditor(ui->scriptEditor);
	setupTreeview();
	setupTabs(ui->tabWidget);
	setupConsole();
	preferencesDialog=NULL;
	loadPreferences();

	//Make project treeview hidden until its useful.
	ui->treeView->setVisible(false);
	ui->actionShowProjects->setChecked(false);
	ui->actionShowProjects->setEnabled(false);

	ui->mainToolBar->setContextMenuPolicy(Qt::ActionsContextMenu);
}

MainWindow::~MainWindow()
{
	delete console;
	delete output;
	delete worker;
	delete interact;
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
	p->setCacheEnabled(ui->actionEnableCaches->isChecked());
	p->setWindowPosition(pos());
	p->setWindowSize(size());
}

void MainWindow::setDefaultViewport()
{
	Preferences* p=Preferences::getInstance();
	float rx,ry,rz,x,z,d;
	ui->view->getViewport(rx,ry,rz,x,z,d);
	p->setDefaultRotationX(rx);
	p->setDefaultRotationY(ry);
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
	ui->console->setVisible(showConsole);

	bool showProjects=p->getShowProjects();
	ui->actionShowProjects->setChecked(showProjects);
	ui->treeView->setVisible(showProjects);

	bool b=p->getCacheEnabled();
	ui->actionEnableCaches->setChecked(b);
	enableCaches(b);

	move(p->getWindowPosition());
	resize(p->getWindowSize());

	getDefaultViewport();
}

void MainWindow::getDefaultViewport()
{
	Preferences* p=Preferences::getInstance();
	float rx,ry,rz,x,z,d;
	rx=p->getDefaultRotationX();
	ry=p->getDefaultRotationY();
	rz=p->getDefaultRotationZ();
	x=p->getDefaultX();
	z=p->getDefaultZ();
	d=p->getDefaultDistance();
	ui->view->setViewport(rx,ry,rz,x,z,d);
}

void MainWindow::setupActions()
{
	connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(newFile()));
	connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(openFile()));
	connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(saveFile()));
	connect(ui->actionSaveAll,SIGNAL(triggered()),this,SLOT(saveAllFiles()));
	connect(ui->actionSaveAs,SIGNAL(triggered()),this,SLOT(saveAsFile()));
	connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(closeFile()));
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
	connect(ui->actionGenerateGcode,SIGNAL(triggered()),this,SLOT(compileAndGenerate()));
	connect(ui->actionPreferences,SIGNAL(triggered()),this,SLOT(showPreferences()));


	connect(ui->actionExportImage,SIGNAL(triggered()),this,SLOT(grabFrameBuffer()));
	connect(ui->actionShowEditor,SIGNAL(triggered(bool)),ui->tabWidget,SLOT(setVisible(bool)));
	connect(ui->actionShowConsole,SIGNAL(triggered(bool)),ui->console,SLOT(setVisible(bool)));
	connect(ui->actionShowProjects,SIGNAL(triggered(bool)),ui->treeView,SLOT(setVisible(bool)));
	connect(ui->actionSetViewport,SIGNAL(triggered()),this,SLOT(setDefaultViewport()));
	connect(ui->actionDefaultView,SIGNAL(triggered()),this,SLOT(getDefaultViewport()));

	connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabChanged(int)));

	connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(showAbout()));
	connect(ui->actionAboutQt,SIGNAL(triggered()),this,SLOT(showAboutQt()));
	connect(ui->actionShowBuiltins,SIGNAL(triggered()),this,SLOT(showBuiltins()));

	connect(ui->actionUserGuide,SIGNAL(triggered()),this,SLOT(showUserGuide()));

	connect(ui->actionFlushCaches,SIGNAL(triggered()),this,SLOT(flushCaches()));
	connect(ui->actionEnableCaches,SIGNAL(triggered(bool)),this,SLOT(enableCaches(bool)));

}

void MainWindow::setupExportActions()
{
	QSignalMapper* signalMapper = new QSignalMapper(this);
	signalMapper->setMapping(ui->actionExportVRML,"wrl");
	signalMapper->setMapping(ui->actionExportOBJ,"obj");
	signalMapper->setMapping(ui->actionExportAsciiSTL,"stl");
	signalMapper->setMapping(ui->actionExportAMF,"amf");
	signalMapper->setMapping(ui->actionExport3MF,"3mf");
	signalMapper->setMapping(ui->actionExportOFF,"stl");
	signalMapper->setMapping(ui->actionExportCSG,"csg");
	signalMapper->setMapping(ui->actionExportNEF,"nef");

	connect(ui->actionExportAsciiSTL,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionExportVRML,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionExportOBJ,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionExportOFF,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionExportAMF,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionExport3MF,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionExportCSG,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionExportNEF,SIGNAL(triggered()),signalMapper,SLOT(map()));

	connect(signalMapper,SIGNAL(mapped(QString)),this,SLOT(exportFile(QString)));
}

void MainWindow::setupViewActions()
{
	QSignalMapper* signalMapper = new QSignalMapper(this);
	signalMapper->setMapping(ui->actionTop,GLView::Top);
	signalMapper->setMapping(ui->actionBottom,GLView::Bottom);
	signalMapper->setMapping(ui->actionLeft,GLView::Left);
	signalMapper->setMapping(ui->actionRight,GLView::Right);
	signalMapper->setMapping(ui->actionBack,GLView::Back);
	signalMapper->setMapping(ui->actionFront,GLView::Front);

	connect(ui->actionTop,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionBottom,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionLeft,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionRight,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionBack,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionFront,SIGNAL(triggered()),signalMapper,SLOT(map()));

	connect(signalMapper,SIGNAL(mapped(int)),ui->view,SLOT(changeViewport(int)));

}

void MainWindow::grabFrameBuffer()
{
	QImage image = ui->view->grabFramebuffer();
	QString fn = QFileDialog::getSaveFileName(this, tr("Export..."),
				 QString(), tr("PNG Files (*.png)"));
	image.save(fn);
}

void MainWindow::exportFile(QString type)
{
	if(worker->resultAvailable()) {

		QFileInfo fileInfo(currentEditor()->getFileName());

		QString ext="."+type.toLower();
		QString filter=tr("%1 Files (*%2);;All Files (*)").arg(type.toUpper()).arg(ext);
		QString suggestedName=fileInfo.completeBaseName()+ext;
		QString suggestedLocation=fileInfo.absoluteDir().filePath(suggestedName);

		QString fileName=QFileDialog::getSaveFileName(this,tr("Export..."),suggestedLocation,filter);

		fileInfo=QFileInfo(fileName);
		if(fileInfo.suffix()=="")
			fileName.append(ext);

		worker->exportResult(fileName);
	} else {
		QMessageBox::information(this,tr("Export"),
			tr("You have to compile the script before you can export"));
	}
}

void MainWindow::showPreferences()
{
	if(!preferencesDialog) {
		preferencesDialog = new PreferencesDialog(this);
		connect(preferencesDialog,SIGNAL(preferencesUpdated()),ui->view,SLOT(preferencesUpdated()));
	}

	preferencesDialog->show();
}

void MainWindow::disableRulers(bool checked)
{
	if(!checked)
		ui->actionShowRulers->setChecked(checked);

	ui->view->setShowRulers(ui->actionShowRulers->isChecked());
	ui->actionShowRulers->setEnabled(checked);
}

void MainWindow::enableCaches(bool b)
{
	CacheManager* cm=CacheManager::getInstance();
	if(b)
		cm->enableCaches();
	else
		cm->disableCaches();
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
	headers << tr("Projects");
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

void MainWindow::setupTabs(QTabWidget* tabWidget)
{
	tabWidget->setTabsClosable(true);
	connect(tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(closeFile(int)));
}

void MainWindow::setupConsole()
{
	Console* c=ui->console;

	QFont font;
	font.setFamily("Courier");
	font.setFixedPitch(true);
	font.setPointSize(8);
	c->setFont(font);

	console=new TextEditIODevice(reinterpret_cast<QTextEdit*>(c),this);
	output=new QTextStream(console);
	worker=new BackgroundWorker(*output);
	connect(worker,SIGNAL(done()),this,SLOT(evaluationDone()));

	interact=new Interactive(*output);
	c->setPrompt(interact->getPrompt());
	connect(c,SIGNAL(execCommand(QString)),interact,SLOT(execCommand(QString)));
}

void MainWindow::clipboardDataChanged()
{
	if(const QMimeData* md = QApplication::clipboard()->mimeData())
		ui->actionPaste->setEnabled(md->hasText());
}


void MainWindow::closeEvent(QCloseEvent* e)
{
	if(maybeSave(false)) {
		savePreferences();
		e->accept();
	} else {
		e->ignore();
	}
}

void MainWindow::loadFiles(const QStringList& filenames)
{
	int i=0;
	foreach(QString file, filenames) {
		CodeEditor* e=currentEditor();
		if(e->getFileName().isEmpty()) {
			e->loadFile(file);
		} else {
			e = new CodeEditor(this);
			if(e->loadFile(file)) {
				i=ui->tabWidget->addTab(e,file);
				setupEditor(e);
			} else {
				delete e;
			}
		}
	}
	ui->tabWidget->setCurrentIndex(i);
}

bool MainWindow::maybeSave(bool compiling)
{
	bool modified=false;
	QList<QString> files;
	for(int i=0; i<ui->tabWidget->count(); i++) {
		CodeEditor* c=qobject_cast<CodeEditor*>(ui->tabWidget->widget(i));
		if(c->document()->isModified()) {
			files.append(c->getFileName());
			modified=true;
		}
	}
	if(!modified) return true;

	Preferences* p=Preferences::getInstance();

	if(compiling && p->getAutoSaveOnCompile()) {
		return saveSelectedFiles(files);
	}

	SaveItemsDialog s(this,compiling,files);

	if(s.exec()==QDialog::Accepted) {
		bool autoSave = s.getAutoSaveOnCompile();
		p->setAutoSaveOnCompile(autoSave);

		QList<QString> files=s.getItemsToSave();
		return saveSelectedFiles(files);
	}

	return false;
}

void MainWindow::newFile()
{
	CodeEditor* e = new CodeEditor(this);
	int i=ui->tabWidget->addTab(e,tr("[New]"));
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

bool MainWindow::closeFile()
{
	return closeFile(ui->tabWidget->currentIndex());
}

bool MainWindow::closeFile(int i)
{
	bool result=false;
	CodeEditor* c=getEditor(i);
	if(c->document()->isModified()) {
		QList<QString> files;
		files.append(c->getFileName());
		SaveItemsDialog s(this,false,files);
		if(s.exec()==QDialog::Accepted) {
			files=s.getItemsToSave();
			result=saveSelectedFiles(files);
		} else {
			return false;
		}
	}

	if(ui->tabWidget->count()<=1)
		newFile();

	ui->tabWidget->removeTab(i);
	delete c;

	return result;
}

bool MainWindow::saveAllFiles()
{
	QList<QString> all;
	for(int i=0; i<ui->tabWidget->count(); i++) {
		CodeEditor* c=qobject_cast<CodeEditor*>(ui->tabWidget->widget(i));
		if(c->document()->isModified()) {
			QString file=c->getFileName();
			all.append(file);
		}
	}
	return saveSelectedFiles(all);
}

bool MainWindow::saveSelectedFiles(QList<QString> files)
{
	bool result=true;
	for(int i=0; i<ui->tabWidget->count(); i++) {
		CodeEditor* c=qobject_cast<CodeEditor*>(ui->tabWidget->widget(i));
		QString file=c->getFileName();
		if(files.contains(file))
			if(!c->saveFile())
				result=false;
	}
	return result;
}

void MainWindow::openFile()
{
	if(!currentEditor()->getFileName().isEmpty())
		newFile();

	//current editor could now be different
	if(!currentEditor()->openFile())
		closeFile();
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

CodeEditor* MainWindow::getEditor(int i)
{
	return qobject_cast<CodeEditor*>(ui->tabWidget->widget(i));
}

void MainWindow::compileAndRender()
{
	compileOrGenerate(false);
}

void MainWindow::compileAndGenerate()
{
	compileOrGenerate(true);
}

void MainWindow::compileOrGenerate(bool generate)
{
	if(maybeSave(true)) {
		CodeEditor* e=currentEditor();
		QString file=e->getFileName();
		if(!file.isEmpty()) {
			ui->view->setCompiling(!generate);
			worker->setup(file,"",false,generate);

			//Stop the syntax highlighter to prevent a crash
			e->stopHighlighting();
			worker->evaluate();
			ui->actionCompileAndRender->setEnabled(false);
			ui->actionGenerateGcode->setEnabled(false);
		}
	}
}

void MainWindow::evaluationDone()
{
	if(worker->resultAvailable()) {
		Renderer* r = worker->getRenderer();
		ui->view->setRenderer(r);
		worker->resultAccepted();
	}
	ui->actionCompileAndRender->setEnabled(true);
	ui->actionGenerateGcode->setEnabled(true);
	ui->view->setCompiling(false);

	ui->console->displayPrompt();
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

void MainWindow::print()
{
	PrintConsole p;
	p.exec();
}

void MainWindow::showAbout()
{
	AboutDialog about;
	about.exec();
}

void MainWindow::showAboutQt()
{
	QMessageBox::aboutQt(this);
}

void MainWindow::showBuiltins()
{
	CodeEditor* e = new CodeEditor(this);
	int i=ui->tabWidget->addTab(e,tr("Built In"));
	ui->tabWidget->setCurrentIndex(i);

	connect(e,SIGNAL(copyAvailable(bool)), ui->actionCopy, SLOT(setEnabled(bool)));

	QTextEdit* c=(QTextEdit*)e;
	QIODevice* t=new TextEditIODevice(c,this);
	QTextStream out(t);
	Reporter r(out);
	BuiltinCreator* b = BuiltinCreator::getInstance(&r);
	b->generateDocs();
	out.flush();
	delete t;

	//Scroll back to top
	QScrollBar* sb = e->verticalScrollBar();
	sb->setValue(sb->minimum());

	//Make sure the user cannot modify or save this document
	e->setReadOnly(true);
	e->document()->setModified(false);
}

void MainWindow::showUserGuide()
{
#ifdef DOCDIR
	QDir docdir(QSTRINGIFY(DOCDIR));
#else
	QDir docdir(QCoreApplication::applicationDirPath());
#endif

	QFileInfo guide(docdir,"user_guide.html");
	QUrl url;
	if(guide.exists())
		url=QUrl(QString("file:///%1").arg(guide.absoluteFilePath()));
	else
		url=QUrl("https://github.com/GilesBathgate/RapCAD/blob/master/doc/user_guide.asciidoc");

	QDesktopServices::openUrl(url);
}

void MainWindow::flushCaches()
{
	CacheManager* m=CacheManager::getInstance();
	m->flushCaches();
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