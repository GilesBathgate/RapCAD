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

	double getDefaultRotationX();
	void setDefaultRotationX(double d);

	double getDefaultRotationZ();
	void setDefaultRotationZ(double d);

	double getDefaultX();
	void setDefaultX(double d);

	double getDefaultZ();
	void setDefaultZ(double d);

	double getDefaultDistance();
	void setDefaultDistance(double d);

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

	void setVertexSize(double);
	double getVertexSize();

	void setEdgeSize(double);
	double getEdgeSize();

	void setAutoSaveOnCompile(bool);
	bool getAutoSaveOnCompile();

private:
	Preferences();
	static Preferences* instance;
	QSettings* settings;
};

#endif // PREFERENCES_H
