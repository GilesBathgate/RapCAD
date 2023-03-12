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
#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "abstractsettings.h"
#include "bedappearance.h"
#include "decimal.h"
#include <QColor>
#include <QFont>
#include <QPoint>
#include <QSettings>
#include <QSize>
#include <QVector3D>

class Preferences
{
	Q_DISABLE_COPY_MOVE(Preferences)
public:
	static Preferences& getInstance();
	static Preferences& getTemporary();

	void clear();
	void sync();

	Precision getPrecision() const;
	void setPrecision(Precision);

	int getDecimalPlaces() const;
	void setDecimalPlaces(int);

	int getSignificandBits() const;
	void setSignificandBits(int);

	Rounding getFunctionRounding() const;
	void setFunctionRounding(Rounding);

	NumberFormats getNumberFormat() const;
	void setNumberFormat(NumberFormats);

	float getDefaultRotationX() const;
	void setDefaultRotationX(float);

	float getDefaultRotationY() const;
	void setDefaultRotationY(float);

	float getDefaultRotationZ() const;
	void setDefaultRotationZ(float);

	float getDefaultX() const;
	void setDefaultX(float);

	float getDefaultZ() const;
	void setDefaultZ(float);

	float getDefaultDistance() const;
	void setDefaultDistance(float);

	QColor getMarkedVertexColor() const;
	void setMarkedVertexColor(const QColor&);

	QColor getVertexColor() const;
	void setVertexColor(const QColor&);

	QColor getMarkedEdgeColor() const;
	void setMarkedEdgeColor(const QColor&);

	QColor getEdgeColor() const;
	void setEdgeColor(const QColor&);

	QColor getMarkedFacetColor() const;
	void setMarkedFacetColor(const QColor&);

	QColor getFacetColor() const;
	void setFacetColor(const QColor&);

	bool getShowAxes() const;
	void setShowAxes(bool);

	bool getShowEdges() const;
	void setShowEdges(bool);

	bool getSkeleton() const;
	void setSkeleton(bool);

	bool getShowBase() const;
	void setShowBase(bool);

	bool getShowPrintArea() const;
	void setShowPrintArea(bool);

	bool getShowRulers() const;
	void setShowRulers(bool);

	bool getShowEditor() const;
	void setShowEditor(bool);

	bool getShowConsole() const;
	void setShowConsole(bool);

	bool getShowExplorer() const;
	void setShowExplorer(bool);

	void setWindowPosition(QPoint);
	QPoint getWindowPosition() const;

	void setWindowSize(QSize);
	QSize getWindowSize() const;

	void setVertexSize(float);
	float getVertexSize() const;

	void setEdgeSize(float);
	float getEdgeSize() const;

	void setAutoSaveOnCompile(bool);
	bool getAutoSaveOnCompile() const;

	void setCacheEnabled(bool);
	bool getCacheEnabled() const;

	QPoint getPrintOrigin() const;
	void setPrintOrigin(QPoint);

	QList<int> getPrintVolume() const;
	void setPrintVolume(QList<int> v);

	BedAppearance getPrintBedAppearance() const;
	void setPrintBedAppearance(BedAppearance);

	QFont getEditorFont() const;
	void setEditorFont(const QFont&);

	bool getShowTooltips() const;
	void setShowTooltips(bool);

	bool getHighlightLine() const;
	void setHighlightLine(bool);

	QString getLaunchCommand() const;
	void setLaunchCommand(const QString&);

	bool getShowGCODEButton() const;
	void setShowGCODEButton(bool);

	QString getCAMScript() const;
	void setCAMScript(const QString&);

	bool getTranslateOrigin() const;
	void setTranslateOrigin(bool);

	bool getDarkTheme() const;
	void setDarkTheme(bool);

	QString getIndent() const;
	void setIndent(const QString&);

	int getThreadPoolSize() const;
	void setThreadPoolSize(int);

	bool getVisibleWhiteSpace() const;
	void setVisibleWhiteSpace(bool);

	bool getSoftwareOpenGL() const;
	void setSoftwareOpenGL(bool);

	bool getUseCGALAssertions() const;
	void setUseCGALAssertions(bool);

private:
	Preferences(AbstractSettings*);
	~Preferences();
	void updateAssertions() const;
	void updatePrecision();

	AbstractSettings* settings;
	int precision;
};

#endif // PREFERENCES_H
