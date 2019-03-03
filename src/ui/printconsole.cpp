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

#include "printconsole.h"
#include "ui_printconsole.h"

PrintConsole::PrintConsole(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::PrintConsole)
{
	ui->setupUi(this);
	QIcon rapcadIcon(":/icons/rapcad-16x16.png");
	setWindowIcon(rapcadIcon);
	QString start("#ff0");
	QString finish("#cc0");
	ui->xPlus100->setStyleSheet(getStyleString(start,finish));
	ui->xPlus10->setStyleSheet(getStyleString(start,finish));
	ui->xPlus1->setStyleSheet(getStyleString(start,finish));
	ui->xPlus0_1->setStyleSheet(getStyleString(start,finish));
	ui->homeX->setStyleSheet(getStyleString(start,finish));
	ui->xMinus0_1->setStyleSheet(getStyleString(start,finish));
	ui->xMinus1->setStyleSheet(getStyleString(start,finish));
	ui->xMinus10->setStyleSheet(getStyleString(start,finish));
	ui->xMinus100->setStyleSheet(getStyleString(start,finish));

	start="#66f";
	finish="#66c";
	ui->yPlus100->setStyleSheet(getStyleString(start,finish));
	ui->yPlus10->setStyleSheet(getStyleString(start,finish));
	ui->yPlus1->setStyleSheet(getStyleString(start,finish));
	ui->yPlus0_1->setStyleSheet(getStyleString(start,finish));
	ui->homeY->setStyleSheet(getStyleString(start,finish));
	ui->yMinus0_1->setStyleSheet(getStyleString(start,finish));
	ui->yMinus1->setStyleSheet(getStyleString(start,finish));
	ui->yMinus10->setStyleSheet(getStyleString(start,finish));
	ui->yMinus100->setStyleSheet(getStyleString(start,finish));

	start="#0f0";
	finish="#0c0";
	ui->zPlus10->setStyleSheet(getStyleString(start,finish));
	ui->zPlus1->setStyleSheet(getStyleString(start,finish));
	ui->zPlus0_1->setStyleSheet(getStyleString(start,finish));
	ui->homeZ->setStyleSheet(getStyleString(start,finish));
	ui->zMinus0_1->setStyleSheet(getStyleString(start,finish));
	ui->zMinus1->setStyleSheet(getStyleString(start,finish));
	ui->zMinus10->setStyleSheet(getStyleString(start,finish));
}

QString PrintConsole::getStyleString(const QString &start, const QString &finish)
{
	QString style(
	"border: 1px solid #888;"
	"border-radius: 4px;"
	"background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 ");
	style.append(start);
	style.append(", stop: 1 ");
	style.append(finish);
	style.append(");");
	return style;
}

PrintConsole::~PrintConsole()
{
	delete ui;
}
