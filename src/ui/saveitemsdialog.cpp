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

#include "saveitemsdialog.h"
#include "ui_saveitemsdialog.h"
#include <QFileInfo>
#include <QPushButton>

SaveItemsDialog::SaveItemsDialog(QWidget* parent, bool compiling, const QList<QString>& items) :
	QDialog(parent),
	ui(new Ui::SaveItemsDialog)
{
	ui->setupUi(this);
	if(!compiling) {
		ui->checkBox->hide();
		QPushButton* discardButton=ui->buttonBox->addButton(tr("Do not Save"), QDialogButtonBox::DestructiveRole);
		connect(discardButton,&QPushButton::clicked,this,&SaveItemsDialog::discardAll);
	}

	QPushButton* saveButton=ui->buttonBox->button(QDialogButtonBox::Save);
	saveButton->setDefault(true);
	saveButton->setFocus(Qt::TabFocusReason);
	saveButton->setMinimumWidth(130); // bad magic number to avoid resizing of button

	for(const auto& fileName: items) {
		QString visibleName;
		QString directory;
		if(fileName.isEmpty()) {
			visibleName = tr("[New]");
		} else {
			const QFileInfo& info = QFileInfo(fileName);
			directory = info.absolutePath();
			visibleName = info.fileName();
		}
		auto* item = new QTreeWidgetItem(ui->treeWidget, QStringList() << visibleName << directory);
		item->setData(0, Qt::UserRole, fileName);
	}

	ui->treeWidget->resizeColumnToContents(0);
	ui->treeWidget->selectAll();
	updateSaveButton();

	connect(ui->buttonBox->button(QDialogButtonBox::Save),&QPushButton::clicked,this,&SaveItemsDialog::collectItemsToSave);
	connect(ui->treeWidget,&QTreeWidget::itemSelectionChanged,this,&SaveItemsDialog::updateSaveButton);
}

SaveItemsDialog::~SaveItemsDialog()
{
	delete ui;
}

void SaveItemsDialog::collectItemsToSave()
{
	itemsToSave.clear();
	const QList<QTreeWidgetItem*> items=ui->treeWidget->selectedItems();
	for(QTreeWidgetItem* item: items) {
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
	const int count=ui->treeWidget->selectedItems().count();
	QPushButton* button = ui->buttonBox->button(QDialogButtonBox::Save);
	const int total=ui->treeWidget->topLevelItemCount();
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

QList<QString> SaveItemsDialog::getItemsToSave() const
{
	return itemsToSave;
}

bool SaveItemsDialog::getAutoSaveOnCompile() const
{
	return ui->checkBox->checkState() == Qt::Checked;
}
