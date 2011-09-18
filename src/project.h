#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QList>
#include <QXmlStreamReader>

class Project
{
public:
	Project();
	QList<QString> getSources();
	void setSources(QList<QString>);
	void parseProject(QString);
	void parseSource(QXmlStreamReader&);
	void parseName(QXmlStreamReader&);
	void writeProject(QString);
private:
	QList<QString> sources;
	QString name;
};

#endif // PROJECT_H
