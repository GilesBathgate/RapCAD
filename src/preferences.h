/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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

class Preferences
{
public:
	static Preferences* getInstance();
	static void syncDelete();

	int getPrecision();
	void setPrecision(int);

	bool getFunctionRounding();
	void setFunctionRounding(bool);

	bool getRationalFormat();
	void setRationalFormat(bool b);

	float getDefaultRotationX();
	void setDefaultRotationX(float);

	float getDefaultRotationY();
	void setDefaultRotationY(float);

	float getDefaultRotationZ();
	void setDefaultRotationZ(float);

	float getDefaultX();
	void setDefaultX(float);

	float getDefaultZ();
	void setDefaultZ(float);

	float getDefaultDistance();
	void setDefaultDistance(float);

	QColor getMarkedVertexColor();
	void setMarkedVertexColor(QColor);

	QColor getVertexColor();
	void setVertexColor(QColor);

	QColor getMarkedEdgeColor();
	void setMarkedEdgeColor(QColor);

	QColor getEdgeColor();
	void setEdgeColor(QColor);

	QColor getMarkedFacetColor();
	void setMarkedFacetColor(QColor);

	QColor getFacetColor();
	void setFacetColor(QColor);

	bool getShowAxes();
	void setShowAxes(bool);

	bool getShowEdges();
	void setShowEdges(bool);

	bool getSkeleton();
	void setSkeleton(bool);

	bool getShowBase();
	void setShowBase(bool);

	bool getShowPrintArea();
	void setShowPrintArea(bool);

	bool getShowRulers();
	void setShowRulers(bool);

	bool getShowEditor();
	void setShowEditor(bool);

	bool getShowConsole();
	void setShowConsole(bool);

	bool getShowProjects();
	void setShowProjects(bool);

	void setWindowPosition(QPoint);
	QPoint getWindowPosition();

	void setWindowSize(QSize);
	QSize getWindowSize();

	void setVertexSize(float);
	float getVertexSize();

	void setEdgeSize(float);
	float getEdgeSize();

	void setAutoSaveOnCompile(bool);
	bool getAutoSaveOnCompile();

	void setCacheEnabled(bool);
	bool getCacheEnabled();
private:
	Preferences();
	static Preferences* instance;
	QSettings* settings;
	int precision;
	void updatePrecision();
};

#endif // PREFERENCES_H
