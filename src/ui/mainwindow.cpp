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
#include "builtincreator.h"
#include "cachemanager.h"
#include "mainwindow.h"
#include "preferences.h"
#include "printconsole.h"
#include "renderer.h"
#include "saveitemsdialog.h"
#include "stringify.h"
#include "ui/preferencesdialog.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeData>
#include <QProcess>
#include <QScrollBar>
#include <QStyleFactory>
#include <contrib/qtcompat.h>

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	projectModel(nullptr),
	console(nullptr),
	output(nullptr),
	reporter(nullptr),
	worker(nullptr),
	interact(nullptr),
	aboutDialog(nullptr)
{
	setTheme();

	ui->setupUi(this);
	const QIcon rapcadIcon(":/icons/rapcad-16x16.png");
	setWindowIcon(rapcadIcon);
	setupLayout();
	setupActions();
	setupExportActions();
	setupViewActions();
	setupTreeview();
	setupTabs(ui->tabWidget);
	setupConsole();
	setupEditor(ui->scriptEditor);
	setupExamples();

	ui->searchWidget->setVisible(false);
	ui->mainToolBar->setContextMenuPolicy(Qt::ActionsContextMenu);

	loadPreferences();

	//Make project treeview actions disabled until its useful.
	ui->actionNewProject->setEnabled(false);
	ui->projectPage->setEnabled(false);
	ui->toolBox->setCurrentIndex(1);

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
	delete ui;
}

void MainWindow::deleteTempFiles()
{
	for(auto* file: std::as_const(temporyFiles)) {
		file->close();
		delete file;
	}
}

void MainWindow::setTheme()
{
	if(!QIcon::hasThemeIcon("document-open")) {
		QIcon::setThemeName("gnome");
	}

	auto& prefs=Preferences::getInstance();
	if(prefs.getDarkTheme()) {
		QApplication::setStyle(QStyleFactory::create("Fusion"));
		auto p = QApplication::palette();
		p.setColor(QPalette::Window, QColor(53, 53, 53));
		p.setColor(QPalette::WindowText, Qt::white);
		p.setColor(QPalette::Base, QColor(35, 35, 35));
		p.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
		p.setColor(QPalette::ToolTipBase, QColor(25, 25, 25));
		p.setColor(QPalette::ToolTipText, Qt::white);
		p.setColor(QPalette::Text, Qt::white);
		p.setColor(QPalette::Button, QColor(53, 53, 53));
		p.setColor(QPalette::ButtonText, Qt::white);
		p.setColor(QPalette::BrightText, Qt::red);
		p.setColor(QPalette::Link, QColor(42, 130, 218));
		p.setColor(QPalette::Highlight, QColorConstants::Svg::dodgerblue);
		p.setColor(QPalette::HighlightedText, QColor(35, 35, 35));
		p.setColor(QPalette::Active, QPalette::Button, QColor(53, 53, 53));
		p.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);
		p.setColor(QPalette::Disabled, QPalette::WindowText, Qt::darkGray);
		p.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
		p.setColor(QPalette::Disabled, QPalette::Light, QColor(53, 53, 53));
		QApplication::setPalette(p);
	}
}

void MainWindow::savePreferences()
{
	auto& p=Preferences::getInstance();
	p.setShowRulers(ui->actionShowRulers->isChecked());
	p.setShowAxes(ui->actionShowAxes->isChecked());
	p.setShowEdges(ui->actionShowEdges->isChecked());
	p.setSkeleton(ui->actionSkeleton->isChecked());
	p.setShowBase(ui->actionShowBase->isChecked());
	p.setShowPrintArea(ui->actionShowPrintArea->isChecked());
	p.setShowEditor(ui->actionShowEditor->isChecked());
	p.setShowConsole(ui->actionShowConsole->isChecked());
	p.setShowExplorer(ui->actionShowExplorer->isChecked());
	p.setCacheEnabled(ui->actionEnableCaches->isChecked());
	p.setWindowPosition(pos());
	p.setWindowSize(size());
}

void MainWindow::setDefaultCamera()
{
	auto& p=Preferences::getInstance();
	const Camera& c=ui->view->getCamera();
	p.setDefaultRotationX(c.getRotateX());
	p.setDefaultRotationY(c.getRotateY());
	p.setDefaultRotationZ(c.getRotateZ());
	p.setDefaultX(c.getPositionX());
	p.setDefaultZ(c.getPositionZ());
	p.setDefaultDistance(c.getPositionY());
}

void MainWindow::loadPreferences()
{
	auto& p=Preferences::getInstance();

	const bool showRulers=p.getShowRulers();
	ui->actionShowRulers->setChecked(showRulers);
	ui->view->setShowRulers(showRulers);

	const bool showAxes=p.getShowAxes();
	ui->actionShowAxes->setChecked(showAxes);
	ui->view->setShowAxes(showAxes);
	disableRulers(showAxes);

	const bool showEdges=p.getShowEdges();
	ui->actionShowEdges->setChecked(showEdges);
	ui->view->setShowEdges(showEdges);

	const bool showSkeleton=p.getSkeleton();
	ui->actionSkeleton->setChecked(showSkeleton);
	ui->view->setSkeleton(showSkeleton);

	const bool showBase=p.getShowBase();
	ui->actionShowBase->setChecked(showBase);
	ui->view->setShowBase(showBase);

	const bool showPrintArea=p.getShowPrintArea();
	ui->actionShowPrintArea->setChecked(showPrintArea);
	ui->view->setShowPrintArea(showPrintArea);

	const bool showEditor=p.getShowEditor();
	ui->actionShowEditor->setChecked(showEditor);
	ui->tabWidget->setVisible(showEditor);

	const bool showConsole=p.getShowConsole();
	ui->actionShowConsole->setChecked(showConsole);
	ui->console->setVisible(showConsole);

	const bool showExplorer=p.getShowExplorer();
	ui->actionShowExplorer->setChecked(showExplorer);
	ui->toolBox->setVisible(showExplorer);

	const bool b=p.getCacheEnabled();
	ui->actionEnableCaches->setChecked(b);
	enableCaches(b);

	move(p.getWindowPosition());
	resize(p.getWindowSize());

	getDefaultCamera();

	const QPoint& o=p.getPrintOrigin();
	ui->view->setPrintOrigin(o.x(), o.y());

	const QList<int>& v=p.getPrintVolume();
	ui->view->setPrintVolume(v.at(0),v.at(1),v.at(2));

	ui->view->setBedAppearance(p.getPrintBedAppearance());

	ui->actionGenerateGcode->setVisible(p.getShowGCODEButton());

}

void MainWindow::getDefaultCamera() const
{
	auto& p=Preferences::getInstance();
	const Camera c{
		p.getDefaultRotationX(),
		p.getDefaultRotationY(),
		p.getDefaultRotationZ(),
		p.getDefaultX(),
		p.getDefaultDistance(),
		p.getDefaultZ()
	};
	ui->view->setCamera(c);
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
	connect(ui->actionShowExplorer,&QAction::triggered,ui->toolBox,&QToolBox::setVisible);
	connect(ui->actionSetViewport,&QAction::triggered,this,&MainWindow::setDefaultCamera);
	connect(ui->actionDefaultView,&QAction::triggered,this,&MainWindow::getDefaultCamera);

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
	connect(ui->actionExportAsciiSTL,&QAction::triggered,this,[this](){exportFile("stl");});
	connect(ui->actionExportVRML,&QAction::triggered,this,[this](){exportFile("wrl");});
	connect(ui->actionExportOBJ,&QAction::triggered,this,[this](){exportFile("obj");});
	connect(ui->actionExportOFF,&QAction::triggered,this,[this](){exportFile("off");});
	connect(ui->actionExportAMF,&QAction::triggered,this,[this](){exportFile("amf");});
	connect(ui->actionExport3MF,&QAction::triggered,this,[this](){exportFile("3mf");});
	connect(ui->actionExportCSG,&QAction::triggered,this,[this](){exportFile("csg");});
	connect(ui->actionExportNEF,&QAction::triggered,this,[this](){exportFile("nef");});
	connect(ui->actionExportSVG,&QAction::triggered,this,[this](){exportFile("svg");});
}

void MainWindow::setupViewActions()
{
	connect(ui->actionTop,&QAction::triggered,this,[this](){ui->view->changeViewDirection(ViewDirections::Top);});
	connect(ui->actionBottom,&QAction::triggered,this,[this](){ui->view->changeViewDirection(ViewDirections::Bottom);});
	connect(ui->actionNorth,&QAction::triggered,this,[this](){ui->view->changeViewDirection(ViewDirections::North);});
	connect(ui->actionSouth,&QAction::triggered,this,[this](){ui->view->changeViewDirection(ViewDirections::South);});
	connect(ui->actionWest,&QAction::triggered,this,[this](){ui->view->changeViewDirection(ViewDirections::West);});
	connect(ui->actionEast,&QAction::triggered,this,[this](){ui->view->changeViewDirection(ViewDirections::East);});
}

void MainWindow::grabFrameBuffer()
{
	const QImage& image=ui->view->grabFramebuffer();
	const QString& fileName=MainWindow::getSaveFileName(this, tr("Export..."),
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
	while(dialog.exec() == QDialog::Accepted) {
		const auto& selected=dialog.selectedFiles();
		QString fileName=selected.first();
		const QFileInfo info(fileName);
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

	const QFileInfo fileInfo(currentEditor()->getFileName());

	const auto& ext=QString(".%1").arg(type.toLower());
	const auto& filter=tr("%1 Files (*%2);;All Files (*)").arg(type.toUpper(),ext);
	const auto& suggestedName=fileInfo.completeBaseName().append(ext);
	const auto& suggestedLocation=fileInfo.absoluteDir().filePath(suggestedName);

	const auto& fileName=MainWindow::getSaveFileName(this,tr("Export..."),suggestedLocation,filter,ext);
	if(fileName.isEmpty())
		return;

	worker->exportResult(fileName);

}

void MainWindow::showPreferences()
{
	PreferencesDialog preferencesDialog(this);
	preferencesDialog.exec();
	preferencesUpdated();
}

void MainWindow::preferencesUpdated()
{
	auto& p=Preferences::getInstance();

	for(auto i=0; i<ui->tabWidget->count(); ++i) {
		auto* c=qobject_cast<CodeEditor*>(ui->tabWidget->widget(i));
		c->preferencesUpdated();
	}

	ui->actionGenerateGcode->setVisible(p.getShowGCODEButton());

	const QPoint& o=p.getPrintOrigin();
	ui->view->setPrintOrigin(o.x(),o.y());

	const QList<int>& v=p.getPrintVolume();
	ui->view->setPrintVolume(v.at(0),v.at(1),v.at(2));

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
	auto& cm=CacheManager::getInstance();
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
	ui->toolBox->setVisible(true);

	const QString& dirName=QFileDialog::getExistingDirectory(this,tr("New Project..."));
	if(dirName.isEmpty())
		return;

	const QDir directory(dirName);
	const QString& projectName=directory.dirName();
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

	auto& b=BuiltinCreator::getInstance(*reporter);
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

void MainWindow::setupExamples()
{
	auto& b=BuiltinCreator::getInstance(*reporter);
	const QHash<QString,Module*>& modules=b.getModuleNames();
	for(Module* m : modules) {
		if(!m->isDeprecated()&&m->hasExample()) {
			auto* item=new QListWidgetItem(m->getFullName(),ui->examplesList);
			item->setData(Qt::UserRole,QVariant::fromValue(m));
		}
	}

	ui->examplesList->sortItems();
	connect(ui->examplesList,&QListView::doubleClicked,this,&MainWindow::examplesListClicked);
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

	auto& p=Preferences::getInstance();

	if(compiling && p.getAutoSaveOnCompile()) {
		return saveSelectedFiles(files);
	}

	SaveItemsDialog s(this,compiling,files);

	if(s.exec()==QDialog::Accepted) {
		const bool autoSave = s.getAutoSaveOnCompile();
		p.setAutoSaveOnCompile(autoSave);

		const QList<QString>& f=s.getItemsToSave();
		return saveSelectedFiles(f);
	}

	return false;
}

void MainWindow::newFile()
{
	auto* e = new CodeEditor(this);
	const int i=ui->tabWidget->addTab(e,tr("[New]"));
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
			const QString& file=c->getFileName();
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
		const QString& file=c->getFileName();
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
	const QString& n=QFileInfo(fileName).fileName();
	const int i=ui->tabWidget->indexOf(editor);
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
		const QString& file=e->getFileName();
		if(!file.isEmpty()) {
			ui->view->setCompiling(!generate);
			worker->setup(file,"",generate);

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

void MainWindow::examplesListClicked(const QModelIndex& item)
{
	auto* m=item.data(Qt::UserRole).value<Module*>();

	QString example=m->getExample();
	if(example.contains('%')) {
		auto& p=Preferences::getInstance();
		example=example.arg(p.getIndent());
	}

	currentEditor()->appendPlainText(example);
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
	const int i=ui->tabWidget->addTab(e,tr("Built In"));
	ui->tabWidget->setCurrentIndex(i);

	connect(e,&CodeEditor::copyAvailable,ui->actionCopy,&QAction::setEnabled);

	auto& b=BuiltinCreator::getInstance(*reporter);

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
	const QDir docdir(QSTRINGIFY(DOCDIR));
#else
	const QDir docdir(QCoreApplication::applicationDirPath());
#endif

	const QFileInfo guide(docdir,"user_guide.html");
	QUrl url;
	if(guide.exists())
		url=QUrl(QString("file:///%1").arg(guide.absoluteFilePath()));
	else
		url=QUrl("https://github.com/GilesBathgate/RapCAD/blob/master/doc/user_guide.asciidoc");

	QDesktopServices::openUrl(url);
}

void MainWindow::flushCaches()
{
	auto& m=CacheManager::getInstance();
	m.flushCaches();
}

void MainWindow::sendToCAM()
{
	const QString title=tr("Send to CAM");
	if(!worker->resultAvailable()) {
		QMessageBox::information(this,title, tr("You have to compile the script before you can export"));
		return;
	}

	auto& p=Preferences::getInstance();
	const QString& command = p.getLaunchCommand();
	if(command.isEmpty()) {
		QMessageBox::information(this,title, tr("No launch command set in preferences"));
		return;
	}

	const QFileInfo info(currentEditor()->getFileName());
	const QString& fileTemplate=QDir::tempPath().append("%1%2_XXXXXX.3mf").arg(QDir::separator()).arg(info.baseName());
	auto* file=new QTemporaryFile(fileTemplate);
	if(!file->open()) {
		QMessageBox::information(this,title, tr("Could not create tempory file"));
		delete file;
		return;
	}
	temporyFiles.append(file);

	const QString& fileName=file->fileName();
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
