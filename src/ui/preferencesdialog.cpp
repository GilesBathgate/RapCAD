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

	connect(this->ui->markedVertexColorToolButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
	connect(this->ui->vertexColorToolButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
	connect(this->ui->markedEdgeColorToolButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
	connect(this->ui->edgeColorToolButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
	connect(this->ui->markedFacetColorToolButton,SIGNAL(clicked()),signalMapper,SLOT(map()));
	connect(this->ui->facetColorToolButton,SIGNAL(clicked()),signalMapper,SLOT(map()));

	connect(signalMapper,SIGNAL(mapped(QWidget*)),this,SLOT(colorButtonPressed(QWidget*)));

	connect(this->ui->vertexSizeSpinBox,SIGNAL(valueChanged(double)),SLOT(vertexSizeChanged(double)));
	connect(this->ui->edgeSizeSpinBox,SIGNAL(valueChanged(double)),SLOT(edgeSizeChanged(double)));
	connect(this->ui->precisionSpinBox,SIGNAL(valueChanged(int)),SLOT(precisionChanged(int)));

	connect(this->ui->checkBox,&QCheckBox::stateChanged,this,&PreferencesDialog::autoSaveOnCompileChanged);
	connect(this->ui->functionRoundingCheckBox,&QCheckBox::stateChanged,this,&PreferencesDialog::functionRoundingChanged);
	connect(this->ui->rationalFormatCheckBox,&QCheckBox::stateChanged,this,&PreferencesDialog::rationalFormatChanged);

	connect(this->ui->widthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(volumeChanged()));
	connect(this->ui->lengthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(volumeChanged()));
	connect(this->ui->heightSpinBox,SIGNAL(valueChanged(int)),this,SLOT(volumeChanged()));

	connect(this->ui->XspinBox,SIGNAL(valueChanged(int)),this,SLOT(originChanged()));
	connect(this->ui->YspinBox,SIGNAL(valueChanged(int)),this,SLOT(originChanged()));

	connect(this->ui->appearanceComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(appearanceChanged(int)));
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
