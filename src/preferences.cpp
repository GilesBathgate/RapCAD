#include "preferences.h"

Preferences::Preferences()
{
	settings = new QSettings();
}

Preferences* Preferences::instance=NULL;

Preferences* Preferences::getInstance()
{
	if(!instance)
		instance = new Preferences();

	return instance;
}

int Preferences::getPrecision()
{
	return settings->value("Precision",16).toInt();
}

void Preferences::setPrecision(int p)
{
	settings->setValue("Precision",p);
}

bool Preferences::getFunctionRounding()
{
	return settings->value("FunctionRounding",false).toBool();
}

void Preferences::setFunctionRounding(bool b)
{
	settings->setValue("FunctionRounding",b);
}

double Preferences::getDefaultRotationX()
{
	return settings->value("DefaultRotationX",35.0).toDouble();
}

double Preferences::getDefaultRotationZ()
{
	return settings->value("DefaultRotationZ",35.0).toDouble();
}

double Preferences::getDefaultX()
{
	return settings->value("DefaultX",0.0).toDouble();
}

double Preferences::getDefaultZ()
{
	return settings->value("DefaultZ",0.0).toDouble();
}

double Preferences::getDefaultDistance()
{
	return settings->value("DefaultDistance",500.0).toDouble();
}

void Preferences::setDefaultRotationX(double x)
{
	settings->setValue("DefaultRotationX",x);
}

void Preferences::setDefaultRotationZ(double z)
{
	settings->setValue("DefaultRotationZ",z);
}

void Preferences::setDefaultX(double x)
{
	settings->setValue("DefaultX",x);
}

void Preferences::setDefaultZ(double z)
{
	settings->setValue("DefaultZ",z);
}

void Preferences::setDefaultDistance(double d)
{
	settings->setValue("DefaultDistance",d);
}

QColor Preferences::getMarkedVertexColor()
{
	return settings->value("MarkedVertexColor",QColor(0xff,0xff,0xff)).value<QColor>();
}

void Preferences::setMarkedVertexColor(QColor c)
{
	settings->setValue("MarkedVertexColor",c);
}

QColor Preferences::getVertexColor()
{
	return settings->value("VertexColor",QColor(0xff,0xff,0xff)).value<QColor>();
}

void Preferences::setVertexColor(QColor c)
{
	settings->setValue("VertexColor",c);
}

QColor Preferences::getMarkedEdgeColor()
{
	return settings->value("MarkedEdgeColor",QColor(0x00,0x00,0x00)).value<QColor>();
}

void Preferences::setMarkedEdgeColor(QColor c)
{
	settings->setValue("MarkedEdgeColor",c);
}

QColor Preferences::getEdgeColor()
{
	return settings->value("EdgeColor",QColor(0x00,0x00,0x00)).value<QColor>();
}

void Preferences::setEdgeColor(QColor c)
{
	settings->setValue("EdgeColor",c);
}

QColor Preferences::getMarkedFacetColor()
{
	return settings->value("MarkedFacetColor",QColor(0xff,0x55,0x00)).value<QColor>();
}

void Preferences::setMarkedFacetColor(QColor c)
{
	settings->setValue("MarkedFacetColor",c);
}

QColor Preferences::getFacetColor()
{
	return settings->value("FacetColor",QColor(0xff,0xaa,0x00)).value<QColor>();
}

void Preferences::setFacetColor(QColor c)
{
	settings->setValue("FacetColor",c);
}

bool Preferences::getShowAxes()
{
	return settings->value("ShowAxes",true).toBool();
}

void Preferences::setShowAxes(bool show)
{
	settings->setValue("ShowAxes",show);
}

bool Preferences::getShowEdges()
{
	return settings->value("ShowEdges",true).toBool();
}

void Preferences::setShowEdges(bool show)
{
	settings->setValue("ShowEdges",show);
}

bool Preferences::getSkeleton()
{
	return settings->value("Skeleton",false).toBool();
}

void Preferences::setSkeleton(bool show)
{
	settings->setValue("Skeleton",show);
}

bool Preferences::getShowBase()
{
	return settings->value("ShowBase",true).toBool();
}

void Preferences::setShowBase(bool show)
{
	settings->setValue("ShowBase",show);
}

bool Preferences::getShowPrintArea()
{
	return settings->value("ShowPrintArea",true).toBool();
}

void Preferences::setShowPrintArea(bool show)
{
	settings->setValue("ShowPrintArea",show);
}

bool Preferences::getShowRulers()
{
	return settings->value("ShowRulers",true).toBool();
}

void Preferences::setShowRulers(bool show)
{
	settings->setValue("ShowRulers",show);
}

bool Preferences::getShowEditor()
{
	return settings->value("ShowEditor",true).toBool();
}

void Preferences::setShowEditor(bool show)
{
	settings->setValue("ShowEditor",show);
}

bool Preferences::getShowConsole()
{
	return settings->value("ShowConsole",true).toBool();
}

void Preferences::setShowConsole(bool show)
{
	settings->setValue("ShowConsole",show);
}

bool Preferences::getShowProjects()
{
	return settings->value("ShowProjects",true).toBool();
}

void Preferences::setShowProjects(bool show)
{
	settings->setValue("ShowProjects",show);
}

QPoint Preferences::getWindowPosition()
{
	return settings->value("WindowPosition",QPoint(0,0)).toPoint();
}

void Preferences::setWindowPosition(QPoint p)
{
	settings->setValue("WindowPosition",p);
}

QSize Preferences::getWindowSize()
{
	return settings->value("WindowSize",QSize(1000,600)).toSize();
}

void Preferences::setWindowSize(QSize s)
{
	settings->setValue("WindowSize",s);
}

double Preferences::getVertexSize()
{
	return settings->value("VertexSize",0).toDouble();
}

void Preferences::setVertexSize(double s)
{
	settings->setValue("VertexSize",s);
}

double Preferences::getEdgeSize()
{
	return settings->value("EdgeSize",1).toDouble();
}

void Preferences::setEdgeSize(double s)
{
	settings->setValue("EdgeSize",s);
}

bool Preferences::getAutoSaveOnCompile()
{
	return settings->value("AutoSaveOnCompile",false).toBool();
}

void Preferences::setAutoSaveOnCompile(bool b)
{
	settings->setValue("AutoSaveOnCompile",b);
}


void Preferences::syncDelete()
{
	if(instance) {
		instance->settings->sync();
		delete instance->settings;
		delete instance;
	}
}
