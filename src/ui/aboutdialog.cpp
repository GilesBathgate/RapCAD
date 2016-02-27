#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "stringify.h"
#include "qglobal.h"
#if USE_CGAL
#include <CGAL/version.h>
#endif
#include <boost/version.hpp>
#ifdef USE_DXF
#include <dxflib/dl_dxf.h>
#endif

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

	ui->name->setText(tr("RapCAD Version %1").arg(QSTRINGIFY(RAPCAD_VERSION)));
	ui->components->appendPlainText(tr("Qt\tVersion: %1").arg(QT_VERSION_STR));
#if USE_CGAL
	ui->components->appendPlainText(tr("CGAL\tVersion: %1").arg(CGAL_VERSION_STR));
#endif
	ui->components->appendPlainText(tr("Boost\tVersion: %1.%2.%3").arg(BOOST_VERSION / 100000).arg(BOOST_VERSION / 100 % 1000).arg(BOOST_VERSION % 100));
#ifdef USE_DXF
	ui->components->appendPlainText(tr("Dxflib\tVersion: %1").arg(DL_VERSION));
#endif
}

AboutDialog::~AboutDialog()
{
	delete ui;
}
