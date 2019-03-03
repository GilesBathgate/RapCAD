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

#include "simplerenderer.h"
#include "qgl.h"

SimpleRenderer::SimpleRenderer(Primitive* pr) :
	primitive(pr)
{
}

void SimpleRenderer::paint(bool, bool)
{
	glLineWidth(1);
	glColor3f(0.0, 0.0, 1.0);
	descendChildren(primitive);
}

void SimpleRenderer::preferencesUpdated()
{

}

void SimpleRenderer::setCompiling(bool)
{

}

void SimpleRenderer::descendChildren(Primitive* p)
{
	for(Primitive* c: p->getChildren()) {
		descendChildren(c);

		for(Polygon* p: c->getPolygons()) {
			glBegin(GL_LINE_STRIP);
			for(const auto& pt: p->getPoints()) {
#ifdef USE_CGAL
				GLfloat x,y,z;
				to_glcoord(pt,x,y,z);
#else
				decimal x=pt.x(),y=pt.y(),z=pt.z();
#endif
				glVertex3f(x,y,z);
			}
			glEnd();
		}
	}
}

