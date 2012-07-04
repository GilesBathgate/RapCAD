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

private:
	Preferences();
	static Preferences* instance;
	QSettings* settings;
};

#endif // PREFERENCES_H
