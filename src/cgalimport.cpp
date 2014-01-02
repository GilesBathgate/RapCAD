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
#if USE_CGAL
#include "cgalimport.h"
#include <CGAL/IO/Polyhedron_iostream.h>
#include <QRegExp>
#include <QStringList>
#include "script.h"
#include "treeevaluator.h"
#include "nodeevaluator.h"

CGALImport::CGALImport(QTextStream& out) : output(out)
{
}

Primitive* CGALImport::import(QString filename)
{
	QFileInfo file(filename);
	output << "Info: Importing '" << file.absoluteFilePath() << "'\n";
	output.flush();
	QString suffix=file.suffix().toLower();
	if(suffix=="off")
		return importOFF(file);
	if(suffix=="stl")
		return importSTL(file);
	if(suffix=="rcad"||suffix=="csg")
		return importRCAD(file);

	output << "Warning: Unknown import type '" << suffix << "'\n";
	return NULL;
}

Primitive* CGALImport::importOFF(QFileInfo fileinfo)
{
	CGAL::Polyhedron3 poly;
	std::ifstream file(fileinfo.absoluteFilePath().toLocal8Bit().constData());
	file >> poly;
	file.close();

	return new CGALPrimitive(poly);
}

Primitive* CGALImport::importSTL(QFileInfo fileinfo)
{
	CGALPrimitive* p=new CGALPrimitive();
	QFile f(fileinfo.absoluteFilePath());
	if(!f.open(QIODevice::ReadOnly)) {
		output << "Warning: Can't open import file '" << fileinfo.absoluteFilePath() << "'\n";
		return p;
	}

	QByteArray header=f.read(5);
	if(header.size()==5 && QString(header)=="solid") {
		QTextStream data(&f);
		QRegExp re=QRegExp("\\s*(vertex)?\\s+");
		while(!data.atEnd()) {
			QString line=data.readLine();
			if(line.contains("solid") || line.contains("facet") || line.contains("endloop"))
				continue;
			if(line.contains("outer loop")) {
				p->createPolygon();
				continue;
			}
			if(line.contains("vertex")) {
				QStringList tokens=line.split(re);
				bool ok=false;
				if(tokens.size()==4) {
					double x,y,z;
					bool ox,oy,oz;
					x=tokens[1].toDouble(&ox);
					y=tokens[2].toDouble(&oy);
					z=tokens[3].toDouble(&oz);
					if((ok=ox&&oy&&oz)) {
						CGAL::Point3 pt(x,y,z);
						p->appendVertex(pt);
					}
				}
				if(!ok) {
					output << "Warning: Can't parse vertex line '" << line << "'\n";
				}
			}
		}
	} else {
		f.read(80-5+4);
		while(1) {
			struct {
				float i, j, k;
				float x1, y1, z1;
				float x2, y2, z2;
				float x3, y3, z3;
				unsigned short acount;
			}
			__attribute__((packed))
			data;

			if(f.read((char*)&data, sizeof(data)) != sizeof(data))
				break;
			p->createPolygon();
			CGAL::Point3 v1(data.x1,data.y1,data.z1);
			p->appendVertex(v1);
			CGAL::Point3 v2(data.x2,data.y2,data.z2);
			p->appendVertex(v2);
			CGAL::Point3 v3(data.x3,data.y3,data.z3);
			p->appendVertex(v3);
		}
	}
	return p->buildPrimitive();
}

Primitive *CGALImport::importRCAD(QFileInfo f)
{
	Reporter* r=new Reporter(output);

	Script* s=parse(f.absoluteFilePath(),r);
	TreeEvaluator te(output);
	s->accept(te);

	Node* n = te.getRootNode();
	NodeEvaluator ne(output);
	n->accept(ne);
	delete n;

	delete r;

	return ne.getResult();
}
#endif
