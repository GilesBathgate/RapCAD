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
#include "preferences.h"

#include <math.h>
#define LOG10_2 0.30102999566398119521 /* log10(2) = log base 10 of 2 */

#ifdef USE_CGAL
#include <CGAL/Gmpfr.h>
#endif

Preferences::Preferences() :
	settings(new QSettings())
{
	updatePrecision();
}

void Preferences::updatePrecision()
{
	precision=getSignificandBits();
#ifdef USE_CGAL
	CGAL::Gmpfr::set_default_precision(precision);
#endif
}

bool Preferences::getHighlightLine() const
{
	return settings->value("HighlightLine",false).toBool();
}

void Preferences::setHighlightLine(bool value)
{
	settings->setValue("HighlightLine",value);
}

bool Preferences::getShowTooltips() const
{
	return settings->value("ShowTooltips",true).toBool();
}

void Preferences::setShowTooltips(bool value)
{
	settings->setValue("ShowTooltips",value);
}

QFont Preferences::getEditorFont() const
{
	QString family=settings->value("EditorFont.Family","Courier").toString();
	int size=settings->value("EditorFont.Size",10).toInt();

	return QFont(family,size);
}

void Preferences::setEditorFont(const QFont& value)
{
	settings->setValue("EditorFont.Family",value.family());
	settings->setValue("EditorFont.Size",value.pointSize());
}

Preferences* Preferences::instance=nullptr;

Preferences* Preferences::getInstance()
{
	if(!instance)
		instance = new Preferences();

	return instance;
}

int Preferences::getPrecision() const
{
	return settings->value("Precision",2).toInt();
}

void Preferences::setPrecision(int p)
{
	settings->setValue("Precision",p);
}

int Preferences::getDecimalPlaces() const
{
	return ceil(getSignificandBits()*LOG10_2);
}

void Preferences::setDecimalPlaces(int p)
{
	setSignificandBits(floor(p/LOG10_2));
}

int Preferences::getSignificandBits() const
{
	return settings->value("SignificandBits",53).toInt();
}

void Preferences::setSignificandBits(int b)
{
	settings->setValue("SignificandBits",b);
	updatePrecision();
}

int Preferences::getFunctionRounding() const
{
	return settings->value("FunctionRounding",0).toInt();
}

void Preferences::setFunctionRounding(int i)
{
	settings->setValue("FunctionRounding",i);
}

bool Preferences::getRationalFormat() const
{
	return settings->value("RationalFormat",false).toBool();
}

void Preferences::setRationalFormat(bool b)
{
	settings->setValue("RationalFormat",b);
}

float Preferences::getDefaultRotationX() const
{
	return settings->value("DefaultRotationX",35.0).toFloat();
}

float Preferences::getDefaultRotationY() const
{
	return settings->value("DefaultRotationY",0.0).toFloat();
}

float Preferences::getDefaultRotationZ() const
{
	return settings->value("DefaultRotationZ",35.0).toFloat();
}

float Preferences::getDefaultX() const
{
	return settings->value("DefaultX",0.0).toFloat();
}

float Preferences::getDefaultZ() const
{
	return settings->value("DefaultZ",0.0).toFloat();
}

float Preferences::getDefaultDistance() const
{
	return settings->value("DefaultDistance",500.0).toFloat();
}

void Preferences::setDefaultRotationX(float x)
{
	settings->setValue("DefaultRotationX",x);
}

void Preferences::setDefaultRotationY(float y)
{
	settings->setValue("DefaultRotationY",y);
}

void Preferences::setDefaultRotationZ(float z)
{
	settings->setValue("DefaultRotationZ",z);
}

void Preferences::setDefaultX(float x)
{
	settings->setValue("DefaultX",x);
}

void Preferences::setDefaultZ(float z)
{
	settings->setValue("DefaultZ",z);
}

void Preferences::setDefaultDistance(float d)
{
	settings->setValue("DefaultDistance",d);
}

QColor Preferences::getMarkedVertexColor() const
{
	return settings->value("MarkedVertexColor",QColor(0xff,0xff,0xff)).value<QColor>();
}

void Preferences::setMarkedVertexColor(QColor c)
{
	settings->setValue("MarkedVertexColor",c);
}

QColor Preferences::getVertexColor() const
{
	return settings->value("VertexColor",QColor(0xff,0xff,0xff)).value<QColor>();
}

void Preferences::setVertexColor(QColor c)
{
	settings->setValue("VertexColor",c);
}

QColor Preferences::getMarkedEdgeColor() const
{
	return settings->value("MarkedEdgeColor",QColor(0x00,0x00,0x00)).value<QColor>();
}

void Preferences::setMarkedEdgeColor(QColor c)
{
	settings->setValue("MarkedEdgeColor",c);
}

QColor Preferences::getEdgeColor() const
{
	return settings->value("EdgeColor",QColor(0x00,0x00,0x00)).value<QColor>();
}

void Preferences::setEdgeColor(QColor c)
{
	settings->setValue("EdgeColor",c);
}

QColor Preferences::getMarkedFacetColor() const
{
	return settings->value("MarkedFacetColor",QColor(0xff,0x55,0x00)).value<QColor>();
}

void Preferences::setMarkedFacetColor(QColor c)
{
	settings->setValue("MarkedFacetColor",c);
}

QColor Preferences::getFacetColor() const
{
	return settings->value("FacetColor",QColor(0xff,0xaa,0x00)).value<QColor>();
}

void Preferences::setFacetColor(QColor c)
{
	settings->setValue("FacetColor",c);
}

bool Preferences::getShowAxes() const
{
	return settings->value("ShowAxes",true).toBool();
}

void Preferences::setShowAxes(bool show)
{
	settings->setValue("ShowAxes",show);
}

bool Preferences::getShowEdges() const
{
	return settings->value("ShowEdges",true).toBool();
}

void Preferences::setShowEdges(bool show)
{
	settings->setValue("ShowEdges",show);
}

bool Preferences::getSkeleton() const
{
	return settings->value("Skeleton",false).toBool();
}

void Preferences::setSkeleton(bool show)
{
	settings->setValue("Skeleton",show);
}

bool Preferences::getShowBase() const
{
	return settings->value("ShowBase",true).toBool();
}

void Preferences::setShowBase(bool show)
{
	settings->setValue("ShowBase",show);
}

bool Preferences::getShowPrintArea() const
{
	return settings->value("ShowPrintArea",true).toBool();
}

void Preferences::setShowPrintArea(bool show)
{
	settings->setValue("ShowPrintArea",show);
}

bool Preferences::getShowRulers() const
{
	return settings->value("ShowRulers",true).toBool();
}

void Preferences::setShowRulers(bool show)
{
	settings->setValue("ShowRulers",show);
}

bool Preferences::getShowEditor() const
{
	return settings->value("ShowEditor",true).toBool();
}

void Preferences::setShowEditor(bool show)
{
	settings->setValue("ShowEditor",show);
}

bool Preferences::getShowConsole() const
{
	return settings->value("ShowConsole",true).toBool();
}

void Preferences::setShowConsole(bool show)
{
	settings->setValue("ShowConsole",show);
}

bool Preferences::getShowProjects() const
{
	return settings->value("ShowProjects",true).toBool();
}

void Preferences::setShowProjects(bool show)
{
	settings->setValue("ShowProjects",show);
}

QPoint Preferences::getWindowPosition() const
{
	return settings->value("WindowPosition",QPoint(0,0)).toPoint();
}

void Preferences::setWindowPosition(QPoint p)
{
	settings->setValue("WindowPosition",p);
}

QSize Preferences::getWindowSize() const
{
	return settings->value("WindowSize",QSize(1000,600)).toSize();
}

void Preferences::setWindowSize(QSize s)
{
	settings->setValue("WindowSize",s);
}

float Preferences::getVertexSize() const
{
	return settings->value("VertexSize",0).toFloat();
}

void Preferences::setVertexSize(float s)
{
	settings->setValue("VertexSize",s);
}

float Preferences::getEdgeSize() const
{
	return settings->value("EdgeSize",1).toFloat();
}

void Preferences::setEdgeSize(float s)
{
	settings->setValue("EdgeSize",s);
}

bool Preferences::getAutoSaveOnCompile() const
{
	return settings->value("AutoSaveOnCompile",false).toBool();
}

void Preferences::setAutoSaveOnCompile(bool b)
{
	settings->setValue("AutoSaveOnCompile",b);
}

void Preferences::setCacheEnabled(bool b)
{
	settings->setValue("CacheEnabled",b);
}

bool Preferences::getCacheEnabled() const
{
	return settings->value("CacheEnabled",false).toBool();
}

QPointF Preferences::getPrintOrigin() const
{
	return settings->value("PrintOrigin", QPointF(-125.0, -105.0)).toPointF();
}

void Preferences::setPrintOrigin(QPointF s)
{
	return settings->setValue("PrintOrigin", s);
}

QVector3D Preferences::getPrintVolume() const
{
	QList<QVariant> v=settings->value("PrintVolume", QList<QVariant>({250.0,210.0,200.0})).toList();
	return QVector3D(v.at(0).toFloat(),v.at(1).toFloat(),v.at(2).toFloat());
}

void Preferences::setPrintVolume(QVector3D v)
{
	//Cast to double needed so QSettings formats strings nicely.
	QList<QVariant> d({(double)v.x(),(double)v.y(),(double)v.z()});
	settings->setValue("PrintVolume",d);
}

GLView::Appearance_t Preferences::getPrintBedAppearance() const
{
	int i=settings->value("PrintBedAppearance",0).toInt();
	return (GLView::Appearance_t)i;
}

void Preferences::setPrintBedAppearance(GLView::Appearance_t v)
{
	settings->setValue("PrintBedAppearance",(int)v);
}

void Preferences::syncDelete()
{
	if(instance) {
		instance->settings->sync();
		delete instance->settings;
		delete instance;
	}
}
