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

#include "project.h"
#include <QFile>

Project::Project()
{
}

void Project::parseProject(const QString& filename)
{
	auto* file=new QFile(filename);
	if(!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
		return;
	}

	QXmlStreamReader xml(file);
	while(!xml.atEnd() && !xml.hasError()) {
		QXmlStreamReader::TokenType token = xml.readNext();
		if(token == QXmlStreamReader::StartDocument) {
			continue;
		}
		if(token == QXmlStreamReader::StartElement) {
			if(xml.name() == "project") {
				continue;
			}
			if(xml.name() == "name") {
				parseName(xml);
			}
			if(xml.name() == "source") {
				parseSource(xml);
			}
		}
	}
	delete file;
}

void Project::parseName(QXmlStreamReader& xml)
{
	while(xml.tokenType() != QXmlStreamReader::EndElement) {
		QXmlStreamReader::TokenType token=xml.readNext();
		if(token == QXmlStreamReader::Characters) {
			name = xml.text().toString();
		}
	}
}

void Project::parseSource(QXmlStreamReader& xml)
{
	while(xml.tokenType() != QXmlStreamReader::EndElement) {
		QXmlStreamReader::TokenType token=xml.readNext();
		if(token == QXmlStreamReader::Characters) {
			sources.append(xml.text().toString());
		}
	}
}

void Project::writeProject(const QString& filename)
{
	auto* file=new QFile(filename);
	if(!file->open(QIODevice::WriteOnly)) {
		return;
	}
	QXmlStreamWriter xml(file);
	xml.setAutoFormatting(true);
	xml.writeStartDocument();
	xml.writeStartElement("project");
	xml.writeAttribute("version","0.1");
	xml.writeTextElement("name",name);
	for(QString source: sources)
		xml.writeTextElement("source",source);
	xml.writeEndElement();
	xml.writeEndDocument();
	delete file;
}

QList<QString> Project::getSources() const
{
	return sources;
}

void Project::setSources(const QList<QString>& value)
{
	sources = value;
}
