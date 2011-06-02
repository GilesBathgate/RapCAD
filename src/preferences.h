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

private:
	Preferences();
	static Preferences* instance;
	QSettings* settings;
};

#endif // PREFERENCES_H
