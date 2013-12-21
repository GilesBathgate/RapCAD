#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "stringify.h"
#include "qglobal.h"
#if USE_CGAL
#include <CGAL/version.h>
#endif
#include <boost/version.hpp>
#include <dxflib/dl_dxf.h>

AboutDialog::AboutDialog(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::AboutDialog)
{
	ui->setupUi(this);
	this->setFixedSize(this->width(),this->height());

	QIcon rapcadIcon(":/icons/rapcad-16x16.png");
	this->setWindowIcon(rapcadIcon);

	QPixmap rapcadPicture(":/icons/rapcad-100x100.png");
	ui->picture->setPixmap(rapcadPicture);

	ui->name->setText("RapCAD Version " + QSTRINGIFY(RAPCAD_VERSION));
	ui->components->appendPlainText("Qt\tVersion: " + QString(QT_VERSION_STR));
#if USE_CGAL
	ui->components->appendPlainText("CGAL\tVersion: " + QString(CGAL_VERSION_STR));
#endif
	ui->components->appendPlainText("Boost\tVersion: " + QString(BOOST_LIB_VERSION));
	ui->components->appendPlainText("Dxflib\tVersion: " + QString(DL_VERSION));
}

AboutDialog::~AboutDialog()
{
	delete ui;
}
