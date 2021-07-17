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

#include <QFileDialog>
#include <QMimeData>
#include <QClipboard>
#include <QScrollBar>
#include <QMessageBox>
#include <QDesktopServices>
#include <QProcess>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "renderer.h"
#include "preferences.h"
#include "saveitemsdialog.h"
#include "printconsole.h"
#include "builtincreator.h"
#include "stringify.h"
#include "cachemanager.h"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	projectModel(nullptr),
	console(nullptr),
	output(nullptr),
	reporter(nullptr),
	worker(nullptr),
	interact(nullptr),
	aboutDialog(nullptr),
	preferencesDialog(nullptr)
{
	if(!QIcon::hasThemeIcon("document-open")) {
		QIcon::setThemeName("gnome");
	}

	ui->setupUi(this);
	QIcon rapcadIcon(":/icons/rapcad-16x16.png");
	setWindowIcon(rapcadIcon);
	setupLayout();
	setupActions();
	setupExportActions();
	setupViewActions();
	setupTreeview();
	setupTabs(ui->tabWidget);
	setupConsole();
	setupEditor(ui->scriptEditor);

	ui->searchWidget->setVisible(false);
	ui->mainToolBar->setContextMenuPolicy(Qt::ActionsContextMenu);

	loadPreferences();

	//Make project treeview actions disabled until its useful.
	ui->actionNewProject->setEnabled(false);
	ui->actionShowProjects->setChecked(false);
	ui->actionShowProjects->setEnabled(false);

}

MainWindow::~MainWindow()
{
	deleteTempFiles();
	delete projectModel;
	delete console;
	delete output;
	delete reporter;
	delete worker;
	delete interact;
	delete aboutDialog;
	delete preferencesDialog;
	delete ui;
}

void MainWindow::deleteTempFiles()
{
	for(auto* file: temporyFiles) {
		file->close();
		delete file;
	}
}

void MainWindow::savePreferences()
{
	Preferences& p=Preferences::getInstance();
	p.setShowRulers(ui->actionShowRulers->isChecked());
	p.setShowAxes(ui->actionShowAxes->isChecked());
	p.setShowEdges(ui->actionShowEdges->isChecked());
	p.setSkeleton(ui->actionSkeleton->isChecked());
	p.setShowBase(ui->actionShowBase->isChecked());
	p.setShowPrintArea(ui->actionShowPrintArea->isChecked());
	p.setShowEditor(ui->actionShowEditor->isChecked());
	p.setShowConsole(ui->actionShowConsole->isChecked());
	p.setShowProjects(ui->actionShowProjects->isChecked());
	p.setCacheEnabled(ui->actionEnableCaches->isChecked());
	p.setWindowPosition(pos());
	p.setWindowSize(size());
}

void MainWindow::setDefaultViewport()
{
	Preferences& p=Preferences::getInstance();
	float rx;
	float ry;
	float rz;
	float x;
	float z;
	float d;
	ui->view->getViewport(rx,ry,rz,x,z,d);
	p.setDefaultRotationX(rx);
	p.setDefaultRotationY(ry);
	p.setDefaultRotationZ(rz);
	p.setDefaultX(x);
	p.setDefaultZ(z);
	p.setDefaultDistance(d);
}

void MainWindow::loadPreferences()
{
	Preferences& p=Preferences::getInstance();

	bool showRulers=p.getShowRulers();
	ui->actionShowRulers->setChecked(showRulers);
	ui->view->setShowRulers(showRulers);

	bool showAxes=p.getShowAxes();
	ui->actionShowAxes->setChecked(showAxes);
	ui->view->setShowAxes(showAxes);
	disableRulers(showAxes);

	bool showEdges=p.getShowEdges();
	ui->actionShowEdges->setChecked(showEdges);
	ui->view->setShowEdges(showEdges);

	bool showSkeleton=p.getSkeleton();
	ui->actionSkeleton->setChecked(showSkeleton);
	ui->view->setSkeleton(showSkeleton);

	bool showBase=p.getShowBase();
	ui->actionShowBase->setChecked(showBase);
	ui->view->setShowBase(showBase);

	bool showPrintArea=p.getShowPrintArea();
	ui->actionShowPrintArea->setChecked(showPrintArea);
	ui->view->setShowPrintArea(showPrintArea);

	bool showEditor=p.getShowEditor();
	ui->actionShowEditor->setChecked(showEditor);
	ui->tabWidget->setVisible(showEditor);

	bool showConsole=p.getShowConsole();
	ui->actionShowConsole->setChecked(showConsole);
	ui->console->setVisible(showConsole);

	bool showProjects=p.getShowProjects();
	ui->actionShowProjects->setChecked(showProjects);
	ui->treeView->setVisible(showProjects);

	bool b=p.getCacheEnabled();
	ui->actionEnableCaches->setChecked(b);
	enableCaches(b);

	move(p.getWindowPosition());
	resize(p.getWindowSize());

	getDefaultViewport();

	QPointF o=p.getPrintOrigin();
	ui->view->setPrintOrigin(o.x(), o.y());

	QVector3D v=p.getPrintVolume();
	ui->view->setPrintVolume(v.x(), v.y(), v.z());

	ui->view->setBedAppearance(p.getPrintBedAppearance());

	ui->actionGenerateGcode->setVisible(p.getShowGCODEButton());

}

void MainWindow::getDefaultViewport() const
{
	Preferences& p=Preferences::getInstance();
	float rx=p.getDefaultRotationX();
	float ry=p.getDefaultRotationY();
	float rz=p.getDefaultRotationZ();
	float x=p.getDefaultX();
	float z=p.getDefaultZ();
	float d=p.getDefaultDistance();
	ui->view->setViewport(rx,ry,rz,x,z,d);
}

void MainWindow::setupActions()
{
	connect(ui->actionNew,&QAction::triggered,this,&MainWindow::newFile);
	connect(ui->actionNewProject,&QAction::triggered,this,&MainWindow::newProject);
	connect(ui->actionOpen,&QAction::triggered,this,&MainWindow::openFile);
	connect(ui->actionSave,&QAction::triggered,this,&MainWindow::saveFile);
	connect(ui->actionSaveAll,&QAction::triggered,this,&MainWindow::saveAllFiles);
	connect(ui->actionSaveAs,&QAction::triggered,this,&MainWindow::saveAsFile);
	connect(ui->actionClose,&QAction::triggered,this,&MainWindow::closeCurrentFile);
	connect(ui->actionQuit,&QAction::triggered,this,&MainWindow::close);
	connect(ui->actionUndo,&QAction::triggered,this,&MainWindow::undo);
	connect(ui->actionRedo,&QAction::triggered,this,&MainWindow::redo);
	connect(ui->actionIncreaseIndent,&QAction::triggered,this,&MainWindow::increaseSelectionIndent);
	connect(ui->actionDecreaseIndent,&QAction::triggered,this,&MainWindow::decreaseSelectionIndent);
	connect(ui->actionCut,&QAction::triggered,this,&MainWindow::cut);
	connect(ui->actionCopy,&QAction::triggered,this,&MainWindow::copy);
	connect(ui->actionPaste,&QAction::triggered,this,&MainWindow::paste);
	connect(ui->actionFind,&QAction::triggered,ui->searchWidget,&SearchWidget::showSearch);
	connect(ui->actionFindAndReplace,&QAction::triggered,ui->searchWidget,&SearchWidget::showReplace);

	clipboardDataChanged();
	connect(QApplication::clipboard(),&QClipboard::dataChanged,this,&MainWindow::clipboardDataChanged);

	//Make sure when axis is toggled rulers are disabled but not vice versa
	connect(ui->actionShowAxes,&QAction::triggered,this,&MainWindow::disableRulers);
	connect(ui->actionShowEdges,&QAction::triggered,ui->view,&GLView::setShowEdges);
	connect(ui->actionSkeleton,&QAction::triggered,ui->view,&GLView::setSkeleton);
	connect(ui->actionShowAxes,&QAction::triggered,ui->view,&GLView::setShowAxes);
	connect(ui->actionShowBase,&QAction::triggered,ui->view,&GLView::setShowBase);
	connect(ui->actionShowPrintArea,&QAction::triggered,ui->view,&GLView::setShowPrintArea);
	connect(ui->actionShowRulers,&QAction::triggered,ui->view,&GLView::setShowRulers);
	connect(ui->actionCompileAndRender,&QAction::triggered,this,&MainWindow::compileAndRender);
	connect(ui->actionGenerateGcode,&QAction::triggered,this,&MainWindow::compileAndGenerate);
	connect(ui->actionPreferences,&QAction::triggered,this,&MainWindow::showPreferences);

	connect(ui->actionExportImage,&QAction::triggered,this,&MainWindow::grabFrameBuffer);
	connect(ui->actionShowEditor,&QAction::triggered,ui->tabWidget,&QTabWidget::setVisible);
	connect(ui->actionShowConsole,&QAction::triggered,ui->console,&Console::setVisible);
	connect(ui->actionShowProjects,&QAction::triggered,ui->treeView,&QTreeView::setVisible);
	connect(ui->actionSetViewport,&QAction::triggered,this,&MainWindow::setDefaultViewport);
	connect(ui->actionDefaultView,&QAction::triggered,this,&MainWindow::getDefaultViewport);

	connect(ui->tabWidget,&QTabWidget::currentChanged,this,&MainWindow::tabChanged);

	connect(ui->actionAbout,&QAction::triggered,this,&MainWindow::showAbout);
	connect(ui->actionAboutQt,&QAction::triggered,this,&MainWindow::showAboutQt);
	connect(ui->actionShowBuiltins,&QAction::triggered,this,&MainWindow::showBuiltins);

	connect(ui->actionUserGuide,&QAction::triggered,this,&MainWindow::showUserGuide);

	connect(ui->actionFlushCaches,&QAction::triggered,this,&MainWindow::flushCaches);
	connect(ui->actionEnableCaches,&QAction::triggered,this,&MainWindow::enableCaches);

	connect(ui->actionSendToCAM,&QAction::triggered,this,&MainWindow::sendToCAM);

}

void MainWindow::setupExportActions()
{
	auto* signalMapper = new QSignalMapper(this);
	signalMapper->setMapping(ui->actionExportVRML,"wrl");
	signalMapper->setMapping(ui->actionExportOBJ,"obj");
	signalMapper->setMapping(ui->actionExportAsciiSTL,"stl");
	signalMapper->setMapping(ui->actionExportAMF,"amf");
	signalMapper->setMapping(ui->actionExport3MF,"3mf");
	signalMapper->setMapping(ui->actionExportOFF,"stl");
	signalMapper->setMapping(ui->actionExportCSG,"csg");
	signalMapper->setMapping(ui->actionExportNEF,"nef");
	signalMapper->setMapping(ui->actionExportSVG,"svg");

	connect(ui->actionExportAsciiSTL,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionExportVRML,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionExportOBJ,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionExportOFF,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionExportAMF,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionExport3MF,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionExportCSG,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionExportNEF,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionExportSVG,SIGNAL(triggered()),signalMapper,SLOT(map()));

	connect(signalMapper,SIGNAL(mapped(QString)),this,SLOT(exportFile(QString)));
}

void MainWindow::setupViewActions()
{
	auto* signalMapper = new QSignalMapper(this);
	signalMapper->setMapping(ui->actionTop,static_cast<int>(ViewDirections::Top));
	signalMapper->setMapping(ui->actionBottom,static_cast<int>(ViewDirections::Bottom));
	signalMapper->setMapping(ui->actionNorth,static_cast<int>(ViewDirections::North));
	signalMapper->setMapping(ui->actionSouth,static_cast<int>(ViewDirections::South));
	signalMapper->setMapping(ui->actionWest,static_cast<int>(ViewDirections::West));
	signalMapper->setMapping(ui->actionEast,static_cast<int>(ViewDirections::East));


	connect(ui->actionTop,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionBottom,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionNorth,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionSouth,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionWest,SIGNAL(triggered()),signalMapper,SLOT(map()));
	connect(ui->actionEast,SIGNAL(triggered()),signalMapper,SLOT(map()));

	connect(signalMapper,SIGNAL(mapped(int)),ui->view,SLOT(changeViewport(int)));

}

void MainWindow::grabFrameBuffer()
{
	QImage image=ui->view->grabFramebuffer();
	QString fileName=MainWindow::getSaveFileName(this, tr("Export..."),
				 QString(), tr("PNG Files (*.png)"), "png");
	if(fileName.isEmpty())
		return;
	image.save(fileName);
}

QString MainWindow::getSaveFileName(QWidget* parent,const QString& caption,const QString& dir,const QString& filter,const QString& suffix)
{
	QFileDialog dialog(parent,caption,dir,filter);
	dialog.setDefaultSuffix(suffix);
	dialog.setOption(QFileDialog::DontConfirmOverwrite,true);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	while (dialog.exec() == QDialog::Accepted) {
		QString fileName=dialog.selectedFiles().first();
		QFileInfo info(fileName);
		if(!info.exists())
			return fileName;

		auto result=QMessageBox::warning(parent,caption,
			tr("A file named \"%1\" already exists. Do you want to replace it?").arg(info.fileName()),
			QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel,QMessageBox::No);
		if(result==QMessageBox::Cancel) break;
		if(result==QMessageBox::Yes) return fileName;
	}
	return QString();
}

void MainWindow::exportFile(const QString& type)
{
	if(!worker->resultAvailable()) {
		QMessageBox::information(this,tr("Export"),
			tr("You have to compile the script before you can export"));
		return;
	}

	QFileInfo fileInfo(currentEditor()->getFileName());

	QString ext=QString(".%1").arg(type.toLower());
	QString filter=tr("%1 Files (*%2);;All Files (*)").arg(type.toUpper()).arg(ext);
	QString suggestedName=fileInfo.completeBaseName().append(ext);
	QString suggestedLocation=fileInfo.absoluteDir().filePath(suggestedName);

	QString fileName=MainWindow::getSaveFileName(this,tr("Export..."),suggestedLocation,filter,ext);
	if(fileName.isEmpty())
		return;

	worker->exportResult(fileName);

}

void MainWindow::showPreferences()
{
	if(!preferencesDialog) {
		preferencesDialog = new PreferencesDialog(this);
		connect(preferencesDialog,&PreferencesDialog::preferencesUpdated,this,&MainWindow::preferencesUpdated);
	}

	preferencesDialog->show();
}

void MainWindow::preferencesUpdated()
{
	Preferences& p=Preferences::getInstance();

	for(auto i=0; i<ui->tabWidget->count(); ++i) {
		auto* c=qobject_cast<CodeEditor*>(ui->tabWidget->widget(i));
		c->preferencesUpdated();
	}

	ui->actionGenerateGcode->setVisible(p.getShowGCODEButton());

	QPointF o=p.getPrintOrigin();
	ui->view->setPrintOrigin(o.x(),o.y());

	QVector3D v=p.getPrintVolume();
	ui->view->setPrintVolume(v.x(),v.y(),v.z());

	ui->view->setBedAppearance(p.getPrintBedAppearance());

	ui->view->preferencesUpdated();
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
	CacheManager& cm=CacheManager::getInstance();
	if(b)
		cm.enableCaches();
	else
		cm.disableCaches();
}

void MainWindow::setupLayout()
{
	//TODO there must be a better way than this
	QList<int> hSizes;
	QList<int> vSizes;
	hSizes << 160 << 540 << 300;
	ui->vSplitter->setSizes(hSizes);
	vSizes << 150 << 10;
	ui->hSplitter->setSizes(vSizes);
}

void MainWindow::setupTreeview()
{
	projectModel=new Project(this);
	ui->treeView->setModel(projectModel);
}

void MainWindow::newProject()
{
	ui->treeView->setVisible(true);

	QString dirName=QFileDialog::getExistingDirectory(this,tr("New Project..."));
	if(dirName.isEmpty())
		return;

	QDir directory(dirName);
	QString projectName=directory.dirName();
	projectModel->setName(projectName);
	projectModel->createDefaultItems();

}

void MainWindow::setupEditor(CodeEditor* editor)
{
	disableActions(editor);

	connect(editor->document(),&QTextDocument::modificationChanged,ui->actionSave,&QAction::setEnabled);
	connect(editor->document(),&QTextDocument::modificationChanged,ui->actionSaveAll,&QAction::setEnabled);
	connect(editor->document(),&QTextDocument::undoAvailable,ui->actionUndo,&QAction::setEnabled);
	connect(editor->document(),&QTextDocument::redoAvailable,ui->actionRedo,&QAction::setEnabled);
	connect(editor,&CodeEditor::copyAvailable,ui->actionCut,&QAction::setEnabled);
	connect(editor,&CodeEditor::copyAvailable,ui->actionCopy,&QAction::setEnabled);
	connect(editor,&CodeEditor::fileNameChanged,this,&MainWindow::setTabTitle);

	BuiltinCreator& b=BuiltinCreator::getInstance(*reporter);
	editor->setModuleNames(b.getModuleNames());
	ui->searchWidget->setTextEdit(editor);
}

void MainWindow::setupTabs(QTabWidget* tabWidget) const
{
	tabWidget->setTabsClosable(true);
	connect(tabWidget,&QTabWidget::tabCloseRequested,this,&MainWindow::closeFile);
}

void MainWindow::setupConsole()
{
	Console* c=ui->console;

	QFont font;
	font.setFamily("Courier");
	font.setFixedPitch(true);
	font.setPointSize(8);
	c->setFont(font);

	console=new TextEditIODevice(c,this);
	output=new QTextStream(console);
	reporter=new Reporter(*output);
	worker=new BackgroundWorker(*reporter);
	connect(worker,&BackgroundWorker::done,this,&MainWindow::evaluationDone);

	interact=new Interactive(*reporter);
	c->setPrompt(Interactive::getPrompt());
	connect(c,&Console::execCommand,interact,&Interactive::execCommand);
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
	for(const auto& file: filenames) {
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
	for(auto i=0; i<ui->tabWidget->count(); ++i) {
		auto* c=qobject_cast<CodeEditor*>(ui->tabWidget->widget(i));
		if(c->document()->isModified()) {
			files.append(c->getFileName());
			modified=true;
		}
	}
	if(!modified) return true;

	Preferences& p=Preferences::getInstance();

	if(compiling && p.getAutoSaveOnCompile()) {
		return saveSelectedFiles(files);
	}

	SaveItemsDialog s(this,compiling,files);

	if(s.exec()==QDialog::Accepted) {
		bool autoSave = s.getAutoSaveOnCompile();
		p.setAutoSaveOnCompile(autoSave);

		QList<QString> f=s.getItemsToSave();
		return saveSelectedFiles(f);
	}

	return false;
}

void MainWindow::newFile()
{
	auto* e = new CodeEditor(this);
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

bool MainWindow::closeCurrentFile()
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
	for(auto i=0; i<ui->tabWidget->count(); ++i) {
		auto* c=qobject_cast<CodeEditor*>(ui->tabWidget->widget(i));
		if(c->document()->isModified()) {
			QString file=c->getFileName();
			all.append(file);
		}
	}
	return saveSelectedFiles(all);
}

bool MainWindow::saveSelectedFiles(const QList<QString>& files)
{
	bool result=true;
	for(auto i=0; i<ui->tabWidget->count(); ++i) {
		auto* c=qobject_cast<CodeEditor*>(ui->tabWidget->widget(i));
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
		closeCurrentFile();
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
			worker->setup(file,"",generate);

			//Stop the syntax highlighter to prevent a crash
			CodeEditor::stopHighlighting();
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

void MainWindow::increaseSelectionIndent()
{
	currentEditor()->increaseSelectionIndent();
}

void MainWindow::decreaseSelectionIndent()
{
	currentEditor()->decreaseSelectionIndent();
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
	if(!aboutDialog)
		aboutDialog = new AboutDialog(this);

	aboutDialog->show();
}

void MainWindow::showAboutQt()
{
	QMessageBox::aboutQt(this);
}

void MainWindow::showBuiltins()
{
	auto* e = new CodeEditor(this);
	int i=ui->tabWidget->addTab(e,tr("Built In"));
	ui->tabWidget->setCurrentIndex(i);

	connect(e,&CodeEditor::copyAvailable,ui->actionCopy,&QAction::setEnabled);

	BuiltinCreator& b = BuiltinCreator::getInstance(*reporter);

	TextEditIODevice t(e,this);
	QTextStream out(&t);
	b.generateDocs(out);
	out.flush();

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
	CacheManager& m=CacheManager::getInstance();
	m.flushCaches();
}

void MainWindow::sendToCAM()
{
	const QString title=tr("Send to CAM");
	if(!worker->resultAvailable()) {
		QMessageBox::information(this,title, tr("You have to compile the script before you can export"));
		return;
	}

	Preferences& p=Preferences::getInstance();
	QString command = p.getLaunchCommand();
	if(command.isEmpty()) {
		QMessageBox::information(this,title, tr("No launch command set in preferences"));
		return;
	}

	QString fileTemplate=QDir::tempPath().append("/XXXXXX.").append("3mf");
	auto* file=new QTemporaryFile(fileTemplate);
	if(!file->open()) {
		QMessageBox::information(this,title, tr("Could not create tempory file"));
		delete file;
		return;
	}
	temporyFiles.append(file);

	QString fileName=file->fileName();
	worker->exportResult(fileName);
	QProcess::startDetached(command,QStringList()<<fileName);
}

void MainWindow::tabChanged(int i)
{
	//block signals from all the other tabs except
	//the new one
	QTabWidget* t=ui->tabWidget;
	for(auto j=0; j<t->count(); ++j)
		t->widget(j)->blockSignals(true);

	CodeEditor* c=getEditor(i);
	c->blockSignals(false);
	disableActions(c);
	ui->searchWidget->setTextEdit(c);
}
