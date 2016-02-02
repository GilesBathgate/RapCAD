#include "project.h"
#include <QFile>

Project::Project()
{
}

void Project::parseProject(QString filename)
{
	QFile* file=new QFile(filename);
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

void Project::writeProject(QString filename)
{
	QFile* file=new QFile(filename);
	if(!file->open(QIODevice::WriteOnly)) {
		return;
	}
	QXmlStreamWriter xml(file);
	xml.setAutoFormatting(true);
	xml.writeStartDocument();
	xml.writeStartElement("project");
	xml.writeAttribute("version","0.1");
	xml.writeTextElement("name",name);
	foreach(QString source,sources)
	xml.writeTextElement("source",source);
	xml.writeEndElement();
	xml.writeEndDocument();
	delete file;
}
