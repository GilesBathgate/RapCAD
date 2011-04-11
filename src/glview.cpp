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

#include "GLView.h"

GLView::GLView(QWidget* parent) : QGLWidget(parent)
{
	distance=500;
	showaxes=true;
}

void GLView::initializeGL()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

void GLView::resizeGL(int w, int h)
{
	glViewport(0, 0, (GLint)w, (GLint)h);
}

void GLView::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if(showaxes) {
		glLineWidth(1);
		glColor3d(0.5, 0.5, 0.5);
		glBegin(GL_LINES);
		glVertex3d(-distance/10, 0, 0);
		glVertex3d(+distance/10, 0, 0);
		glVertex3d(0, -distance/10, 0);
		glVertex3d(0, +distance/10, 0);
		glVertex3d(0, 0, -distance/10);
		glVertex3d(0, 0, +distance/10);
		glEnd();
	}
}
