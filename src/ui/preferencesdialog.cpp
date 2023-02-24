/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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
#include "ui_preferences.h"
#include <QColorDialog>
#include <QDir>
#include <QMessageBox>

PreferencesDialog::PreferencesDialog(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::Preferences),
	preferences(Preferences::getTemporary())
{
	ui->setupUi(this);

	setupWidgets();
	setupButtons();
}

void PreferencesDialog::setupWidgets()
{
	const QFont& f=preferences.getEditorFont();
	ui->fontComboBox->setCurrentFont(f);
	const int pointSize=f.pointSize();
	QComboBox& c(*ui->sizeComboBox);
	const QList<int> sizes=QFontDatabase::standardSizes();
	for(auto size: sizes) {
		c.addItem(QString::number(size));
		if(size==pointSize)
			c.setCurrentIndex(c.count()-1);
	}
	ui->visibleWhiteSpacdCheckBox->setChecked(preferences.getVisibleWhiteSpace());
	ui->darkThemeCheckBox->setChecked(preferences.getDarkTheme());
	ui->softwareOpenGLCheckBox->setChecked(preferences.getSoftwareOpenGL());
#ifndef Q_OS_WIN
	ui->softwareOpenGLCheckBox->setEnabled(false);
#endif
	ui->tooltipsCheckBox->setChecked(preferences.getShowTooltips());
	ui->highlightLineCheckbox->setChecked(preferences.getHighlightLine());

	setColor(ui->markedVertexColorFrame,preferences.getMarkedVertexColor());
	setColor(ui->vertexColorFrame,preferences.getVertexColor());
	setColor(ui->markedEdgeColorFrame,preferences.getMarkedEdgeColor());
	setColor(ui->edgeColorFrame,preferences.getEdgeColor());
	setColor(ui->markedFacetColorFrame,preferences.getMarkedFacetColor());
	setColor(ui->facetColorFrame,preferences.getFacetColor());
	ui->vertexSizeSpinBox->setValue(preferences.getVertexSize());
	ui->edgeSizeSpinBox->setValue(preferences.getEdgeSize());
	ui->checkBox->setChecked(preferences.getAutoSaveOnCompile());
	bool enabled=true;
	switch(preferences.getPrecision()) {
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
	ui->placesSpinBox->setValue(preferences.getDecimalPlaces());
	ui->bitsSpinBox->setValue(preferences.getSignificandBits());
	switch(preferences.getFunctionRounding()) {
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
	switch(preferences.getNumberFormat()) {
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

	const QPointF& o=preferences.getPrintOrigin();
	ui->XspinBox->setValue(o.x());
	ui->YspinBox->setValue(o.y());

	const QVector3D& v=preferences.getPrintVolume();
	ui->widthSpinBox->setValue(int(v.x()));
	ui->lengthSpinBox->setValue(int(v.y()));
	ui->heightSpinBox->setValue(int(v.z()));

	const int i=static_cast<int>(preferences.getPrintBedAppearance());
	ui->appearanceComboBox->setCurrentIndex(i);

	const QString& command=preferences.getLaunchCommand();
	ui->launchCommandLineEdit->setText(command);
	launchCommandChanged(command);

	ui->showGCODEButtonCheckbox->setChecked(preferences.getShowGCODEButton());
	ui->processingScriptlineEdit->setText(preferences.getCAMScript());
	ui->translateCheckBox->setChecked(preferences.getTranslateOrigin());

	const QString& indent=preferences.getIndent();
	if(indent.contains('\t')) {
		ui->tabsRadioButton->setChecked(true);
	} else {
		ui->spacesRadioButton->setChecked(true);
		ui->spacesSpinBox->setEnabled(true);
		ui->spacesSpinBox->setValue(indent.length());
	}

	ui->threadPoolSizeSpinBox->setValue(preferences.getThreadPoolSize());
	ui->useCGALAssertionsCheckBox->setChecked(preferences.getUseCGALAssertions());
#ifdef USE_CGAL
#if CGAL_VERSION_NR < CGAL_VERSION_NUMBER(5,4,0)
	ui->threadPoolSizeSpinBox->setDisabled(true);
#endif
#if CGAL_VERSION_NR < CGAL_VERSION_NUMBER(5,6,0)
	ui->useCGALAssertionsCheckBox->setDisabled(true);
#endif
#endif


	updatePrecision();
}

void PreferencesDialog::setColor(QWidget* w,const QColor& c)
{
	const QString& style=QString("* { background-color: rgb(%1,%2,%3); }").arg(c.red()).arg(c.green()).arg(c.blue());
	w->setStyleSheet(style);
}

void PreferencesDialog::setupButtons()
{
	connect(ui->buttonBox,&QDialogButtonBox::clicked,this,&PreferencesDialog::reset);

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

	connect(ui->visibleWhiteSpacdCheckBox,&QCheckBox::stateChanged,this,&PreferencesDialog::visibleWhiteSpaceChanged);
	connect(ui->darkThemeCheckBox,&QCheckBox::stateChanged,this,&PreferencesDialog::darkThemeChanged);
	connect(ui->softwareOpenGLCheckBox,&QCheckBox::stateChanged,this,&PreferencesDialog::softwareOpenGLChanged);
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
	connect(ui->useCGALAssertionsCheckBox,&QCheckBox::stateChanged,this,&PreferencesDialog::useCGALAssertionsChanged);
}

void PreferencesDialog::updatePrecision()
{
	const decimal& ex=r_round_preference(decimal(10)/decimal(9));
	ui->exampleLabel->setText(to_string(ex));
}

void PreferencesDialog::reset(QAbstractButton* button)
{
	auto role=ui->buttonBox->buttonRole(button);
	if(role==QDialogButtonBox::ResetRole) {
		auto sure=QMessageBox::question(this,tr("Are you sure?"),tr("This will reset all the preferences to thier defaults."));
		if(sure==QMessageBox::Yes) {
			Preferences::getInstance().clear();
			reject();
		}
	}
}

void PreferencesDialog::colorButtonPressed(QWidget* frame)
{
	const QColor& c = QColorDialog::getColor(Qt::white,this);
	setColor(frame,c);


	if(frame==ui->markedVertexColorFrame) {
		preferences.setMarkedVertexColor(c);
	} else if(frame==ui->vertexColorFrame) {
		preferences.setVertexColor(c);
	} else if(frame==ui->markedEdgeColorFrame) {
		preferences.setMarkedEdgeColor(c);
	} else if(frame==ui->edgeColorFrame) {
		preferences.setEdgeColor(c);
	} else if(frame==ui->markedFacetColorFrame) {
		preferences.setMarkedFacetColor(c);
	} else if(frame==ui->facetColorFrame) {
		preferences.setFacetColor(c);
	}

}

void PreferencesDialog::vertexSizeChanged(double s)
{
	preferences.setVertexSize(float(s));
}

void PreferencesDialog::edgeSizeChanged(double s)
{
	preferences.setEdgeSize(float(s));
}

void PreferencesDialog::autoSaveOnCompileChanged(int s)
{
	preferences.setAutoSaveOnCompile(s == Qt::Checked);
}

void PreferencesDialog::visibleWhiteSpaceChanged(int s)
{
	preferences.setVisibleWhiteSpace(s == Qt::Checked);
}

void PreferencesDialog::darkThemeChanged(int s)
{
	preferences.setDarkTheme(s == Qt::Checked);
}

void PreferencesDialog::softwareOpenGLChanged(int s)
{
	preferences.setSoftwareOpenGL(s == Qt::Checked);
}

void PreferencesDialog::accept()
{
	preferences.sync();
	QDialog::accept();
}

void PreferencesDialog::reject()
{
	preferences.clear();
	QDialog::reject();
}

void PreferencesDialog::showTooltipsChanged(int s)
{
	preferences.setShowTooltips(s == Qt::Checked);
}

void PreferencesDialog::highlightLineChanged(int s)
{
	preferences.setHighlightLine(s == Qt::Checked);
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
	preferences.setLaunchCommand(ui->launchCommandLineEdit->text());
}

void PreferencesDialog::showGCODEButtonChanged(int i)
{
	preferences.setShowGCODEButton(i == Qt::Checked);
}

void PreferencesDialog::translateChanged(int i)
{
	preferences.setTranslateOrigin(i == Qt::Checked);
}

void PreferencesDialog::processingScriptUpdated()
{
	preferences.setCAMScript(ui->processingScriptlineEdit->text());
}

void PreferencesDialog::indentRadioChanged(bool checked)
{
	if(checked) {
		ui->spacesSpinBox->setEnabled(false);
		preferences.setIndent("\t");
	} else {
		ui->spacesSpinBox->setEnabled(true);
		preferences.setIndent(QString(ui->spacesSpinBox->value(),' '));
	}
}

void PreferencesDialog::indentSpacesChanged(int)
{
	indentRadioChanged(ui->tabsRadioButton->isChecked());
}

void PreferencesDialog::threadPoolSizeChanged(int value)
{
	preferences.setThreadPoolSize(value);
}

void PreferencesDialog::useCGALAssertionsChanged(int s)
{
	preferences.setUseCGALAssertions(s == Qt::Checked);
}

void PreferencesDialog::placesChanged(int i)
{
	preferences.setDecimalPlaces(i);
	QSpinBox* sb=ui->bitsSpinBox;
	const bool block=sb->blockSignals(true);
	sb->setValue(preferences.getSignificandBits());
	sb->blockSignals(block);
	updatePrecision();
}

void PreferencesDialog::bitsChanged(int i)
{
	preferences.setSignificandBits(i);
	QSpinBox* sb=ui->placesSpinBox;
	const bool block=sb->blockSignals(true);
	sb->setValue(preferences.getDecimalPlaces());
	sb->blockSignals(block);
	updatePrecision();
}

void PreferencesDialog::precisionType(bool)
{
	bool enabled=true;
	if(ui->singleRadio->isChecked()) {
		ui->bitsSpinBox->setValue(23);
		preferences.setPrecision(Precision::Single);
		enabled=false;
	} else if(ui->doubleRadio->isChecked()) {
		ui->bitsSpinBox->setValue(52);
		preferences.setPrecision(Precision::Double);
		enabled=false;
	} else {
		preferences.setPrecision(Precision::Custom);
	}
	ui->placesSpinBox->setEnabled(enabled);
	ui->bitsSpinBox->setEnabled(enabled);
}

void PreferencesDialog::functionRoundingChanged(bool)
{
	if(ui->decimalRoundingRadio->isChecked())
		preferences.setFunctionRounding(Rounding::Decimal);
	else if(ui->base2RoundingRadio->isChecked())
		preferences.setFunctionRounding(Rounding::Base2);
	else
		preferences.setFunctionRounding(Rounding::None);

	updatePrecision();
}

void PreferencesDialog::outputFormatChanged(bool)
{
	if(ui->rationalRadio->isChecked())
		preferences.setNumberFormat(NumberFormats::Rational);
	else if(ui->scientificRadio->isChecked())
		preferences.setNumberFormat(NumberFormats::Scientific);
	else
		preferences.setNumberFormat(NumberFormats::Decimal);

	updatePrecision();
}

void PreferencesDialog::volumeChanged()
{
	auto width = float(ui->widthSpinBox->value());
	auto length = float(ui->lengthSpinBox->value());
	auto height = float(ui->heightSpinBox->value());
	preferences.setPrintVolume(QVector3D(width,length,height));
}

void PreferencesDialog::originChanged()
{
	const QPointF o(ui->XspinBox->value(),ui->YspinBox->value());
	preferences.setPrintOrigin(o);
}

void PreferencesDialog::appearanceChanged(int i)
{
	auto a = static_cast<BedAppearance>(i);
	switch(a) {
		case BedAppearance::MK42: {
			ui->XspinBox->setValue(-125);
			ui->YspinBox->setValue(-105);
			originChanged();
			ui->widthSpinBox->setValue(250);
			ui->lengthSpinBox->setValue(210);
			volumeChanged();
			preferences.setPrintBedAppearance(a);
		}
		break;
		case BedAppearance::MK2: {
			ui->XspinBox->setValue(-100);
			ui->YspinBox->setValue(-100);
			originChanged();
			ui->widthSpinBox->setValue(200);
			ui->lengthSpinBox->setValue(200);
			volumeChanged();
			preferences.setPrintBedAppearance(a);
		}
		break;
	}
}

void PreferencesDialog::fontChanged(QFont f)
{
	const QString& s=ui->sizeComboBox->currentText();
	f.setPointSize(s.toInt());
	preferences.setEditorFont(f);
}

void PreferencesDialog::fontSizeChanged(int i)
{
	QFont f=ui->fontComboBox->currentFont();
	const QString& s=ui->sizeComboBox->itemText(i);
	f.setPointSize(s.toInt());
	preferences.setEditorFont(f);
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}
