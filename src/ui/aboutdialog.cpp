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

#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "stringify.h"
#include "qglobal.h"
#ifdef USE_CGAL
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
	setFixedSize(width(),height());

	QIcon rapcadIcon(":/icons/rapcad-16x16.png");
	setWindowIcon(rapcadIcon);

	QPixmap rapcadPicture(":/icons/rapcad-100x100.png");
	ui->picture->setPixmap(rapcadPicture);

	ui->name->setText(tr("RapCAD Version %1").arg(QSTRINGIFY(RAPCAD_VERSION)));
	ui->components->appendPlainText(tr("Qt\tVersion: %1").arg(QT_VERSION_STR));
#ifdef USE_CGAL
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
