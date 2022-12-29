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

#include "simplerenderer.h"
#include "qopengl.h"

SimpleRenderer::SimpleRenderer(Primitive* pr) :
	primitive(pr)
{
}

void SimpleRenderer::paint(QOpenGLFunctions_1_0& f,bool, bool)
{
	f.glLineWidth(1);
	f.glColor4f(0.0,0.0,1.0,0.5);
	f.glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
	descendChildren(f,primitive);
}

void SimpleRenderer::preferencesUpdated()
{

}

void SimpleRenderer::setCompiling(bool)
{

}

void SimpleRenderer::descendChildren(QOpenGLFunctions_1_0& f, Primitive* p)
{
	for(Primitive* c: p->getChildren()) {
		descendChildren(f,c);

		if(c->getType()!=PrimitiveTypes::Lines) {
			f.glEnable(GL_BLEND);
			for(Polygon* pg: c->getPolygons()) {
				f.glBegin(GL_QUADS);
				for(const auto& pt: pg->getPoints()) {
					drawPoint(f,pt);
				}
				f.glEnd();
			}
			f.glDisable(GL_BLEND);
		}

		for(Polygon* pg: c->getPolygons()) {
			f.glBegin(GL_LINE_STRIP);
			for(const auto& pt: pg->getPoints()) {
				drawPoint(f,pt);
			}
			f.glEnd();
		}
	}
}

void SimpleRenderer::drawPoint(QOpenGLFunctions_1_0& f,const Point& pt)
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	to_glcoord(pt,x,y,z);
	f.glVertex3f(x,y,z);
}
