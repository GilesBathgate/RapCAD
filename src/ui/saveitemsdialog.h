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
#ifndef SAVEITEMSDIALOG_H
#define SAVEITEMSDIALOG_H

#include <QDialog>

namespace Ui {
	class SaveItemsDialog;
}

class SaveItemsDialog : public QDialog
{
	Q_OBJECT

public:
	SaveItemsDialog(QWidget* parent,bool,const QList<QString>&);
	~SaveItemsDialog();

	QList<QString> getItemsToSave() const;
	bool getAutoSaveOnCompile() const;

private slots:
	void collectItemsToSave();
	void discardAll();
	void updateSaveButton();
private:
	Ui::SaveItemsDialog* ui;
	QList<QString> itemsToSave;
};

#endif // SAVEITEMSDIALOG_H
