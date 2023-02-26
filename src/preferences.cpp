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
#include "preferences.h"

#include <cmath>
static constexpr double LOG10_2=0.30102999566398119521; /* log10(2) = log base 10 of 2 */

#ifdef USE_CGAL
#include <CGAL/config.h>
#include <CGAL/exceptions.h>
#include <mpfr.h>
namespace CGAL {
#if CGAL_VERSION_NR >= CGAL_VERSION_NUMBER(5,6,0)
extern void set_use_assertions(bool);
#endif
}
#endif

class Settings :
	public AbstractSettings,
	private QSettings
{
	using Base=QSettings;
public:
	void sync() override
	{
		Base::sync();
	}

	QVariant value(const QString& k,const QVariant& d) const override
	{
		return Base::value(k,d);
	}

	void setValue(const QString& k,const QVariant& v) override
	{
		Base::setValue(k,v);
	}

	void clear() override
	{
		Base::clear();
	}
};

class TemporarySettings :
	public AbstractSettings,
	private QMap<QString,QVariant>
{
	using Base=QMap<QString,QVariant>;
	AbstractSettings* realSettings;
public:
	TemporarySettings(AbstractSettings* s) : realSettings(s) {}

	void sync() override
	{
		for(const auto& k: Base::keys()) {
			const auto& v=Base::value(k);
			realSettings->setValue(k,v);
		}
		Base::clear();
	}

	QVariant value(const QString& k,const QVariant& d) const override
	{
		return realSettings->value(k,d);
	}

	void setValue(const QString& k,const QVariant& v) override
	{
		Base::insert(k,v);
	}

	void clear() override
	{
		Base::clear();
	}
};

Preferences& Preferences::getInstance()
{
	static Preferences instance(new Settings);
	return instance;
}

Preferences& Preferences::getTemporary()
{
	static Preferences temporary(new TemporarySettings(getInstance().settings));
	temporary.clear();
	return temporary;
}

void Preferences::clear()
{
	settings->clear();
}

void Preferences::sync()
{
	settings->sync();
}

Preferences::Preferences(AbstractSettings* s) :
	settings(s),
	precision(0)
{
	updateAssertions();
	updatePrecision();
}

Preferences::~Preferences()
{
	//Ensure preferences are saved.
	settings->sync();
	delete settings;
}

void Preferences::updatePrecision()
{
	precision=getSignificandBits();
#ifdef USE_CGAL
	try {
		mpfr_set_default_prec(precision);
	} catch(CGAL::Assertion_exception&) {
		//Ignore
	}
#endif
}

void Preferences::updateAssertions() const
{
#ifdef USE_CGAL
#if CGAL_VERSION_NR >= CGAL_VERSION_NUMBER(5,6,0)
	const bool b=getUseCGALAssertions();
	CGAL::set_use_assertions(b);
#endif
#endif
}

bool Preferences::getUseCGALAssertions() const
{
	return settings->value("UseCGALAssertions",true).toBool();
}

void Preferences::setUseCGALAssertions(bool b)
{
	settings->setValue("UseCGALAssertions",b);
	updateAssertions();
}

bool Preferences::getSoftwareOpenGL() const
{
#ifdef Q_OS_WIN
	return settings->value("SoftwareOpenGL",true).toBool();
#else
	return false;
#endif
}

void Preferences::setSoftwareOpenGL(bool value)
{
	settings->setValue("SoftwareOpenGL",value);
}

bool Preferences::getVisibleWhiteSpace() const
{
	return settings->value("VisibleWhiteSpace",false).toBool();
}

void Preferences::setVisibleWhiteSpace(bool value)
{
	settings->setValue("VisibleWhiteSpace",value);
}

int Preferences::getThreadPoolSize() const
{
	return settings->value("ThreadPoolSize",0).toInt();
}

void Preferences::setThreadPoolSize(int value)
{
	settings->setValue("ThreadPoolSize",value);
}

QString Preferences::getIndent() const
{
	return settings->value("Indent","\t").toString();
}

void Preferences::setIndent(const QString& value)
{
	settings->setValue("Indent", value);
}

QString Preferences::getCAMScript() const
{
	return settings->value("CAMScript","reprap.rcam").toString();
}

void Preferences::setCAMScript(const QString& value)
{
	settings->setValue("CAMScript",value);
}

bool Preferences::getTranslateOrigin() const
{
	return settings->value("TranslateOrigin",false).toBool();
}

void Preferences::setTranslateOrigin(bool value)
{
	settings->setValue("TranslateOrigin",value);
}

bool Preferences::getDarkTheme() const
{
	return settings->value("DarkTheme",false).toBool();
}

void Preferences::setDarkTheme(bool b)
{
	settings->setValue("DarkTheme",b);
}

bool Preferences::getShowGCODEButton() const
{
	return settings->value("ShowGCODEButton",false).toBool();
}

void Preferences::setShowGCODEButton(bool value)
{
	settings->setValue("ShowGCODEButton",value);
}

bool Preferences::getHighlightLine() const
{
	return settings->value("HighlightLine",false).toBool();
}

void Preferences::setHighlightLine(bool value)
{
	settings->setValue("HighlightLine",value);
}

QString Preferences::getLaunchCommand() const
{
	return settings->value("LaunchCommand","").toString();
}

void Preferences::setLaunchCommand(const QString& value)
{
	settings->setValue("LaunchCommand",value);
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
	const QString& family=settings->value("EditorFont.Family","Courier").toString();
	const int size=settings->value("EditorFont.Size",10).toInt();

	return QFont(family,size);
}

void Preferences::setEditorFont(const QFont& value)
{
	settings->setValue("EditorFont.Family",value.family());
	settings->setValue("EditorFont.Size",value.pointSize());
}

Precision Preferences::getPrecision() const
{
	const int p=settings->value("Precision",2).toInt();
	return static_cast<Precision>(p);
}

void Preferences::setPrecision(Precision p)
{
	settings->setValue("Precision",static_cast<int>(p));
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

Rounding Preferences::getFunctionRounding() const
{
	const int rounding=settings->value("FunctionRounding",0).toInt();
	return static_cast<Rounding>(rounding);
}

void Preferences::setFunctionRounding(Rounding i)
{
	settings->setValue("FunctionRounding",static_cast<int>(i));
}

NumberFormats Preferences::getNumberFormat() const
{
	const int format=settings->value("NumberFormat",0).toInt();
	return static_cast<NumberFormats>(format);
}

void Preferences::setNumberFormat(NumberFormats i)
{
	settings->setValue("NumberFormat",static_cast<int>(i));
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

void Preferences::setMarkedVertexColor(const QColor& c)
{
	settings->setValue("MarkedVertexColor",c);
}

QColor Preferences::getVertexColor() const
{
	return settings->value("VertexColor",QColor(0xff,0xff,0xff)).value<QColor>();
}

void Preferences::setVertexColor(const QColor& c)
{
	settings->setValue("VertexColor",c);
}

QColor Preferences::getMarkedEdgeColor() const
{
	return settings->value("MarkedEdgeColor",QColor(0x00,0x00,0x00)).value<QColor>();
}

void Preferences::setMarkedEdgeColor(const QColor& c)
{
	settings->setValue("MarkedEdgeColor",c);
}

QColor Preferences::getEdgeColor() const
{
	return settings->value("EdgeColor",QColor(0x00,0x00,0x00)).value<QColor>();
}

void Preferences::setEdgeColor(const QColor& c)
{
	settings->setValue("EdgeColor",c);
}

QColor Preferences::getMarkedFacetColor() const
{
	return settings->value("MarkedFacetColor",QColor(0xff,0x55,0x00)).value<QColor>();
}

void Preferences::setMarkedFacetColor(const QColor& c)
{
	settings->setValue("MarkedFacetColor",c);
}

QColor Preferences::getFacetColor() const
{
	return settings->value("FacetColor",QColor(0xff,0xaa,0x00)).value<QColor>();
}

void Preferences::setFacetColor(const QColor& c)
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

bool Preferences::getShowExplorer() const
{
	return settings->value("ShowExplorer",false).toBool();
}

void Preferences::setShowExplorer(bool show)
{
	settings->setValue("ShowExplorer",show);
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

QPoint Preferences::getPrintOrigin() const
{
	return settings->value("PrintOrigin",QPoint(-125,-105)).toPoint();
}

void Preferences::setPrintOrigin(QPoint s)
{
	return settings->setValue("PrintOrigin",s);
}

QList<int> Preferences::getPrintVolume() const
{
	const QList<QVariant>& v=settings->value("PrintVolume",QList<QVariant>{250,210,200}).toList();
	return {v.at(0).toInt(),v.at(1).toInt(),v.at(2).toInt()};
}

void Preferences::setPrintVolume(QList<int> v)
{
	//Cast to int needed so QSettings formats strings nicely.
	const QList<QVariant> d{v.at(0),v.at(1),v.at(2)};
	settings->setValue("PrintVolume",d);
}

BedAppearance Preferences::getPrintBedAppearance() const
{
	const int i=settings->value("PrintBedAppearance",0).toInt();
	return static_cast<BedAppearance>(i);
}

void Preferences::setPrintBedAppearance(BedAppearance v)
{
	settings->setValue("PrintBedAppearance",static_cast<int>(v));
}
