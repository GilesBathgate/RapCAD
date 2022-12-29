/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2022 Giles Bathgate
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

CGALRenderer::CGALRenderer(Primitive* primitive) :
  simple(new SimpleRenderer(primitive)),
  vertexSize(0.0F),
  edgeSize(0.0F)
{
	loadPreferences();
	descendChildren(primitive);
}

CGALRenderer::~CGALRenderer()
{
	delete simple;
}

void CGALRenderer::descendChildren(Primitive* p)
{
	using converter=CGAL::OGL::Nef3_Converter<CGAL::NefPolyhedron3>;
	auto* pr=dynamic_cast<CGALPrimitive*>(p);
	if(pr) {
		converter::convert_to_OGLPolyhedron(pr->getNefPolyhedron(),this);
	} else if(p) {
		for(Primitive* c: p->getChildren())
			descendChildren(c);
	}
}

void CGALRenderer::loadPreferences()
{
	auto& p=Preferences::getInstance();
	setColor(markedVertexColor,p.getMarkedVertexColor());
	setColor(vertexColor,p.getVertexColor());
	setColor(markedEdgeColor,p.getMarkedEdgeColor());
	setColor(edgeColor,p.getEdgeColor());
	setColor(markedFacetColor,p.getMarkedFacetColor());
	setColor(facetColor,p.getFacetColor());
	vertexSize=p.getVertexSize();
	edgeSize=p.getEdgeSize();
}

void CGALRenderer::preferencesUpdated()
{
	loadPreferences();
	init_=false;
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
	init_=false;
}

void CGALRenderer::desaturate(CGAL::IO::Color& c)
{
	QColor rgb(c.red(),c.green(),c.blue());
	setColor(c,QColor::fromHsv(rgb.hue(),0,rgb.value()));
}

void CGALRenderer::drawVertices(QOpenGLFunctions_1_0& f,Vertex_iterator v) const
{
	float p=getVertexSize();
	if(p==0) return;
	CGAL::IO::Color c=getVertexColor(v->mark());
	f.glPointSize(p);
	f.glColor3ub(c.red(),c.green(),c.blue());
	f.glBegin(GL_POINTS);
	f.glVertex3f(v->x(),v->y(),v->z());
	f.glEnd();
}

void CGALRenderer::drawEdges(QOpenGLFunctions_1_0& f,Edge_iterator e) const
{
	float w=getEdgeSize();
	if(w==0) return;
	auto p=e->source(),q=e->target();
	CGAL::IO::Color c=getEdgeColor(e->mark());
	f.glLineWidth(w);
	f.glColor3ub(c.red(),c.green(),c.blue());
	f.glBegin(GL_LINE_STRIP);
	f.glVertex3f(p.x(),p.y(),p.z());
	f.glVertex3f(q.x(),q.y(),q.z());
	f.glEnd();
}

struct CallbackData
{
	QOpenGLFunctions_1_0& functions;
	float* normal;
};

inline void CGAL_GLU_TESS_CALLBACK vertexCallback(GLvoid* vertex,GLvoid* data)
{
	auto* v=static_cast<GLfloat*>(vertex);
	auto* d=static_cast<CallbackData*>(data);
	auto& f=d->functions;
	f.glNormal3fv(d->normal);
	f.glVertex3fv(v);
}

inline void CGAL_GLU_TESS_CALLBACK combineCallback(GLdouble vertex[3],GLvoid*[4],GLfloat[4],GLvoid** dataOut)
{
	static std::list<GLdouble*> cache;
	if (dataOut) {
		cache.push_back(vertex);
		*dataOut=vertex;
	} else {
		for (auto v : cache)
			delete[] v;
		cache.clear();
	}
}

inline void CGAL_GLU_TESS_CALLBACK beginCallback(GLenum which,GLvoid* data)
{
	auto& f=static_cast<CallbackData*>(data)->functions;
	f.glBegin(which);
}

inline void CGAL_GLU_TESS_CALLBACK endCallback(GLvoid* data)
{
	auto& f=static_cast<CallbackData*>(data)->functions;
	f.glEnd();
}

inline void CGAL_GLU_TESS_CALLBACK errorCallback(GLenum errorCode)
{
	const GLubyte* estring=gluErrorString(errorCode);
	std::cerr << "Tessellation Error: " << estring << std::endl;
}

void CGALRenderer::drawFacets(QOpenGLFunctions_1_0& f,Halffacet_iterator fc) const
{
	GLUtesselator* t=gluNewTess();
	using Callback=void (CGAL_GLU_TESS_CALLBACK *)(CGAL_GLU_TESS_DOTS);
	gluTessCallback(t,GLenum(GLU_TESS_VERTEX_DATA),(Callback)&vertexCallback);
	gluTessCallback(t,GLenum(GLU_TESS_COMBINE),(Callback)&combineCallback);
	gluTessCallback(t,GLenum(GLU_TESS_BEGIN_DATA),(Callback)&beginCallback);
	gluTessCallback(t,GLenum(GLU_TESS_END_DATA),(Callback)&endCallback);
	gluTessCallback(t,GLenum(GLU_TESS_ERROR),(Callback)&errorCallback);
	gluTessProperty(t,GLenum(GLU_TESS_WINDING_RULE),GLU_TESS_WINDING_POSITIVE);

	CGAL::OGL::DFacet::Coord_const_iterator cit;
	CGAL::IO::Color c=getFacetColor(fc->mark());
	f.glColor3ub(c.red(),c.green(),c.blue());
	CallbackData data{f,fc->normal()};
	gluTessBeginPolygon(t,&data);
	gluTessNormal(t,fc->dx(),fc->dy(),fc->dz());
	for(unsigned i=0; i < fc->number_of_facet_cycles(); ++i) {
		gluTessBeginContour(t);
		for(cit=fc->facet_cycle_begin(i); cit != fc->facet_cycle_end(i); ++cit) {
			double loc[3];
			loc[0]=(*cit)[0];
			loc[1]=(*cit)[1];
			loc[2]=(*cit)[2];
			gluTessVertex(t,loc,*cit);
		}
		gluTessEndContour(t);
	}
	gluTessEndPolygon(t);
	gluDeleteTess(t);
	combineCallback(nullptr,nullptr,nullptr,nullptr);
}

void CGALRenderer::fillDisplayLists(QOpenGLFunctions_1_0& f) {
	f.glNewList(object_list_+0,GL_COMPILE);
	Vertex_iterator v;
	for(v=vertices_.begin();v!=vertices_.end();++v)
		drawVertices(f,v);
	f.glEndList();

	f.glNewList(object_list_+1,GL_COMPILE);
	Edge_iterator e;
	for(e=edges_.begin();e!=edges_.end();++e)
		drawEdges(f,e);
	f.glEndList();

	f.glNewList(object_list_+2,GL_COMPILE);
	Halffacet_iterator fc;
	for(fc=halffacets_.begin();fc!=halffacets_.end();++fc)
		drawFacets(f,fc);
	f.glEndList();
}

void CGALRenderer::paint(QOpenGLFunctions_1_0& f,bool skeleton,bool showedges)
{
	if (!init_) {
		init_=true;
		object_list_=f.glGenLists(3);
		CGAL_assertion(object_list_);
		fillDisplayLists(f);
	}
	if(!skeleton) {
		f.glCallList(object_list_+2);
	}
	if(skeleton||showedges) {
		f.glDisable(GL_LIGHTING);
		f.glCallList(object_list_+1);
		f.glCallList(object_list_+0);
		f.glEnable(GL_LIGHTING);
	}

	simple->paint(f,skeleton,showedges);

}

void CGALRenderer::setColor(CGAL::IO::Color& t,const QColor& c)
{
	t.set_rgb(c.red(),c.green(),c.blue(),c.alpha());
}

CGAL::IO::Color CGALRenderer::getVertexColor(bool mark) const
{
	return mark ? markedVertexColor : vertexColor;
}

CGAL::IO::Color CGALRenderer::getEdgeColor(bool mark) const
{
	return mark ? markedEdgeColor : edgeColor;
}

CGAL::IO::Color CGALRenderer::getFacetColor(bool mark) const
{
	return mark ? markedFacetColor : facetColor;
}

float CGALRenderer::getVertexSize() const
{
	return vertexSize;
}

float CGALRenderer::getEdgeSize() const
{
	return edgeSize;
}
#endif
