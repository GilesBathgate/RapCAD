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
#include "cgalrenderer.h"
#include "preferences.h"
#include "primitive.h"

CGALRenderer::CGALRenderer(CGALPrimitive* pr)
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
	primitive=pr;
	CGAL::OGL::Nef3_Converter<CGAL::NefPolyhedron3>::convert_to_OGLPolyhedron(pr->getNefPolyhedron(),this);
}

void CGALRenderer::draw(bool skeleton, bool showedges)
{
	init();
	if(!skeleton) {
		glCallList(this->object_list_+2);
	}
	if(skeleton||showedges) {
		glDisable(GL_LIGHTING);
		glCallList(this->object_list_+1);
		glCallList(this->object_list_);
		glEnable(GL_LIGHTING);
	}

	glLineWidth(1);
	glColor3d(0.0, 0.0, 1.0);
	QList<Primitive*> children=primitive->getChildren();
	foreach(Primitive* c, children) {
		foreach(Polygon* p,c->getPolygons()) {
			glBegin(GL_LINE_STRIP);
			foreach(Point pt,p->getPoints()) {
				decimal x,y,z;
				pt.getXYZ(x,y,z);
				glVertex3d(x, y, z);
			}
			glEnd();
		}
	}
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

double CGALRenderer::getVertexSize() const
{
	return vertexSize;
}

double CGALRenderer::getEdgeSize() const
{
	return edgeSize;
}
#endif
