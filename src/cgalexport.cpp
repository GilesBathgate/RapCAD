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

#include "cgal.h"

#include "cgalexport.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QString>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/print_wavefront.h>
#include <CGAL/IO/Polyhedron_VRML_2_ostream.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <fstream>
#include"cgalexplorer.h"
#include "onceonly.h"
#include "rmath.h"
#include "polyhedron.h"
#include "contrib/qzipwriter_p.h"

CGALExport::CGALExport(Primitive* p,Reporter& r) :
	reporter(r),
	primitive(p),
	id(0)
{
}

void CGALExport::exportResult(QString filename)
{
	QFileInfo file(filename);
	QString suffix=file.suffix().toLower();
	QString path=file.absoluteFilePath();
	if(suffix=="off")
		return exportOFF(path);
	if(suffix=="obj")
		return exportOBJ(path);
	if(suffix=="wrl")
		return exportVRML(path);
	if(suffix=="amf")
		return exportAMF(path);
	if(suffix=="3mf")
		return export3MF(path);
	if(suffix=="stl")
		return exportAsciiSTL(path);
	if(suffix=="csg")
		return exportCSG(path);
	if(suffix=="nef")
		return exportNEF(path);
	if(suffix=="svg")
		return exportSVG(path);
}

void CGALExport::exportVRML(QString filename)
{
	auto* pr=dynamic_cast<CGALPrimitive*>(primitive);
	if(!pr)
		return;

	CGAL::Polyhedron3* poly=pr->getPolyhedron();
	std::ofstream file(QFile::encodeName(filename));
	CGAL::VRML_2_ostream out(file);
	out << *poly;
	file.close();
	delete poly;
}

void CGALExport::exportOBJ(QString filename)
{
	auto* pr=dynamic_cast<CGALPrimitive*>(primitive);
	if(!pr)
		return;

	CGAL::Polyhedron3* poly=pr->getPolyhedron();
	std::ofstream file(QFile::encodeName(filename));
	print_polyhedron_wavefront(file,*poly);
	file.close();
	delete poly;
}

void CGALExport::exportOFF(QString filename)
{
	auto* pr=dynamic_cast<CGALPrimitive*>(primitive);
	if(!pr)
		return;

	//http://people.sc.fsu.edu/~jburkardt/data/off/off.html
	CGAL::Polyhedron3* poly=pr->getPolyhedron();
	std::ofstream file(QFile::encodeName(filename));
	file << *poly;
	file.close();
	delete poly;
}

typedef CGAL::Polyhedron3::Vertex_const_iterator VertexIterator;
typedef CGAL::Polyhedron3::Facet_const_iterator FacetIterator;
typedef CGAL::Polyhedron3::Halfedge_around_facet_const_circulator HalffacetCirculator;

static QList<CGAL::Triangle3> generateTriangles(CGAL::Polyhedron3* poly)
{
	QList<CGAL::Triangle3> triangles;
	for(FacetIterator fi=poly->facets_begin(); fi!=poly->facets_end(); ++fi) {
		HalffacetCirculator hc=fi->facet_begin();
		CGAL_assertion(circulator_size(hc)==3);
		CGAL::Triangle3 t((hc++)->vertex()->point(),
						  (hc++)->vertex()->point(),
						  (hc++)->vertex()->point());
		triangles.append(t);
	}
	return triangles;
}

void CGALExport::exportAsciiSTL(QString filename)
{
	auto* pr=dynamic_cast<CGALPrimitive*>(primitive);
	if(!pr)
		return;

	QFile data(filename);
	if(!data.open(QFile::WriteOnly | QFile::Truncate)) {
		reporter.reportWarning(tr("Can't write file '%1'").arg(filename));
		return;
	}

	CGAL::Polyhedron3* poly=pr->getPolyhedron();
	QTextStream output(&data);

	output << "solid RapCAD_Model\n";

	for(const auto& t: generateTriangles(poly)) {
		CGAL::Vector3 n=t.supporting_plane().orthogonal_vector();
		CGAL::Scalar ls=n.squared_length();
		CGAL::Scalar l=r_sqrt(ls);
		CGAL::Vector3 un=n/l;

		QString x1=to_string(t[0].x());
		QString y1=to_string(t[0].y());
		QString z1=to_string(t[0].z());
		QString x2=to_string(t[1].x());
		QString y2=to_string(t[1].y());
		QString z2=to_string(t[1].z());
		QString x3=to_string(t[2].x());
		QString y3=to_string(t[2].y());
		QString z3=to_string(t[2].z());
		QString nx=to_string(un.x());
		QString ny=to_string(un.y());
		QString nz=to_string(un.z());

		output << "  facet normal " << nx << " " << ny << " " << nz << "\n";
		output << "    outer loop\n";
		output << "      vertex " << x1 << " " << y1 << " " << z1 << "\n";
		output << "      vertex " << x2 << " " << y2 << " " << z2 << "\n";
		output << "      vertex " << x3 << " " << y3 << " " << z3 << "\n";
		output << "    endloop\n";
		output << "  endfacet\n";
	}

	output << "endsolid RapCAD_Model\n";
	output.flush();
	data.close();
	delete poly;
}

void CGALExport::exportAMF(QString filename)
{
	auto* file=new QFile(filename);
	if(!file->open(QIODevice::WriteOnly)) {
		reporter.reportWarning(tr("Can't write file '%1'").arg(filename));
		return;
	}

	QXmlStreamWriter xml(file);
	xml.setAutoFormatting(true);
	xml.writeStartDocument();
	xml.writeComment("Exported by RapCAD");
	xml.writeStartElement("amf");
	xml.writeAttribute("unit","millimeter");

	id=0;
	descendChildren(primitive,xml);

	xml.writeEndDocument();
	delete file;

}

void CGALExport::descendChildren(Primitive* p,QXmlStreamWriter& xml)
{
	auto* pr=dynamic_cast<CGALPrimitive*>(p);
	if(pr) {
		exportAMFObject(pr,xml);
	} else {
		for(Primitive* c: p->getChildren()) {
			descendChildren(c,xml);
		}
	}
}

void CGALExport::exportAMFObject(CGALPrimitive* p,QXmlStreamWriter& xml)
{
	CGAL::Polyhedron3* poly=p->getPolyhedron();

	xml.writeStartElement("object");
	xml.writeAttribute("id",QString().setNum(id++));
	xml.writeStartElement("mesh");
	xml.writeStartElement("vertices");

	int vertex_count=0;
	QMap<CGAL::Point3,int> vertices;
	for(VertexIterator vi=poly->vertices_begin(); vi!=poly->vertices_end(); ++vi) {
		CGAL::Point3 p=vi->point();
		xml.writeStartElement("vertex");
		xml.writeStartElement("coordinates");
		QString x,y,z;
		x=to_string(p.x());
		y=to_string(p.y());
		z=to_string(p.z());
		xml.writeTextElement("x",x);
		xml.writeTextElement("y",y);
		xml.writeTextElement("z",z);
		xml.writeEndElement(); //coordinates
		xml.writeEndElement(); //vertex

		vertices.insert(p,vertex_count++);
	}

	xml.writeEndElement(); //vertices

	xml.writeStartElement("volume");
	for(const auto& t: generateTriangles(poly)) {
		xml.writeStartElement("triangle");
		int v1,v2,v3;
		v1=vertices[t[0]];
		v2=vertices[t[1]];
		v3=vertices[t[2]];
		xml.writeTextElement("v1",QString().setNum(v1));
		xml.writeTextElement("v2",QString().setNum(v2));
		xml.writeTextElement("v3",QString().setNum(v3));
		xml.writeEndElement(); //triangle
	}
	xml.writeEndElement(); //volume

	xml.writeEndElement(); //mesh
	xml.writeEndElement(); //object
	delete poly;
}

void CGALExport::exportCSG(QString filename)
{
	QFile data(filename);
	if(!data.open(QFile::WriteOnly | QFile::Truncate)) {
		reporter.reportWarning(tr("Can't write file '%1'").arg(filename));
		return;
	}

	CGALExplorer e(primitive);
	CGALPrimitive* prim=e.getPrimitive();

	QTextStream output(&data);

	output << "polyhedron([";

	QList<CGAL::Point3> points=prim->getPoints();
	OnceOnly first;
	for(const auto& p: points) {
		if(!first())
			output << ",";
		output << to_string(p);
	}

	output << "],[";

	OnceOnly first_poly;
	for(CGALPolygon* poly: prim->getCGALPolygons()) {
		if(!first_poly())
			output << ",";
		output << "[" ;
		OnceOnly first_p;
		for(const auto& p: poly->getPoints()) {
			if(!first_p())
				output << ",";
			int i=points.indexOf(p); //eek, this could be slow on large models.
			output << i;
		}
		output << "]";
	}
	output << "]);";
	output.flush();
	data.close();
}

void CGALExport::export3MF(QString filename)
{
	auto* pr=dynamic_cast<CGALPrimitive*>(primitive);
	if(!pr)
		return;

	CGAL::Polyhedron3* poly=pr->getPolyhedron();

	QByteArray model;
	QXmlStreamWriter xml(&model);
	xml.setAutoFormatting(true);
	xml.writeStartDocument();
	xml.writeComment("Exported by RapCAD");

	xml.writeStartElement("model");
	xml.writeAttribute("unit","millimeter");
	xml.writeStartElement("resources");
	xml.writeStartElement("object");
	xml.writeAttribute("id","1");
	xml.writeAttribute("type","model");
	xml.writeStartElement("mesh");
	xml.writeStartElement("vertices");

	int vertex_count=0;
	QMap<CGAL::Point3,int> vertices;
	for(VertexIterator vi=poly->vertices_begin(); vi!=poly->vertices_end(); ++vi) {
		CGAL::Point3 p = vi->point();
		xml.writeStartElement("vertex");
		QString x,y,z;
		x=to_string(p.x());
		y=to_string(p.y());
		z=to_string(p.z());
		xml.writeAttribute("x",x);
		xml.writeAttribute("y",y);
		xml.writeAttribute("z",z);
		xml.writeEndElement(); //vertex

		vertices.insert(p,vertex_count++);
	}

	xml.writeEndElement(); //vertices

	xml.writeStartElement("triangles");
	for(const auto& t: generateTriangles(poly)) {
		xml.writeStartElement("triangle");
		int v1,v2,v3;
		v1=vertices[t[0]];
		v2=vertices[t[1]];
		v3=vertices[t[2]];
		xml.writeAttribute("v1",QString().setNum(v1));
		xml.writeAttribute("v2",QString().setNum(v2));
		xml.writeAttribute("v3",QString().setNum(v3));
		xml.writeEndElement(); //triangle
	}
	xml.writeEndElement(); //triangles
	xml.writeEndElement(); //mesh
	xml.writeEndElement(); //object
	xml.writeEndElement(); //resources

	xml.writeStartElement("build");
	xml.writeStartElement("item");
	xml.writeAttribute("objectid","1");
	xml.writeEndElement(); //item
	xml.writeEndElement(); //build

	xml.writeEndElement(); //model
	xml.writeEndDocument();

	QByteArray rels;
	QXmlStreamWriter relxml(&rels);
	relxml.writeDefaultNamespace("http://schemas.openxmlformats.org/package/2006/relationships");
	relxml.writeStartDocument();
	relxml.writeStartElement("Relationships");
	relxml.writeStartElement("Relationship");
	relxml.writeAttribute("Target","/3D/3dmodel.model");
	relxml.writeAttribute("Id","rel0");
	relxml.writeAttribute("Type","http://schemas.microsoft.com/3dmanufacturing/2013/01/3dmodel");
	relxml.writeEndElement(); //Relationship
	relxml.writeEndElement(); //Relationships
	relxml.writeEndDocument();

	QByteArray ctype;
	QXmlStreamWriter cxml(&ctype);
	cxml.writeStartDocument();
	cxml.writeDefaultNamespace("http://schemas.openxmlformats.org/package/2006/content-types");
	cxml.writeStartElement("Types");
	cxml.writeStartElement("Default");
	cxml.writeAttribute("Extension","rels");
	cxml.writeAttribute("ContentType","application/vnd.openxmlformats-package.relationships+xml");
	cxml.writeEndElement(); //Default
	cxml.writeStartElement("Default");
	cxml.writeAttribute("Extension","model");
	cxml.writeAttribute("ContentType","application/vnd.ms-package.3dmanufacturing-3dmodel+xml");
	cxml.writeEndElement(); //Default
	cxml.writeEndElement(); //Types
	cxml.writeEndDocument();

	QZipWriter zipwriter(filename);
	zipwriter.addFile("3D/3dmodel.model",model);
	zipwriter.addFile("_rels/.rels",rels);
	zipwriter.addFile("[Content_Types].xml",ctype);
	zipwriter.close();
	delete poly;
}

void CGALExport::exportNEF(QString f)
{
	auto* pr=dynamic_cast<CGALPrimitive*>(primitive);
	if(pr) {
		CGAL::NefPolyhedron3 nef=pr->getNefPolyhedron();
		std::ofstream file(QFile::encodeName(f));
		file << nef;
		file.close();
	}
}

void CGALExport::exportSVG(QString filename)
{
	if(primitive->isFullyDimentional()) {
		reporter.reportWarning(tr("Cannot export 3D volume as SVG"));
		return;
	}

	CGALExplorer e(primitive);
	CGALPrimitive* pr=e.getPrimitive();

	auto* file=new QFile(filename);
	if(!file->open(QIODevice::WriteOnly)) {
		return;
	}

	QXmlStreamWriter xml(file);
	xml.setAutoFormatting(true);
	xml.writeStartDocument();
	xml.writeStartElement("svg");
	xml.writeDefaultNamespace("http://www.w3.org/2000/svg");

	for(const auto& pg: pr->getCGALPolygons()) {
		xml.writeStartElement("polygon");
		QString points;
		OnceOnly first;
		for(const auto& pt: pg->getPoints()) {
			if(!first())
				points.append(" ");
			points.append(to_string(pt.x()));
			points.append(",");
			points.append(to_string(pt.y()));
		}
		xml.writeAttribute("points", points);
		xml.writeEndElement(); //polygon
	}

	xml.writeEndElement(); //svg
	xml.writeEndDocument();
	delete file;
}

#endif
