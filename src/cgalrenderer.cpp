/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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

#include "cgalrenderer.h"

using CGAL::OGL::Nef3_Converter;

CGALRenderer::CGALRenderer(const CGALPrimitive& p)
{
	//TODO read in colors from preferences instead of
	//hard coded constants.
	setColor(VertexColor,true,CGAL::Color(0xb7,0xe8,0x5c));
	setColor(VertexColor,false,CGAL::Color(0xff,0xf6,0x7c));
	setColor(EdgeColor,true,CGAL::Color(0xab,0xd8,0x56));
	setColor(EdgeColor,false,CGAL::Color(0xff,0xec,0x5e));
	setColor(FacetColor,true,CGAL::Color(0x9d,0xcb,0x51));
	setColor(FacetColor,false,CGAL::Color(0xf9,0xd7,0x2c));
	Nef3_Converter<CGAL::NefPolyhedron3>::convert_to_OGLPolyhedron(p.getPoly3(),this);
}

void CGALRenderer::draw(bool skeleton, bool showedges)
{
	init();
	if(!skeleton) {
		glCallList(this->object_list_+2);
		if(showedges) {
			glDisable(GL_LIGHTING);
			glCallList(this->object_list_+1);
			glCallList(this->object_list_);
		}
	} else {
		glDisable(GL_LIGHTING);
		glCallList(this->object_list_+1);
		glCallList(this->object_list_);
	}
}

void CGALRenderer::setColor(Color_e t,bool marked,CGAL::Color c)
{
	switch(t) {
	case VertexColor:
		marked ? markedVertexColor=c : vertexColor=c;
		return;
	case EdgeColor:
		marked ? markedEdgeColor=c : edgeColor=c;
		return;
	case FacetColor:
		marked ? markedFacetColor=c : facetColor=c;
		return;
	}
}

CGAL::Color CGALRenderer::getVertexColor(Vertex_iterator v) const
{
	return v->mark() ? markedVertexColor : vertexColor;
}

CGAL::Color CGALRenderer::getEdgeColor(Edge_iterator e) const
{
	return e->mark() ? markedEdgeColor : edgeColor;
}

CGAL::Color CGALRenderer::getFacetColor(Halffacet_iterator f) const
{
	return f->mark() ? markedFacetColor : facetColor;
}
