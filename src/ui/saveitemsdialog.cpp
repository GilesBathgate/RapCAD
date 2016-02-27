#include <QPushButton>
#include <QFileInfo>
#include "saveitemsdialog.h"
#include "ui_saveitemsdialog.h"

SaveItemsDialog::SaveItemsDialog(QWidget* parent,bool compiling,QList<QString> items) :
	QDialog(parent),
	ui(new Ui::SaveItemsDialog)
{
	ui->setupUi(this);
	if(!compiling) {
		ui->checkBox->hide();
		QPushButton* discardButton=ui->buttonBox->addButton(tr("Do not Save"), QDialogButtonBox::DestructiveRole);
		connect(discardButton, SIGNAL(clicked()), this, SLOT(discardAll()));
	}

	QPushButton* saveButton=ui->buttonBox->button(QDialogButtonBox::Save);
	saveButton->setDefault(true);
	saveButton->setFocus(Qt::TabFocusReason);
	saveButton->setMinimumWidth(130); // bad magic number to avoid resizing of button

	foreach(QString fileName, items) {
		QString visibleName;
		QString directory;
		if(fileName.isEmpty()) {
			visibleName = tr("[New]");
		} else {
			QFileInfo info = QFileInfo(fileName);
			directory = info.absolutePath();
			visibleName = info.fileName();
		}
		QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget, QStringList() << visibleName << directory);
		item->setData(0, Qt::UserRole, fileName);
	}

	ui->treeWidget->resizeColumnToContents(0);
	ui->treeWidget->selectAll();
	updateSaveButton();

	connect(ui->buttonBox->button(QDialogButtonBox::Save), SIGNAL(clicked()),
		this, SLOT(collectItemsToSave()));
	connect(ui->treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(updateSaveButton()));
}

SaveItemsDialog::~SaveItemsDialog()
{
	delete ui;
}

void SaveItemsDialog::collectItemsToSave()
{
	itemsToSave.clear();
	foreach(QTreeWidgetItem *item, ui->treeWidget->selectedItems()) {
		itemsToSave.append(item->data(0, Qt::UserRole).toString());
	}
	accept();
}

void SaveItemsDialog::discardAll()
{
	ui->treeWidget->clearSelection();
	collectItemsToSave();
}

void SaveItemsDialog::updateSaveButton()
{
	int count=ui->treeWidget->selectedItems().count();
	QPushButton* button = ui->buttonBox->button(QDialogButtonBox::Save);
	int total=ui->treeWidget->topLevelItemCount();
	if(total==1) {
		button->setEnabled(true);
		button->setText(tr("Save"));
	} else if(count == total) {
		button->setEnabled(true);
		button->setText(tr("Save All"));
	} else if(count == 0) {
		button->setEnabled(false);
		button->setText(tr("Save"));
	} else {
		button->setEnabled(true);
		button->setText(tr("Save Selected"));
	}
}

QList<QString> SaveItemsDialog::getItemsToSave()
{
	return itemsToSave;
}

bool SaveItemsDialog::getAutoSaveOnCompile()
{
	return ui->checkBox->checkState() == Qt::Checked;
}
