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
