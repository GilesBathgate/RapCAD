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
#ifndef PROJECT_H
#define PROJECT_H

#include <QList>
#include <QStandardItemModel>
#include <QString>
#include <QXmlStreamReader>

class Project : public QStandardItemModel
{
	Q_OBJECT
public:
	Project(QObject*);
	const QList<QString>& getSources() const;
	void parseProject(const QString&);
	void parseSource(QXmlStreamReader&);
	void parseName(QXmlStreamReader&);
	void writeProject(const QString&);
	void setName(const QString&);
	void createDefaultItems();
	void addSource(const QString& value);
	void createRootItem();
private:
	QList<QString> sources;
	QString name;
	QString projectFileName;
};

#endif // PROJECT_H
