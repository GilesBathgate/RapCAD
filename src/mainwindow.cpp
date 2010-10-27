/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010  Giles Bathgate
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myModel = new QStandardItemModel();
    QStringList headers;
    headers.append(QString("Projects"));
    myModel->setHorizontalHeaderLabels(headers);
    QStandardItem *parentItem = myModel->invisibleRootItem();

    for (int i = 0; i < 4; ++i) {
         QStandardItem *item = new QStandardItem(QString("item %0").arg(i));
         parentItem->appendRow(item);
         parentItem = item;
     }

    ui->treeView->setModel(myModel);

}

MainWindow::~MainWindow()
{
    delete ui;
}
