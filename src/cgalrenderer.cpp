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
#include "cgalrenderer.h"
#include "preferences.h"
#include "primitive.h"

CGALRenderer::CGALRenderer(Primitive* primitive) :
	simple(new SimpleRenderer(primitive))
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
	typedef CGAL::OGL::Nef3_Converter<CGAL::NefPolyhedron3> converter;
	auto* pr=dynamic_cast<CGALPrimitive*>(p);
	if(pr) {
		converter::convert_to_OGLPolyhedron(pr->getNefPolyhedron(),this);
	} else {
		for(Primitive* c: p->getChildren())
			descendChildren(c);
	}
}

void CGALRenderer::loadPreferences()
{
	Preferences* p = Preferences::getInstance();
	setColor(markedVertexColor,p->getMarkedVertexColor());
	setColor(vertexColor,p->getVertexColor());
	setColor(markedEdgeColor,p->getMarkedEdgeColor());
	setColor(edgeColor,p->getEdgeColor());
	setColor(markedFacetColor,p->getMarkedFacetColor());
	setColor(facetColor,p->getFacetColor());
	vertexSize=p->getVertexSize();
	edgeSize=p->getEdgeSize();
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

void CGALRenderer::desaturate(CGAL::Color& c)
{
	QColor rgb(c.red(),c.green(),c.blue());
	setColor(c,QColor::fromHsv(rgb.hue(),0,rgb.value()));
}

void CGALRenderer::paint(bool skeleton, bool showedges)
{
	init(); //init returns instantly if its already been called.
	if(!skeleton) {
		glCallList(object_list_+2);
	}
	if(skeleton||showedges) {
		glDisable(GL_LIGHTING);
		glCallList(object_list_+1);
		glCallList(object_list_);
		glEnable(GL_LIGHTING);
	}

	simple->paint(skeleton,showedges);

}

void CGALRenderer::setColor(CGAL::Color& t,QColor c)
{
	CGAL::Color cc(c.red(),c.green(),c.blue(),c.alpha());
	t=cc;
}

CGAL::Color CGALRenderer::getVertexColor(bool mark) const
{
	return mark ? markedVertexColor : vertexColor;
}

CGAL::Color CGALRenderer::getEdgeColor(bool mark) const
{
	return mark ? markedEdgeColor : edgeColor;
}

CGAL::Color CGALRenderer::getFacetColor(bool mark) const
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
