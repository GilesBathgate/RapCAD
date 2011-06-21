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

QColor Preferences::getMarkedVertexColor()
{
	return settings->value("MarkedVertexColor",QColor(0xb7,0xe8,0x5c)).value<QColor>();
}

void Preferences::setMarkedVertexColor(QColor c)
{
	settings->setValue("MarkedVertexColor",c);
}

QColor Preferences::getVertexColor()
{
	return settings->value("VertexColor",QColor(0xff,0xf6,0x7c)).value<QColor>();
}

void Preferences::setVertexColor(QColor c)
{
	settings->setValue("VertexColor",c);
}

QColor Preferences::getMarkedEdgeColor()
{
	return settings->value("MarkedEdgeColor",QColor(0xab,0xd8,0x56)).value<QColor>();
}

void Preferences::setMarkedEdgeColor(QColor c)
{
	settings->setValue("MarkedEdgeColor",c);
}

QColor Preferences::getEdgeColor()
{
	return settings->value("EdgeColor",QColor(0xff,0xec,0x5e)).value<QColor>();
}

void Preferences::setEdgeColor(QColor c)
{
	settings->setValue("EdgeColor",c);
}

QColor Preferences::getMarkedFacetColor()
{
	return settings->value("MarkedFacetColor",QColor(0x9d,0xcb,0x51)).value<QColor>();
}

void Preferences::setMarkedFacetColor(QColor c)
{
	settings->setValue("MarkedFacetColor",c);
}

QColor Preferences::getFacetColor()
{
	return settings->value("FacetColor",QColor(0xf9,0xd7,0x2c)).value<QColor>();
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
	return settings->value("ShowEdges",false).toBool();
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

void Preferences::syncDelete()
{
	if(instance) {
		instance->settings->sync();
		delete instance->settings;
		delete instance;
	}
}
