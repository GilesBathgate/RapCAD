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
#ifdef USE_CGAL
#include "cgalimport.h"
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <fstream>
#include <QRegExp>
#include <QStringList>
#include <QXmlStreamReader>
#include "script.h"
#include "treeevaluator.h"
#include "nodeevaluator.h"
#include "contrib/qzipreader_p.h"

CGALImport::CGALImport(Reporter& r) : reporter(r)
{
}

Primitive* CGALImport::import(QString filename)
{
	QFileInfo file(filename);
	reporter.reportMessage(tr("Importing '%1'").arg(file.absoluteFilePath()));

	QString suffix=file.suffix().toLower();
	if(suffix=="off")
		return importOFF(file);
	if(suffix=="nef")
		return importNEF(file);
	if(suffix=="stl")
		return importSTL(file);
	if(suffix=="amf")
		return importAMF(file);
	if(suffix=="3mf")
		return import3MF(file);
	if(suffix=="rcad"||suffix=="csg")
		return importRCAD(file);

	reporter.reportWarning(tr("unknown import type '%1'").arg(suffix));
	return nullptr;
}

Primitive* CGALImport::importOFF(QFileInfo fileinfo)
{
	CGAL::Polyhedron3 poly;
	std::ifstream file(fileinfo.absoluteFilePath().toLocal8Bit().constData());
	file >> poly;
	file.close();

	auto* p=new CGALPrimitive(poly);
	p->setSanitized(false);
	return p;
}

Primitive* CGALImport::importNEF(QFileInfo fileinfo)
{
	CGAL::NefPolyhedron3 nef;
	std::ifstream file(fileinfo.absoluteFilePath().toLocal8Bit().constData());
	file >> nef;
	file.close();

	auto* p=new CGALPrimitive(nef);
	return p;
}

Primitive* CGALImport::importSTL(QFileInfo fileinfo)
{
	auto* p=new CGALPrimitive();
	p->setSanitized(false);
	QFile f(fileinfo.absoluteFilePath());
	if(!f.open(QIODevice::ReadOnly)) {
		reporter.reportWarning(tr("Can't open import file '%1'").arg(fileinfo.absoluteFilePath()));
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
					reporter.reportWarning(tr("Can't parse vertex line '%1'").arg(line));
				}
			}
		}
	} else {
		while(!f.atEnd()) {
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

Primitive* CGALImport::importAMF(QFileInfo fileinfo)
{
	auto* p=new CGALPrimitive();
	p->setSanitized(false);
	QFile f(fileinfo.absoluteFilePath());
	if(!f.open(QIODevice::ReadOnly)) {
		reporter.reportWarning(tr("Can't open import file '%1'").arg(fileinfo.absoluteFilePath()));
		return p;
	}

	QXmlStreamReader xml(&f);
	if(xml.readNextStartElement() && xml.name()=="amf") {
		while(xml.readNextStartElement()) {
			if(xml.name()=="object") {
				while(xml.readNextStartElement()) {
					if(xml.name()=="mesh") {
						while(xml.readNextStartElement()) {
							if(xml.name()=="vertices") {
								while(xml.readNextStartElement()) {
									if(xml.name()=="vertex") {
										while(xml.readNextStartElement()) {
											if(xml.name()=="coordinates") {
												CGAL::Scalar x=0.0,y=0.0,z=0.0;
												while(xml.readNextStartElement()) {
													if(xml.name()=="x") {
														x=to_decimal(xml.readElementText());
													} else if(xml.name()=="y") {
														y=to_decimal(xml.readElementText());
													} else if(xml.name()=="z") {
														z=to_decimal(xml.readElementText());
													} else {
														xml.skipCurrentElement();
													}
												}
												p->createVertex(x,y,z);
											} else {
												xml.skipCurrentElement();
											}
										}
									} else {
										xml.skipCurrentElement();
									}
								}
							} else if(xml.name()=="volume") {
								while(xml.readNextStartElement()) {
									if(xml.name()=="triangle") {
										int v1=0,v2=0,v3=0;
										while(xml.readNextStartElement()) {
											if(xml.name()=="v1") {
												v1=xml.readElementText().toInt();
											} else if(xml.name()=="v2") {
												v2=xml.readElementText().toInt();
											} else if(xml.name()=="v3") {
												v3=xml.readElementText().toInt();
											} else {
												xml.skipCurrentElement();
											}
										}
										Polygon* pg=p->createPolygon();
										pg->append(v1);
										pg->append(v2);
										pg->append(v3);
									} else {
										xml.skipCurrentElement();
									}
								}
							} else {
								xml.skipCurrentElement();
							}
						}
					} else {
						xml.skipCurrentElement();
					}
				}
			} else {
				xml.skipCurrentElement();
			}
		}
	}

	return p;
}

Primitive* CGALImport::import3MF(QFileInfo fileinfo)
{
	auto* p=new CGALPrimitive();
	p->setSanitized(false);
	QFile f(fileinfo.absoluteFilePath());
	if(!f.open(QIODevice::ReadOnly)) {
		reporter.reportWarning(tr("Can't open import file '%1'").arg(fileinfo.absoluteFilePath()));
		return p;
	}
	QZipReader zip(fileinfo.absoluteFilePath());
	QByteArray data=zip.fileData("3D/3dmodel.model");
	zip.close();
	QXmlStreamReader xml(data);
	if(xml.readNextStartElement() && xml.name() == "model") {
		while(xml.readNextStartElement()) {
			if(xml.name() == "resources") {
				while(xml.readNextStartElement()) {
					if(xml.name() == "object") {
						while(xml.readNextStartElement()) {
							if(xml.name() == "mesh") {
								while(xml.readNextStartElement()) {
									if(xml.name() == "vertices") {
										while(xml.readNextStartElement()) {
											if(xml.name() == "vertex") {
												CGAL::Scalar x=0.0,y=0.0,z=0.0;
												for(const auto& attr: xml.attributes()) {
													QStringRef n=attr.name();
													QStringRef v=attr.value();
													if(n == "x")
														x=to_decimal(v.toString());
													else if(n == "y")
														y=to_decimal(v.toString());
													else if(n == "z")
														z=to_decimal(v.toString());
												}
												p->createVertex(x,y,z);
											}
											xml.skipCurrentElement();
										}
									} else if(xml.name() == "triangles") {
										while(xml.readNextStartElement()) {
											if(xml.name() == "triangle") {
												int v1=0,v2=0,v3=0;
												for(const auto& attr: xml.attributes()) {
													QStringRef n=attr.name();
													QStringRef v=attr.value();
													if(n == "v1")
														v1=v.toInt();
													else if(n == "v2")
														v2=v.toInt();
													else if(n == "v3")
														v3=v.toInt();
												}
												Polygon* pg=p->createPolygon();
												pg->append(v1);
												pg->append(v2);
												pg->append(v3);
											}
											xml.skipCurrentElement();
										}
									} else {
										xml.skipCurrentElement();
									}
								}
							} else {
								xml.skipCurrentElement();
							}
						}
					} else {
						xml.skipCurrentElement();
					}
				}
			} else {
				xml.skipCurrentElement();
			}
		}
	}
	return p;
}

Primitive* CGALImport::importRCAD(QFileInfo f)
{
	Script s(reporter);
	s.parse(f);
	TreeEvaluator te(reporter);
	s.accept(te);

	Node* n = te.getRootNode();
	NodeEvaluator ne(reporter);
	n->accept(ne);
	delete n;

	return ne.getResult();
}
#endif
