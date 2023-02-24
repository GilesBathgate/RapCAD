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
#ifdef USE_CGAL
#include "cgalrenderer.h"
#include "cgalprimitive.h"
#include "preferences.h"
#include "primitive.h"
#include "rmath.h"
#include <CGAL/glu.h>

#ifndef GLAPIENTRY
#define GLAPIENTRY
#endif

using KernelF=CGAL::Simple_cartesian<GLfloat>;
using Mark=CGAL::NefPolyhedron3::Mark;

class PointF : public KernelF::Point_3
{
	Mark mark;
public:
	PointF(GLfloat x,GLfloat y,GLfloat z,const Mark& m) :
		KernelF::Point_3(x,y,z),mark(m) {}
	const Mark& getMark() const
	{
		return mark;
	}
};

class SegmentF : public KernelF::Segment_3
{
	Mark mark;
public:
	SegmentF(const PointF& p,const PointF& q,const Mark& m) :
		KernelF::Segment_3(p,q),mark(m) {}
	const Mark& getMark() const
	{
		return mark;
	}
};

class FacetF
{
	using Coord_vector=QVector<PointF>;
	using Cycle_vector=QVector<uint>;
	using Coord_const_iterator=Coord_vector::const_iterator;

	Coord_vector coordinates;
	Cycle_vector facetCycles;
	GLfloat normal[3];
	Mark mark;

public:

	FacetF()=default;
	FacetF(const FacetF&)=default;
	FacetF& operator=(const FacetF&)=default;
	FacetF(FacetF&&)=default;
	~FacetF()
	{
		coordinates.clear();
		facetCycles.clear();
	}

	void appendVertex(const PointF& p)
	{
		coordinates.append(p);
	}

	void setNormal(GLfloat x,GLfloat y,GLfloat z)
	{
		normal[0]=x; normal[1]=y; normal[2]=z;
	}

	GLfloat dx() const
	{
		return normal[0];
	}

	GLfloat dy() const
	{
		return normal[1];
	}

	GLfloat dz() const
	{
		return normal[2];
	}

	void setMark(Mark m)
	{
		mark=m;
	}

	const Mark& getMark() const
	{
		return mark;
	}

	void newFacetCycle()
	{
		facetCycles.append(coordinates.size());
	}

	uint facetCyclesSize() const
	{
		return facetCycles.size();
	}

	Coord_const_iterator facetCyclesBegin(uint i) const
	{
		CGAL_assertion(i<facetCyclesSize());
		if(i==0)
			return coordinates.begin();
		else
			return coordinates.begin()+facetCycles[i];
	}

	Coord_const_iterator facetCyclesEnd(uint i) const
	{
		CGAL_assertion(i<facetCyclesSize());
		if(i<facetCyclesSize()-1)
			return coordinates.begin()+facetCycles[i+1];
		else
			return coordinates.end();
	}
};

class NefConverter
{
	using SNC_structure=CGAL::NefPolyhedron3::SNC_structure;
	using Vertex_const_iterator=SNC_structure::Vertex_const_iterator;
	using Halfedge_const_iterator=SNC_structure::Halfedge_const_iterator;
	using Halffacet_const_iterator=SNC_structure::Halffacet_const_iterator;
	using Halffacet_cycle_const_iterator=SNC_structure::Halffacet_cycle_const_iterator;
	using SHalfedge_const_handle=SNC_structure::SHalfedge_const_handle;
	using Vertex_const_handle=SNC_structure::Vertex_const_handle;
	using Halfedge_const_handle=SNC_structure::Halfedge_const_handle;
	using Halffacet_const_handle=SNC_structure::Halffacet_const_handle;
	using SHalfedge_around_facet_const_circulator=SNC_structure::SHalfedge_around_facet_const_circulator;

	static PointF to_pointf(const CGAL::Point3& p,const Mark& m)
	{
		return PointF(CGAL::to_double(p.x()),CGAL::to_double(p.y()),CGAL::to_double(p.z()),m);
	}

	void convert(Vertex_const_handle v)
	{
		renderer.appendVertex(to_pointf(v->point(),v->mark()));
	}

	void convert(Halfedge_const_handle e)
	{
		const PointF& p=to_pointf(e->source()->point(),Mark());
		const PointF& q=to_pointf(e->twin()->source()->point(),Mark());
		renderer.appendEdge(SegmentF(p,q,e->mark()));
	}

	void convert(Halffacet_const_handle f)
	{
		FacetF g;
		Halffacet_cycle_const_iterator fc;
		CGAL_forall_facet_cycles_of(fc,f) {
			if(fc.is_shalfedge()) {
				g.newFacetCycle();
				SHalfedge_const_handle h=fc;
				SHalfedge_around_facet_const_circulator hc(h),he(hc);
				CGAL_For_all(hc,he) {
					g.appendVertex(to_pointf(hc->source()->source()->point(),Mark()));
				}
			}
		}
		const auto& v=f->plane().orthogonal_vector();
		const auto& n=v/r_sqrt(v.squared_length());
		g.setNormal(CGAL::to_double(n.x()),CGAL::to_double(n.y()),CGAL::to_double(n.z()));
		g.setMark(f->mark());
		renderer.appendFacet(g);
	}

	CGALRenderer& renderer;

public:
	NefConverter(CGALRenderer& r) :
		renderer(r)
	{}

	void convert(const CGAL::NefPolyhedron3& n)
	{
		Vertex_const_iterator v;
		CGAL_forall_vertices(v,*n.sncp()) {
			convert(v);
		}
		Halfedge_const_iterator e;
		CGAL_forall_edges(e,*n.sncp()) {
			convert(e);
		}
		Halffacet_const_iterator f;
		CGAL_forall_facets(f,*n.sncp()) {
			convert(f);
		}
	}

};

class DisplayList
{
	QOpenGLFunctions_1_0& functions;
	GLuint listId;
	GLsizei range;

public:
	DisplayList(QOpenGLFunctions_1_0& f,GLsizei r) :
		functions(f),
		range(r)
	{
		listId=functions.glGenLists(range);
	}

	~DisplayList()
	{
		functions.glDeleteLists(listId,range);
	}

	GLuint getId(GLuint i)
	{
		CGAL_assertion((GLsizei)i<range);
		return listId+i;
	}
};

CGALRenderer::CGALRenderer(Reporter& r,Primitive& pr) :
	reporter(r),
	primitive(pr),
	simpleRenderer(pr),
	displayList(nullptr),
	vertexSize(0.0F),
	edgeSize(0.0F)
{
	loadPreferences();
	descendChildren(primitive);
}

CGALRenderer::~CGALRenderer()
{
	delete displayList;
}

void CGALRenderer::descendChildren(Primitive& p)
{
	auto* pr=dynamic_cast<CGALPrimitive*>(&p);
	if(pr) {
		NefConverter c(*this);
		c.convert(pr->getNefPolyhedron());
	} else {
		for(Primitive* c: p.getChildren())
			descendChildren(*c);
	}
}

void CGALRenderer::loadPreferences()
{
	auto& p=Preferences::getInstance();
	markedVertexColor=p.getMarkedVertexColor();
	vertexColor=p.getVertexColor();
	markedEdgeColor=p.getMarkedEdgeColor();
	edgeColor=p.getEdgeColor();
	markedFacetColor=p.getMarkedFacetColor();
	facetColor=p.getFacetColor();
	vertexSize=p.getVertexSize();
	edgeSize=p.getEdgeSize();
}

void CGALRenderer::preferencesUpdated()
{
	loadPreferences();
	delete displayList;
	displayList=nullptr;
}

void CGALRenderer::setCompiling(bool value)
{
	if(value) {
		desaturate(markedVertexColor);
		desaturate(vertexColor);
		desaturate(markedEdgeColor);
		desaturate(edgeColor);
		desaturate(markedFacetColor);
		desaturate(facetColor);
	} else {
		loadPreferences();
	}
	delete displayList;
	displayList=nullptr;
}

void CGALRenderer::desaturate(QColor& c)
{
	c=QColor::fromHsv(c.hue(),0,c.value());
}

void CGALRenderer::drawVertices(QOpenGLFunctions_1_0& f) const
{
	const float p=getVertexSize();
	if(p==0) return;
	for(const auto& v : getVertices()) {
		const QColor& c=getVertexColor(v.getMark());
		f.glPointSize(p);
		f.glColor3ub(c.red(),c.green(),c.blue());
		f.glBegin(GL_POINTS);
		f.glVertex3f(v.x(),v.y(),v.z());
		f.glEnd();
	}
}

void CGALRenderer::drawEdges(QOpenGLFunctions_1_0& f) const
{
	const float w=getEdgeSize();
	if(w==0) return;
	for(const auto& e : getEdges()) {
		auto p=e.source(),q=e.target();
		const QColor& c=getEdgeColor(e.getMark());
		f.glLineWidth(w);
		f.glColor3ub(c.red(),c.green(),c.blue());
		f.glBegin(GL_LINE_STRIP);
		f.glVertex3f(p.x(),p.y(),p.z());
		f.glVertex3f(q.x(),q.y(),q.z());
		f.glEnd();
	}
}

struct PolygonData {
	Reporter& reporter;
	QOpenGLFunctions_1_0& functions;
	QList<PointF>& extraPoints;
	const FacetF& facet;
};

inline void vertexCallback(GLvoid* vertexData,GLvoid* polygonData)
{
	auto& v=*static_cast<PointF*>(vertexData);
	auto& d=*static_cast<PolygonData*>(polygonData);
	auto& f=d.functions;
	auto& fc=d.facet;
	f.glNormal3f(fc.dx(),fc.dy(),fc.dz());
	f.glVertex3f(v.x(),v.y(),v.z());
}

inline void combineCallback(GLdouble v[3],GLvoid*[4],GLfloat[4],GLvoid** dataOut,GLvoid* polygonData)
{
	auto& points=static_cast<PolygonData*>(polygonData)->extraPoints;
	*dataOut=&points.emplaceBack(v[0],v[1],v[2],Mark());
}

inline void beginCallback(GLenum which,GLvoid* data)
{
	auto& f=static_cast<PolygonData*>(data)->functions;
	f.glBegin(which);
}

inline void endCallback(GLvoid* data)
{
	auto& f=static_cast<PolygonData*>(data)->functions;
	f.glEnd();
}

inline void errorCallback(GLenum errorCode,GLvoid* data)
{
	auto& r=static_cast<PolygonData*>(data)->reporter;
	r.reportTesselationError(QString::fromLocal8Bit(gluErrorString(errorCode)));
}

void CGALRenderer::drawFacets(QOpenGLFunctions_1_0& f) const
{
	GLUtesselator* t=gluNewTess();
	using Callback=void (GLAPIENTRY*)(void);
	gluTessCallback(t,GLenum(GLU_TESS_VERTEX_DATA),(Callback)&vertexCallback);
	gluTessCallback(t,GLenum(GLU_TESS_COMBINE_DATA),(Callback)&combineCallback);
	gluTessCallback(t,GLenum(GLU_TESS_BEGIN_DATA),(Callback)&beginCallback);
	gluTessCallback(t,GLenum(GLU_TESS_END_DATA),(Callback)&endCallback);
	gluTessCallback(t,GLenum(GLU_TESS_ERROR_DATA),(Callback)&errorCallback);
	gluTessProperty(t,GLenum(GLU_TESS_WINDING_RULE),GLU_TESS_WINDING_POSITIVE);

	for(const auto& fc : getFacets()) {
		const QColor& c=getFacetColor(fc.getMark());
		f.glColor3ub(c.red(),c.green(),c.blue());

		QList<PointF> extraPoints;
		PolygonData data{reporter,f,extraPoints,fc};
		gluTessBeginPolygon(t,&data);
		gluTessNormal(t,fc.dx(),fc.dy(),fc.dz());
		for(uint i=0; i<fc.facetCyclesSize(); ++i) {
			gluTessBeginContour(t);
			for(auto cit=fc.facetCyclesBegin(i); cit!=fc.facetCyclesEnd(i); ++cit) {
				const PointF& p=*cit;
				GLdouble c[] {p.x(),p.y(),p.z()};
				gluTessVertex(t,c,(GLvoid*)&p);
			}
			gluTessEndContour(t);
		}
		gluTessEndPolygon(t);
	}

	gluDeleteTess(t);

}

void CGALRenderer::appendVertex(const PointF& p)
{
	vertices.append(p);
}

void CGALRenderer::appendEdge(const SegmentF& s)
{
	edges.append(s);
}

void CGALRenderer::appendFacet(const FacetF& f)
{
	facets.append(f);
}

const QList<PointF>& CGALRenderer::getVertices() const
{
	return vertices;
}

const QList<SegmentF>& CGALRenderer::getEdges() const
{
	return edges;
}

const QList<FacetF>& CGALRenderer::getFacets() const
{
	return facets;
}

void CGALRenderer::fillDisplayLists(QOpenGLFunctions_1_0& f)
{
	f.glNewList(displayList->getId(0),GL_COMPILE);
	drawVertices(f);
	f.glEndList();

	f.glNewList(displayList->getId(1),GL_COMPILE);
	drawEdges(f);
	f.glEndList();

	f.glNewList(displayList->getId(2),GL_COMPILE);
	drawFacets(f);
	f.glEndList();
}

void CGALRenderer::paint(QOpenGLFunctions_1_0& f,bool skeleton,bool showedges)
{
	if(!displayList) {
		displayList=new DisplayList(f,3);
		fillDisplayLists(f);
	}
	if(!skeleton) {
		f.glCallList(displayList->getId(2));
	}
	if(skeleton||showedges) {
		f.glDisable(GL_LIGHTING);
		f.glCallList(displayList->getId(1));
		f.glCallList(displayList->getId(0));
		f.glEnable(GL_LIGHTING);
	}

	simpleRenderer.paint(f,skeleton,showedges);

}

void CGALRenderer::locate(const QVector3D& s,const QVector3D& t)
{
	const Point& p=primitive.locate(Point(s.x(),s.y(),s.z()),Point(t.x(),t.y(),t.z()));
	reporter.reportMessage(to_string(p));
}

const QColor& CGALRenderer::getVertexColor(bool mark) const
{
	return mark ? markedVertexColor : vertexColor;
}

const QColor& CGALRenderer::getEdgeColor(bool mark) const
{
	return mark ? markedEdgeColor : edgeColor;
}

const QColor& CGALRenderer::getFacetColor(bool mark) const
{
	return mark ? markedFacetColor : facetColor;
}

GLfloat CGALRenderer::getVertexSize() const
{
	return vertexSize;
}

GLfloat CGALRenderer::getEdgeSize() const
{
	return edgeSize;
}
#endif
