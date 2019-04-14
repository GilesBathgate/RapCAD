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
#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QSettings>
#include <QColor>
#include <QPoint>
#include <QSize>
#include <QVector3D>
#include <QFont>
#include "ui/glview.h"

class Preferences
{
public:
	static Preferences* getInstance();
	static void syncDelete();

	int getPrecision() const;
	void setPrecision(int);

	int getDecimalPlaces() const;
	void setDecimalPlaces(int);

	int getSignificandBits() const;
	void setSignificandBits(int);

	int getFunctionRounding() const;
	void setFunctionRounding(int);

	bool getRationalFormat() const;
	void setRationalFormat(bool b);

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
	void setMarkedVertexColor(QColor);

	QColor getVertexColor() const;
	void setVertexColor(QColor);

	QColor getMarkedEdgeColor() const;
	void setMarkedEdgeColor(QColor);

	QColor getEdgeColor() const;
	void setEdgeColor(QColor);

	QColor getMarkedFacetColor() const;
	void setMarkedFacetColor(QColor);

	QColor getFacetColor() const;
	void setFacetColor(QColor);

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

	bool getShowProjects() const;
	void setShowProjects(bool);

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

	QPointF getPrintOrigin() const;
	void setPrintOrigin(QPointF s);

	QVector3D getPrintVolume() const;
	void setPrintVolume(QVector3D v);

	GLView::Appearance_t getPrintBedAppearance() const;
	void setPrintBedAppearance(GLView::Appearance_t);

	QFont getEditorFont() const;
	void setEditorFont(const QFont&);

	bool getShowTooltips() const;
	void setShowTooltips(bool);

	bool getHighlightLine() const;
	void setHighlightLine(bool value);

private:
	Preferences();
	void updatePrecision();

	static Preferences* instance;
	QSettings* settings;
	int precision;
};

#endif // PREFERENCES_H
