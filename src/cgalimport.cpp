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
#include <fstream>
#include <QRegExp>
#include <QStringList>
#include "script.h"
#include "treeevaluator.h"
#include "nodeevaluator.h"

CGALImport::CGALImport(Reporter* r)
{
	reporter=r;
}

Primitive* CGALImport::import(QString filename)
{
	QFileInfo file(filename);
	reporter->reportMessage(tr("Importing '%1'").arg(file.absoluteFilePath()));

	QString suffix=file.suffix().toLower();
	if(suffix=="off")
		return importOFF(file);
	if(suffix=="stl")
		return importSTL(file);
	if(suffix=="rcad"||suffix=="csg")
		return importRCAD(file);

	reporter->reportWarning(tr("unknown import type '%1'").arg(suffix));
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
		reporter->reportWarning(tr("Can't open import file '%1'").arg(fileinfo.absoluteFilePath()));
		return p;
	}

	struct {
		float i, j, k;
		float x1, y1, z1;
		float x2, y2, z2;
		float x3, y3, z3;
		unsigned short acount;
	}
	__attribute__((packed))
	data;

	const int datasize=sizeof(data);

	/* Detect binary STL by reading the header
	 * and check whether the size calculated
	 * from the header matches the file size */
	bool binary=false;
	int numfacets;
	QByteArray header=f.read(80);
	f.read((char*)&numfacets,4);
	if(f.size() == 80+4+(datasize*numfacets))
		binary=true;

	if(!binary && QString(header).startsWith("solid")) {
		f.seek(0);
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
					float x,y,z;
					bool ox,oy,oz;
					x=tokens[1].toFloat(&ox);
					y=tokens[2].toFloat(&oy);
					z=tokens[3].toFloat(&oz);
					if((ok=ox&&oy&&oz)) {
						CGAL::Point3 pt(x,y,z);
						p->appendVertex(pt);
					}
				}
				if(!ok) {
					reporter->reportWarning(tr("Can't parse vertex line '%1'").arg(line));
				}
			}
		}
	} else {
		while(1) {
			if(f.read((char*)&data, datasize) != datasize)
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
	return p;
}

Primitive* CGALImport::importRCAD(QFileInfo f)
{
	Script* s=parse(f.absoluteFilePath(),reporter,true);
	TreeEvaluator te(reporter);
	s->accept(te);

	Node* n = te.getRootNode();
	NodeEvaluator ne(reporter);
	n->accept(ne);
	delete n;

	return ne.getResult();
}
#endif
