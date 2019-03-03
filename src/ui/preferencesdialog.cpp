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

#include <QPushButton>
#include <QColorDialog>
#include "preferencesdialog.h"
#include "ui_preferences.h"
#include "preferences.h"
#include "decimal.h"
#include "rmath.h"
#include "ui/glview.h"

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
	QFont f=p->getEditorFont();
	ui->fontComboBox->setCurrentFont(f);
	int pointSize=f.pointSize();
	QComboBox* c=ui->sizeComboBox;
	QFontDatabase db;
	for(auto size: db.standardSizes()) {
		c->addItem(QString::number(size));
		if(size==pointSize)
			c->setCurrentIndex(c->count()-1);
	}
	ui->tooltipsCheckBox->setChecked(p->getShowTooltips());
	ui->highlightLineCheckbox->setChecked(p->getHighlightLine());

	setColor(ui->markedVertexColorFrame,p->getMarkedVertexColor());
	setColor(ui->vertexColorFrame,p->getVertexColor());
	setColor(ui->markedEdgeColorFrame,p->getMarkedEdgeColor());
	setColor(ui->edgeColorFrame,p->getEdgeColor());
	setColor(ui->markedFacetColorFrame,p->getMarkedFacetColor());
	setColor(ui->facetColorFrame,p->getFacetColor());
	ui->vertexSizeSpinBox->setValue(p->getVertexSize());
	ui->edgeSizeSpinBox->setValue(p->getEdgeSize());
	ui->checkBox->setChecked(p->getAutoSaveOnCompile());
	bool enabled=true;
	switch(p->getPrecision()) {
		case 0:
			ui->singleRadio->setChecked(true);
			enabled=false;
			break;
		case 1:
			ui->doubleRadio->setChecked(true);
			enabled=false;
			break;
		default:
			ui->customRadio->setChecked(true);
			break;
	}
	ui->placesSpinBox->setEnabled(enabled);
	ui->bitsSpinBox->setEnabled(enabled);
	ui->placesSpinBox->setValue(p->getDecimalPlaces());
	ui->bitsSpinBox->setValue(p->getSignificandBits());
	switch(p->getFunctionRounding()) {
		case 0:
			ui->decimalRoundingRadio->setChecked(true);
			break;
		case 1:
			ui->base2RoundingRadio->setChecked(true);
			break;
		default:
			ui->noRoundingRadio->setChecked(true);
			break;
	}
	ui->rationalFormatCheckBox->setChecked(p->getRationalFormat());

	QPointF o=p->getPrintOrigin();
	ui->XspinBox->setValue(o.x());
	ui->YspinBox->setValue(o.y());

	QVector3D v=p->getPrintVolume();
	ui->widthSpinBox->setValue(v.x());
	ui->lengthSpinBox->setValue(v.y());
	ui->heightSpinBox->setValue(v.z());

	ui->appearanceComboBox->setCurrentIndex(p->getPrintBedAppearance());
	updatePrecision();
}

void PreferencesDialog::setColor(QWidget* w,QColor c)
{
	QString style=QString("* { background-color: rgb(%1,%2,%3); }").arg(c.red()).arg(c.green()).arg(c.blue());
	w->setStyleSheet(style);
}

void PreferencesDialog::setupButtons()
{
	connect(ui->fontComboBox,SIGNAL(currentFontChanged(QFont)),SLOT(fontChanged(QFont)));
	connect(ui->sizeComboBox,SIGNAL(currentIndexChanged(QString)),SLOT(fontSizeChanged(QString)));

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
	connect(ui->placesSpinBox,SIGNAL(valueChanged(int)),SLOT(placesChanged(int)));
	connect(ui->bitsSpinBox,SIGNAL(valueChanged(int)),SLOT(bitsChanged(int)));
	connect(ui->doubleRadio,SIGNAL(toggled(bool)),SLOT(precisionType(bool)));
	connect(ui->singleRadio,SIGNAL(toggled(bool)),SLOT(precisionType(bool)));

	connect(ui->checkBox,&QCheckBox::stateChanged,this,&PreferencesDialog::autoSaveOnCompileChanged);
	connect(ui->noRoundingRadio,SIGNAL(toggled(bool)),SLOT(functionRoundingChanged(bool)));
	connect(ui->decimalRoundingRadio,SIGNAL(toggled(bool)),SLOT(functionRoundingChanged(bool)));
	connect(ui->base2RoundingRadio,SIGNAL(toggled(bool)),SLOT(functionRoundingChanged(bool)));
	connect(ui->rationalFormatCheckBox,&QCheckBox::stateChanged,this,&PreferencesDialog::rationalFormatChanged);

	connect(ui->widthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(volumeChanged()));
	connect(ui->lengthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(volumeChanged()));
	connect(ui->heightSpinBox,SIGNAL(valueChanged(int)),this,SLOT(volumeChanged()));

	connect(ui->XspinBox,SIGNAL(valueChanged(int)),this,SLOT(originChanged()));
	connect(ui->YspinBox,SIGNAL(valueChanged(int)),this,SLOT(originChanged()));

	connect(ui->appearanceComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(appearanceChanged(int)));

	connect(ui->tooltipsCheckBox,SIGNAL(stateChanged(int)),this,SLOT(showTooltipsChanged(int)));
	connect(ui->highlightLineCheckbox,SIGNAL(stateChanged(int)),this,SLOT(highlightLineChanged(int)));
}

void PreferencesDialog::updatePrecision()
{
	decimal ex=r_round_preference(decimal(10)/decimal(9));
	ui->exampleLabel->setText(to_string(ex));
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

	emit preferencesUpdated();
}

void PreferencesDialog::vertexSizeChanged(double s)
{
	Preferences* p = Preferences::getInstance();
	p->setVertexSize(s);
	emit preferencesUpdated();
}

void PreferencesDialog::edgeSizeChanged(double s)
{
	Preferences* p = Preferences::getInstance();
	p->setEdgeSize(s);
	emit preferencesUpdated();
}

void PreferencesDialog::autoSaveOnCompileChanged(int s)
{
	Preferences* p = Preferences::getInstance();
	p->setAutoSaveOnCompile(s == Qt::Checked);
}

void PreferencesDialog::showTooltipsChanged(int s)
{
	Preferences* p = Preferences::getInstance();
	p->setShowTooltips(s == Qt::Checked);
	emit preferencesUpdated();
}

void PreferencesDialog::highlightLineChanged(int s)
{
	Preferences* p = Preferences::getInstance();
	p->setHighlightLine(s == Qt::Checked);
	emit preferencesUpdated();
}

void PreferencesDialog::placesChanged(int i)
{
	Preferences* p = Preferences::getInstance();
	p->setDecimalPlaces(i);
	QSpinBox* sb=ui->bitsSpinBox;
	bool block=sb->blockSignals(true);
	sb->setValue(p->getSignificandBits());
	sb->blockSignals(block);
	updatePrecision();
}

void PreferencesDialog::bitsChanged(int i)
{
	Preferences* p = Preferences::getInstance();
	p->setSignificandBits(i);
	QSpinBox* sb=ui->placesSpinBox;
	bool block=sb->blockSignals(true);
	sb->setValue(p->getDecimalPlaces());
	sb->blockSignals(block);
	updatePrecision();
}

void PreferencesDialog::precisionType(bool)
{
	Preferences* p = Preferences::getInstance();
	bool enabled=true;
	if(ui->singleRadio->isChecked()) {
		ui->bitsSpinBox->setValue(23);
		p->setPrecision(0);
		enabled=false;
	} else if(ui->doubleRadio->isChecked()) {
		ui->bitsSpinBox->setValue(52);
		p->setPrecision(1);
		enabled=false;
	} else {
		p->setPrecision(2);
	}
	ui->placesSpinBox->setEnabled(enabled);
	ui->bitsSpinBox->setEnabled(enabled);
}

void PreferencesDialog::functionRoundingChanged(bool)
{
	Preferences* p = Preferences::getInstance();
	if(ui->decimalRoundingRadio->isChecked())
		p->setFunctionRounding(0);
	else if(ui->base2RoundingRadio->isChecked())
		p->setFunctionRounding(1);
	else
		p->setFunctionRounding(2);

	updatePrecision();
}

void PreferencesDialog::rationalFormatChanged(int s)
{
	Preferences* p = Preferences::getInstance();
	p->setRationalFormat(s == Qt::Checked);
	updatePrecision();
}

void PreferencesDialog::volumeChanged()
{
	Preferences* p = Preferences::getInstance();
	QVector3D v(ui->widthSpinBox->value(),ui->lengthSpinBox->value(),ui->heightSpinBox->value());
	p->setPrintVolume(v);
	emit preferencesUpdated();
}

void PreferencesDialog::originChanged()
{
	Preferences* p = Preferences::getInstance();
	QPointF o(ui->XspinBox->value(),ui->YspinBox->value());
	p->setPrintOrigin(o);
	emit preferencesUpdated();
}

void PreferencesDialog::appearanceChanged(int i)
{
	auto a = (GLView::Appearance_t)i;
	Preferences* p = Preferences::getInstance();
	switch(a) {
		case GLView::Appearance_t::MK42: {
			ui->XspinBox->setValue(-125);
			ui->YspinBox->setValue(-105);
			originChanged();
			ui->widthSpinBox->setValue(250);
			ui->lengthSpinBox->setValue(210);
			volumeChanged();
			p->setPrintBedAppearance(a);
		}
		break;
		case GLView::Appearance_t::MK2: {
			ui->XspinBox->setValue(-100);
			ui->YspinBox->setValue(-100);
			originChanged();
			ui->widthSpinBox->setValue(200);
			ui->lengthSpinBox->setValue(200);
			volumeChanged();
			p->setPrintBedAppearance(a);
		}
		break;
	}
	emit preferencesUpdated();
}

void PreferencesDialog::fontChanged(QFont f)
{
	Preferences* p = Preferences::getInstance();
	QString s=ui->sizeComboBox->currentText();
	f.setPointSize(s.toInt());
	p->setEditorFont(f);
	emit preferencesUpdated();
}

void PreferencesDialog::fontSizeChanged(const QString& s)
{
	Preferences* p = Preferences::getInstance();
	QFont f=ui->fontComboBox->currentFont();
	f.setPointSize(s.toInt());
	p->setEditorFont(f);
	emit preferencesUpdated();
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}
