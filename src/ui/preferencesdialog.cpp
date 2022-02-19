/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2021 Giles Bathgate
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

#include "decimal.h"
#include "preferences.h"
#include "preferencesdialog.h"
#include "rmath.h"
#include "ui/glview.h"
#include "ui_preferences.h"
#include <QColorDialog>
#include <QDir>

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
	auto& p=Preferences::getInstance();
	QFont f=p.getEditorFont();
	ui->fontComboBox->setCurrentFont(f);
	int pointSize=f.pointSize();
	QComboBox* c=ui->sizeComboBox;
	const QList<int> sizes=QFontDatabase::standardSizes();
	for(auto size: sizes) {
		c->addItem(QString::number(size));
		if(size==pointSize)
			c->setCurrentIndex(c->count()-1);
	}
	ui->darkThemeCheckBox->setChecked(p.getDarkTheme());
	ui->tooltipsCheckBox->setChecked(p.getShowTooltips());
	ui->highlightLineCheckbox->setChecked(p.getHighlightLine());

	setColor(ui->markedVertexColorFrame,p.getMarkedVertexColor());
	setColor(ui->vertexColorFrame,p.getVertexColor());
	setColor(ui->markedEdgeColorFrame,p.getMarkedEdgeColor());
	setColor(ui->edgeColorFrame,p.getEdgeColor());
	setColor(ui->markedFacetColorFrame,p.getMarkedFacetColor());
	setColor(ui->facetColorFrame,p.getFacetColor());
	ui->vertexSizeSpinBox->setValue(p.getVertexSize());
	ui->edgeSizeSpinBox->setValue(p.getEdgeSize());
	ui->checkBox->setChecked(p.getAutoSaveOnCompile());
	bool enabled=true;
	switch(p.getPrecision()) {
		case Precision::Single:
			ui->singleRadio->setChecked(true);
			enabled=false;
			break;
		case Precision::Double:
			ui->doubleRadio->setChecked(true);
			enabled=false;
			break;
		default:
			ui->customRadio->setChecked(true);
			break;
	}
	ui->placesSpinBox->setEnabled(enabled);
	ui->bitsSpinBox->setEnabled(enabled);
	ui->placesSpinBox->setValue(p.getDecimalPlaces());
	ui->bitsSpinBox->setValue(p.getSignificandBits());
	switch(p.getFunctionRounding()) {
		case Rounding::Decimal:
			ui->decimalRoundingRadio->setChecked(true);
			break;
		case Rounding::Base2:
			ui->base2RoundingRadio->setChecked(true);
			break;
		default:
			ui->noRoundingRadio->setChecked(true);
			break;
	}
	switch(p.getNumberFormat()) {
		case NumberFormats::Rational:
			ui->rationalRadio->setChecked(true);
			break;
		case NumberFormats::Scientific:
			ui->scientificRadio->setChecked(true);
			break;
		default:
			ui->fixedRadio->setChecked(true);
			break;
	}

	QPointF o=p.getPrintOrigin();
	ui->XspinBox->setValue(o.x());
	ui->YspinBox->setValue(o.y());

	QVector3D v=p.getPrintVolume();
	ui->widthSpinBox->setValue(int(v.x()));
	ui->lengthSpinBox->setValue(int(v.y()));
	ui->heightSpinBox->setValue(int(v.z()));

	int i=static_cast<int>(p.getPrintBedAppearance());
	ui->appearanceComboBox->setCurrentIndex(i);

	QString command=p.getLaunchCommand();
	ui->launchCommandLineEdit->setText(command);
	launchCommandChanged(command);

	ui->showGCODEButtonCheckbox->setChecked(p.getShowGCODEButton());
	ui->processingScriptlineEdit->setText(p.getCAMScript());
	ui->translateCheckBox->setChecked(p.getTranslateOrigin());

	QString indent=p.getIndent();
	if(indent.contains('\t')) {
		ui->tabsRadioButton->setChecked(true);
	} else {
		ui->spacesRadioButton->setChecked(true);
		ui->spacesSpinBox->setEnabled(true);
		ui->spacesSpinBox->setValue(indent.length());
	}

	ui->threadPoolSizeSpinBox->setValue(p.getThreadPoolSize());

	updatePrecision();
}

void PreferencesDialog::setColor(QWidget* w,const QColor& c)
{
	QString style=QString("* { background-color: rgb(%1,%2,%3); }").arg(c.red()).arg(c.green()).arg(c.blue());
	w->setStyleSheet(style);
}

void PreferencesDialog::setupButtons()
{
	connect(ui->fontComboBox,&QFontComboBox::currentFontChanged,this,&PreferencesDialog::fontChanged);
	connect(ui->sizeComboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&PreferencesDialog::fontSizeChanged);

	connect(ui->markedVertexColorToolButton,&QToolButton::clicked,this,[this](){colorButtonPressed(ui->markedVertexColorFrame);});
	connect(ui->vertexColorToolButton,&QToolButton::clicked,this,[this](){colorButtonPressed(ui->vertexColorFrame);});
	connect(ui->markedEdgeColorToolButton,&QToolButton::clicked,this,[this](){colorButtonPressed(ui->markedEdgeColorFrame);});
	connect(ui->edgeColorToolButton,&QToolButton::clicked,this,[this](){colorButtonPressed(ui->edgeColorFrame);});
	connect(ui->markedFacetColorToolButton,&QToolButton::clicked,this,[this](){colorButtonPressed(ui->markedFacetColorFrame);});
	connect(ui->facetColorToolButton,&QToolButton::clicked,this,[this](){colorButtonPressed(ui->facetColorFrame);});

	connect(ui->vertexSizeSpinBox,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&PreferencesDialog::vertexSizeChanged);
	connect(ui->edgeSizeSpinBox,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&PreferencesDialog::edgeSizeChanged);
	connect(ui->placesSpinBox,QOverload<int>::of(&QSpinBox::valueChanged),this,&PreferencesDialog::placesChanged);
	connect(ui->bitsSpinBox,QOverload<int>::of(&QSpinBox::valueChanged),this,&PreferencesDialog::bitsChanged);
	connect(ui->doubleRadio,&QRadioButton::toggled,this,&PreferencesDialog::precisionType);
	connect(ui->singleRadio,&QRadioButton::toggled,this,&PreferencesDialog::precisionType);

	connect(ui->checkBox,&QCheckBox::stateChanged,this,&PreferencesDialog::autoSaveOnCompileChanged);
	connect(ui->noRoundingRadio,&QRadioButton::toggled,this,&PreferencesDialog::functionRoundingChanged);
	connect(ui->decimalRoundingRadio,&QRadioButton::toggled,this,&PreferencesDialog::functionRoundingChanged);
	connect(ui->base2RoundingRadio,&QRadioButton::toggled,this,&PreferencesDialog::functionRoundingChanged);

	connect(ui->fixedRadio,&QRadioButton::toggled,this,&PreferencesDialog::outputFormatChanged);
	connect(ui->scientificRadio,&QRadioButton::toggled,this,&PreferencesDialog::outputFormatChanged);
	connect(ui->rationalRadio,&QRadioButton::toggled,this,&PreferencesDialog::outputFormatChanged);

	connect(ui->widthSpinBox,QOverload<int>::of(&QSpinBox::valueChanged),this,&PreferencesDialog::volumeChanged);
	connect(ui->lengthSpinBox,QOverload<int>::of(&QSpinBox::valueChanged),this,&PreferencesDialog::volumeChanged);
	connect(ui->heightSpinBox,QOverload<int>::of(&QSpinBox::valueChanged),this,&PreferencesDialog::volumeChanged);

	connect(ui->XspinBox,QOverload<int>::of(&QSpinBox::valueChanged),this,&PreferencesDialog::originChanged);
	connect(ui->YspinBox,QOverload<int>::of(&QSpinBox::valueChanged),this,&PreferencesDialog::originChanged);

	connect(ui->appearanceComboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&PreferencesDialog::appearanceChanged);

	connect(ui->darkThemeCheckBox,&QCheckBox::stateChanged,this,&PreferencesDialog::darkThemeChanged);
	connect(ui->tooltipsCheckBox,&QCheckBox::stateChanged,this,&PreferencesDialog::showTooltipsChanged);
	connect(ui->highlightLineCheckbox,&QCheckBox::stateChanged,this,&PreferencesDialog::highlightLineChanged);

	connect(ui->launchCommandLineEdit,&QLineEdit::textChanged,this,&PreferencesDialog::launchCommandChanged);
	connect(ui->launchCommandLineEdit,&QLineEdit::editingFinished,this,&PreferencesDialog::launchCommandUpdated);

	connect(ui->showGCODEButtonCheckbox,&QCheckBox::stateChanged,this,&PreferencesDialog::showGCODEButtonChanged);
	connect(ui->translateCheckBox,&QCheckBox::stateChanged,this,&PreferencesDialog::translateChanged);
	connect(ui->processingScriptlineEdit,&QLineEdit::editingFinished,this,&PreferencesDialog::processingScriptUpdated);

	connect(ui->tabsRadioButton,&QRadioButton::toggled,this,&PreferencesDialog::indentRadioChanged);
	connect(ui->spacesSpinBox,QOverload<int>::of(&QSpinBox::valueChanged),this,&PreferencesDialog::indentSpacesChanged);

	connect(ui->threadPoolSizeSpinBox,QOverload<int>::of(&QSpinBox::valueChanged),this,&PreferencesDialog::threadPoolSizeChanged);
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

	auto& p=Preferences::getInstance();
	if(frame==ui->markedVertexColorFrame) {
		p.setMarkedVertexColor(c);
	} else if(frame==ui->vertexColorFrame) {
		p.setVertexColor(c);
	} else if(frame==ui->markedEdgeColorFrame) {
		p.setMarkedEdgeColor(c);
	} else if(frame==ui->edgeColorFrame) {
		p.setEdgeColor(c);
	} else if(frame==ui->markedFacetColorFrame) {
		p.setMarkedFacetColor(c);
	} else if(frame==ui->facetColorFrame) {
		p.setFacetColor(c);
	}

	emit preferencesUpdated();
}

void PreferencesDialog::vertexSizeChanged(double s)
{
	auto& p=Preferences::getInstance();
	p.setVertexSize(float(s));
	emit preferencesUpdated();
}

void PreferencesDialog::edgeSizeChanged(double s)
{
	auto& p=Preferences::getInstance();
	p.setEdgeSize(float(s));
	emit preferencesUpdated();
}

void PreferencesDialog::autoSaveOnCompileChanged(int s)
{
	auto& p=Preferences::getInstance();
	p.setAutoSaveOnCompile(s == Qt::Checked);
}

void PreferencesDialog::darkThemeChanged(int s)
{
	auto& p=Preferences::getInstance();
	p.setDarkTheme(s == Qt::Checked);
	emit preferencesUpdated();
}

void PreferencesDialog::showTooltipsChanged(int s)
{
	auto& p=Preferences::getInstance();
	p.setShowTooltips(s == Qt::Checked);
	emit preferencesUpdated();
}

void PreferencesDialog::highlightLineChanged(int s)
{
	auto& p=Preferences::getInstance();
	p.setHighlightLine(s == Qt::Checked);
	emit preferencesUpdated();
}

void PreferencesDialog::launchCommandChanged(const QString& command)
{
	if(command.isEmpty())
		ui->exampleCommandLabel->clear();
	else
		ui->exampleCommandLabel->setText(QString("Example: %1 %2/tempfile.3mf").arg(command,QDir::tempPath()));
}

void PreferencesDialog::launchCommandUpdated()
{
	auto& p=Preferences::getInstance();
	p.setLaunchCommand(ui->launchCommandLineEdit->text());
	emit preferencesUpdated();
}

void PreferencesDialog::showGCODEButtonChanged(int i)
{
	auto& p=Preferences::getInstance();
	p.setShowGCODEButton(i == Qt::Checked);
	emit preferencesUpdated();
}

void PreferencesDialog::translateChanged(int i)
{
	auto& p=Preferences::getInstance();
	p.setTranslateOrigin(i == Qt::Checked);
	emit preferencesUpdated();
}

void PreferencesDialog::processingScriptUpdated()
{
	auto& p=Preferences::getInstance();
	p.setCAMScript(ui->processingScriptlineEdit->text());
	emit preferencesUpdated();
}

void PreferencesDialog::indentRadioChanged(bool checked)
{
	auto& p=Preferences::getInstance();
	if(checked) {
		ui->spacesSpinBox->setEnabled(false);
		p.setIndent("\t");
	} else {
		ui->spacesSpinBox->setEnabled(true);
		p.setIndent(QString(ui->spacesSpinBox->value(),' '));
	}
}

void PreferencesDialog::indentSpacesChanged(int)
{
	indentRadioChanged(ui->tabsRadioButton->isChecked());
}

void PreferencesDialog::threadPoolSizeChanged(int value)
{
	auto& p=Preferences::getInstance();
	p.setThreadPoolSize(value);
}

void PreferencesDialog::placesChanged(int i)
{
	auto& p=Preferences::getInstance();
	p.setDecimalPlaces(i);
	QSpinBox* sb=ui->bitsSpinBox;
	bool block=sb->blockSignals(true);
	sb->setValue(p.getSignificandBits());
	sb->blockSignals(block);
	updatePrecision();
}

void PreferencesDialog::bitsChanged(int i)
{
	auto& p=Preferences::getInstance();
	p.setSignificandBits(i);
	QSpinBox* sb=ui->placesSpinBox;
	bool block=sb->blockSignals(true);
	sb->setValue(p.getDecimalPlaces());
	sb->blockSignals(block);
	updatePrecision();
}

void PreferencesDialog::precisionType(bool)
{
	auto& p=Preferences::getInstance();
	bool enabled=true;
	if(ui->singleRadio->isChecked()) {
		ui->bitsSpinBox->setValue(23);
		p.setPrecision(Precision::Single);
		enabled=false;
	} else if(ui->doubleRadio->isChecked()) {
		ui->bitsSpinBox->setValue(52);
		p.setPrecision(Precision::Double);
		enabled=false;
	} else {
		p.setPrecision(Precision::Custom);
	}
	ui->placesSpinBox->setEnabled(enabled);
	ui->bitsSpinBox->setEnabled(enabled);
}

void PreferencesDialog::functionRoundingChanged(bool)
{
	auto& p=Preferences::getInstance();
	if(ui->decimalRoundingRadio->isChecked())
		p.setFunctionRounding(Rounding::Decimal);
	else if(ui->base2RoundingRadio->isChecked())
		p.setFunctionRounding(Rounding::Base2);
	else
		p.setFunctionRounding(Rounding::None);

	updatePrecision();
}

void PreferencesDialog::outputFormatChanged(bool)
{
	auto& p=Preferences::getInstance();
	if(ui->rationalRadio->isChecked())
		p.setNumberFormat(NumberFormats::Rational);
	else if(ui->scientificRadio->isChecked())
		p.setNumberFormat(NumberFormats::Scientific);
	else
		p.setNumberFormat(NumberFormats::Decimal);

	updatePrecision();
}

void PreferencesDialog::volumeChanged()
{
	auto& p=Preferences::getInstance();
	auto width = float(ui->widthSpinBox->value());
	auto length = float(ui->lengthSpinBox->value());
	auto height = float(ui->heightSpinBox->value());
	p.setPrintVolume(QVector3D(width,length,height));
	emit preferencesUpdated();
}

void PreferencesDialog::originChanged()
{
	auto& p=Preferences::getInstance();
	QPointF o(ui->XspinBox->value(),ui->YspinBox->value());
	p.setPrintOrigin(o);
	emit preferencesUpdated();
}

void PreferencesDialog::appearanceChanged(int i)
{
	auto a = static_cast<BedAppearance>(i);
	auto& p=Preferences::getInstance();
	switch(a) {
		case BedAppearance::MK42: {
			ui->XspinBox->setValue(-125);
			ui->YspinBox->setValue(-105);
			originChanged();
			ui->widthSpinBox->setValue(250);
			ui->lengthSpinBox->setValue(210);
			volumeChanged();
			p.setPrintBedAppearance(a);
		}
		break;
		case BedAppearance::MK2: {
			ui->XspinBox->setValue(-100);
			ui->YspinBox->setValue(-100);
			originChanged();
			ui->widthSpinBox->setValue(200);
			ui->lengthSpinBox->setValue(200);
			volumeChanged();
			p.setPrintBedAppearance(a);
		}
		break;
	}
	emit preferencesUpdated();
}

void PreferencesDialog::fontChanged(QFont f)
{
	auto& p=Preferences::getInstance();
	QString s=ui->sizeComboBox->currentText();
	f.setPointSize(s.toInt());
	p.setEditorFont(f);
	emit preferencesUpdated();
}

void PreferencesDialog::fontSizeChanged(int i)
{
	auto& p=Preferences::getInstance();
	QFont f=ui->fontComboBox->currentFont();
	QString s=ui->sizeComboBox->itemText(i);
	f.setPointSize(s.toInt());
	p.setEditorFont(f);
	emit preferencesUpdated();
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}
