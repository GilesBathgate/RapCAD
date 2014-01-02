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
#include "cgal.h"
#include "cgalexport.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QString>
#include <QXmlStreamWriter>
#include <CGAL/IO/Polyhedron_iostream.h>
#include"cgalexplorer.h"
#include "onceonly.h"

CGALExport::CGALExport(CGALPrimitive* p)
{
	primitive=p;
}

void CGALExport::exportResult(QString filename)
{
	QFileInfo file(filename);
	QString suffix=file.suffix().toLower();
	QString path=file.absoluteFilePath();
	if(suffix=="off")
		return exportOFF(path);
	if(suffix=="amf")
		return exportAMF(path);
	if(suffix=="stl")
		return exportAsciiSTL(path);
	if(suffix=="csg")
		return exportCSG(path);
}

void CGALExport::exportOFF(QString filename)
{
	//http://people.sc.fsu.edu/~jburkardt/data/off/off.html
	CGAL::Polyhedron3* poly=primitive->getPolyhedron();
	std::ofstream file(filename.toLocal8Bit().constData());
	file << *poly;
	file.close();
}

typedef CGAL::Polyhedron3::Vertex Vertex;
typedef CGAL::Polyhedron3::Vertex_const_iterator VertexIterator;
typedef CGAL::Polyhedron3::Facet_const_iterator FacetIterator;
typedef CGAL::Polyhedron3::Halfedge_around_facet_const_circulator HalffacetCirculator;

template<class NT>
inline NT inexact_sqrt(NT const& n)
{
	return NT(sqrt(to_double(n)));
}

void CGALExport::exportAsciiSTL(QString filename)
{
	CGAL::Polyhedron3* poly=primitive->getPolyhedron();

	QFile data(filename);
	if(!data.open(QFile::WriteOnly | QFile::Truncate)) {
		//error
		return;
	}
	QTextStream output(&data);
	//These settings are more conservative and hopefully offer
	//better compatability
	output.setRealNumberPrecision(6);
	output.setRealNumberNotation(QTextStream::ScientificNotation);

	output << "solid RapCAD_Model\n";

	for(FacetIterator fi = poly->facets_begin(); fi != poly->facets_end(); ++fi) {
		HalffacetCirculator hc = fi->facet_begin();
		HalffacetCirculator he = hc;
		Vertex v1, v2, v3;
		v1 = *VertexIterator((hc++)->vertex());
		v3 = *VertexIterator((hc++)->vertex());
		do {
			v2 = v3;
			v3 = *VertexIterator((hc++)->vertex());
			CGAL::Point3 p1,p2,p3;
			p1=v1.point();
			p2=v2.point();
			p3=v3.point();
			if(p1 == p2 || p1 == p3 || p2 == p3)
				continue;

			CGAL::Vector3 n=CGAL::orthogonal_vector(p1,p2,p3);
			CGAL::FT ls=n.squared_length();
			CGAL::FT l=inexact_sqrt(ls);
			CGAL::Vector3 un=n/l;

			double x1 = to_double(p1.x());
			double y1 = to_double(p1.y());
			double z1 = to_double(p1.z());
			double x2 = to_double(p2.x());
			double y2 = to_double(p2.y());
			double z2 = to_double(p2.z());
			double x3 = to_double(p3.x());
			double y3 = to_double(p3.y());
			double z3 = to_double(p3.z());
			double nx=to_double(un.x());
			double ny=to_double(un.y());
			double nz=to_double(un.z());

			output << "  facet normal " << nx << " " << ny << " " << nz << "\n";
			output << "    outer loop\n";
			output << "      vertex " << x1 << " " << y1 << " " << z1 << "\n";
			output << "      vertex " << x2 << " " << y2 << " " << z2 << "\n";
			output << "      vertex " << x3 << " " << y3 << " " << z3 << "\n";
			output << "    endloop\n";
			output << "  endfacet\n";

		} while(hc != he);
	}

	output << "endsolid RapCAD_Model\n";
	output.flush();
	data.close();
}

void CGALExport::exportAMF(QString filename)
{
	//currently does not support multi material - sk12/04/07
	CGAL::Polyhedron3* poly=primitive->getPolyhedron();

	QFile* file=new QFile(filename);
	if(!file->open(QIODevice::WriteOnly)) {
		return;
	}

	QXmlStreamWriter xml(file);
	xml.setAutoFormatting(true);
	xml.writeStartDocument();
	xml.writeComment("Exported by RapCAD");
	xml.writeStartElement("amf");
	xml.writeAttribute("unit","millimeter");

	xml.writeStartElement("object");
	xml.writeAttribute("id","0");
	xml.writeStartElement("mesh");
	xml.writeStartElement("vertices");

	QList<CGAL::Point3> vertices;
	QList<CGAL::Triangle3> triangles;

	for(FacetIterator fi = poly->facets_begin(); fi != poly->facets_end(); ++fi) {
		HalffacetCirculator hc = fi->facet_begin();
		HalffacetCirculator he = hc;
		Vertex v1, v2, v3;
		v1 = *VertexIterator((hc++)->vertex());
		v3 = *VertexIterator((hc++)->vertex());
		do {
			v2 = v3;
			v3 = *VertexIterator((hc++)->vertex());
			CGAL::Point3 p1,p2,p3;
			p1=v1.point();
			p2=v2.point();
			p3=v3.point();

			if(!vertices.contains(p1))
				vertices.append(p1);
			if(!vertices.contains(p2))
				vertices.append(p2);
			if(!vertices.contains(p3))
				vertices.append(p3);

			if(p1 == p2 || p1 == p3 || p2 == p3)
				continue;

			CGAL::Triangle3 t(p1,p2,p3);
			triangles.append(t);

		} while(hc != he);
	}

	foreach(CGAL::Point3 p,vertices) {
		xml.writeStartElement("vertex");
		xml.writeStartElement("coordinates");
		double x,y,z;
		x=to_double(p.x());
		y=to_double(p.y());
		z=to_double(p.z());
		xml.writeTextElement("x",QString().setNum(x));
		xml.writeTextElement("y",QString().setNum(y));
		xml.writeTextElement("z",QString().setNum(z));
		xml.writeEndElement(); //coordinates
		xml.writeEndElement(); //vertex
	}

	xml.writeEndElement(); //vertices

	xml.writeStartElement("volume");
	foreach(CGAL::Triangle3 t, triangles) {
		xml.writeStartElement("triangle");
		int v1,v2,v3;
		v1=vertices.indexOf(t[0]);
		v2=vertices.indexOf(t[1]);
		v3=vertices.indexOf(t[2]);
		xml.writeTextElement("v1",QString().setNum(v1));
		xml.writeTextElement("v2",QString().setNum(v2));
		xml.writeTextElement("v3",QString().setNum(v3));
		xml.writeEndElement(); //triangle
	}
	xml.writeEndElement(); //volume

	xml.writeEndElement(); //mesh
	xml.writeEndElement(); //object
	xml.writeEndDocument();
	delete file;

}

void CGALExport::exportCSG(QString filename)
{
	CGALExplorer* e=new CGALExplorer(primitive);

	CGALPrimitive* prim=e->getPrimitive();

	QFile data(filename);
	if(!data.open(QFile::WriteOnly | QFile::Truncate)) {
		//error
		return;
	}

	QTextStream output(&data);

	output << "polyhedron([";

	QList<CGAL::Point3> points=prim->getPoints();
	OnceOnly first;
	foreach(CGAL::Point3 p,points) {
		if(!first())
			output << ",";
		double x,y,z;
		x=to_double(p.x());
		y=to_double(p.y());
		z=to_double(p.z());
		Point pt(x,y,z);
		output << pt.toString();
	}

	output << "],[";

	OnceOnly first_poly;
	foreach(CGALPolygon* poly, prim->getPolygons()) {
		if(!first_poly())
			output << ",";
		output << "[" ;
		OnceOnly first_p;
		foreach(CGAL::Point3 p, poly->getPoints()) {
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
#endif
