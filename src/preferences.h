#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QSettings>
#include <QColor>

class Preferences
{
public:
	static Preferences* getInstance();
	static void syncDelete();

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

private:
	Preferences();
	static Preferences* instance;
	QSettings* settings;
};

#endif // PREFERENCES_H
