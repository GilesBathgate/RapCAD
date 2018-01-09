/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2018 Giles Bathgate
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

#include <QPushButton>
#include <QColorDialog>
#include "preferencesdialog.h"
#include "ui_preferences.h"
#include "preferences.h"

PreferencesDialog::PreferencesDialog(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::Preferences)
{
	ui->setupUi(this);

	setupWidgets();
	setupButtons();
}

void PreferencesDialog::setupWidgets()
{
	Preferences* p = Preferences::getInstance();
	setColor(ui->markedVertexColorFrame,p->getMarkedVertexColor());
	setColor(ui->vertexColorFrame,p->getVertexColor());
	setColor(ui->markedEdgeColorFrame,p->getMarkedEdgeColor());
	setColor(ui->edgeColorFrame,p->getEdgeColor());
	setColor(ui->markedFacetColorFrame,p->getMarkedFacetColor());
	setColor(ui->facetColorFrame,p->getFacetColor());
	ui->vertexSizeSpinBox->setValue(p->getVertexSize());
	ui->edgeSizeSpinBox->setValue(p->getEdgeSize());
	ui->checkBox->setChecked(p->getAutoSaveOnCompile());
	ui->precisionSpinBox->setValue(p->getPrecision());
	ui->functionRoundingCheckBox->setChecked(p->getFunctionRounding());
	ui->rationalFormatCheckBox->setChecked(p->getRationalFormat());

	QPointF o=p->getPrintOrigin();
	ui->XspinBox->setValue(o.x());
	ui->YspinBox->setValue(o.y());

	QVector3D v=p->getPrintVolume();
	ui->widthSpinBox->setValue(v.x());
	ui->lengthSpinBox->setValue(v.y());
	ui->heightSpinBox->setValue(v.z());

	ui->appearanceComboBox->setCurrentIndex(p->getPrintBedAppearance());
}

void PreferencesDialog::setColor(QWidget* w,QColor c)
{
	QString style=QString("* { background-color: rgb(%1,%2,%3); }").arg(c.red()).arg(c.green()).arg(c.blue());
	w->setStyleSheet(style);
}

void PreferencesDialog::setupButtons()
{
	signalMapper = new QSignalMapper(this);
	signalMapper->setMapping(ui->markedVertexColorToolButton,ui->markedVertexColorFrame);
	signalMapper->setMapping(ui->vertexColorToolButton,ui->vertexColorFrame);
	signalMapper->setMapping(ui->markedEdgeColorToolButton,ui->markedEdgeColorFrame);
	signalMapper->setMapping(ui->edgeColorToolButton,ui->edgeColorFrame);
	signalMapper->setMapping(ui->markedFacetColorToolButton,ui->markedFacetColorFrame);
	signalMapper->setMapping(ui->facetColorToolButton,ui->facetColorFrame);

	connect(ui->markedVertexColorToolButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
	connect(ui->vertexColorToolButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
	connect(ui->markedEdgeColorToolButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
	connect(ui->edgeColorToolButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
	connect(ui->markedFacetColorToolButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
	connect(ui->facetColorToolButton,SIGNAL(clicked()),signalMapper,SLOT(map()));

	connect(signalMapper,SIGNAL(mapped(QWidget*)),this,SLOT(colorButtonPressed(QWidget*)));

	connect(ui->vertexSizeSpinBox,SIGNAL(valueChanged(double)),SLOT(vertexSizeChanged(double)));
	connect(ui->edgeSizeSpinBox,SIGNAL(valueChanged(double)),SLOT(edgeSizeChanged(double)));
	connect(ui->precisionSpinBox,SIGNAL(valueChanged(int)),SLOT(precisionChanged(int)));

	connect(ui->checkBox,&QCheckBox::stateChanged,this,&PreferencesDialog::autoSaveOnCompileChanged);
	connect(ui->functionRoundingCheckBox,&QCheckBox::stateChanged,this,&PreferencesDialog::functionRoundingChanged);
	connect(ui->rationalFormatCheckBox,&QCheckBox::stateChanged,this,&PreferencesDialog::rationalFormatChanged);

	connect(ui->widthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(volumeChanged()));
	connect(ui->lengthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(volumeChanged()));
	connect(ui->heightSpinBox,SIGNAL(valueChanged(int)),this,SLOT(volumeChanged()));

	connect(ui->XspinBox,SIGNAL(valueChanged(int)),this,SLOT(originChanged()));
	connect(ui->YspinBox,SIGNAL(valueChanged(int)),this,SLOT(originChanged()));

	connect(ui->appearanceComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(appearanceChanged(int)));
}

void PreferencesDialog::colorButtonPressed(QWidget* frame)
{
	QColor c = QColorDialog::getColor(Qt::white,this);
	setColor(frame,c);

	Preferences* p = Preferences::getInstance();
	if(frame==ui->markedVertexColorFrame)
		p->setMarkedVertexColor(c);
	else if(frame==ui->vertexColorFrame)
		p->setVertexColor(c);
	else if(frame==ui->markedEdgeColorFrame)
		p->setMarkedEdgeColor(c);
	else if(frame==ui->edgeColorFrame)
		p->setEdgeColor(c);
	else if(frame==ui->markedFacetColorFrame)
		p->setMarkedFacetColor(c);
	else if(frame==ui->facetColorFrame)
		p->setFacetColor(c);

	preferencesUpdated();
}

void PreferencesDialog::vertexSizeChanged(double s)
{
	Preferences* p = Preferences::getInstance();
	p->setVertexSize(s);
	preferencesUpdated();
}

void PreferencesDialog::edgeSizeChanged(double s)
{
	Preferences* p = Preferences::getInstance();
	p->setEdgeSize(s);
	preferencesUpdated();
}

void PreferencesDialog::autoSaveOnCompileChanged(int s)
{
	Preferences* p = Preferences::getInstance();
	p->setAutoSaveOnCompile(s == Qt::Checked);
}

void PreferencesDialog::precisionChanged(int i)
{
	Preferences* p = Preferences::getInstance();
	p->setPrecision(i);
}

void PreferencesDialog::functionRoundingChanged(int s)
{
	Preferences* p = Preferences::getInstance();
	p->setFunctionRounding(s == Qt::Checked);
}

void PreferencesDialog::rationalFormatChanged(int s)
{
	Preferences* p = Preferences::getInstance();
	p->setRationalFormat(s == Qt::Checked);
}

void PreferencesDialog::volumeChanged()
{
	Preferences* p = Preferences::getInstance();
	QVector3D v(ui->widthSpinBox->value(),ui->lengthSpinBox->value(),ui->heightSpinBox->value());
	p->setPrintVolume(v);
	preferencesUpdated();
}

void PreferencesDialog::originChanged()
{
	Preferences* p = Preferences::getInstance();
	QPointF o(ui->XspinBox->value(),ui->YspinBox->value());
	p->setPrintOrigin(o);
	preferencesUpdated();
}

void PreferencesDialog::appearanceChanged(int index)
{
	Preferences* p = Preferences::getInstance();
	switch(index) {
		case 0: { //MK42
			ui->XspinBox->setValue(-125);
			ui->YspinBox->setValue(-105);
			originChanged();
			ui->widthSpinBox->setValue(250);
			ui->lengthSpinBox->setValue(210);
			volumeChanged();
			p->setPrintBedAppearance(index);
		}
		break;
		case 1: { //MK2
			ui->XspinBox->setValue(-100);
			ui->YspinBox->setValue(-100);
			originChanged();
			ui->widthSpinBox->setValue(200);
			ui->lengthSpinBox->setValue(200);
			volumeChanged();
			p->setPrintBedAppearance(index);
		}
		break;
	}
	preferencesUpdated();
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}
