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

static const double farfarAway=100000.0;
static const double rotMulti=0.7;

GLView::GLView(QWidget* parent) : QGLWidget(parent)
{
	distance=500;
	showaxes=true;
	rotateX=35.0;
	rotateY=0.0;
	rotateZ=35.0;
}

void GLView::setRenderer(Renderer* r)
{
	render=r;
}

void GLView::initializeGL()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

void GLView::resizeGL(int w, int h)
{
	glViewport(0, 0, (GLint)w, (GLint)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLdouble)w/(GLdouble)h, +10.0, +farfarAway);
}

void GLView::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	gluLookAt(0.0, -distance, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(rotateX, 1.0, 0.0, 0.0);
	glRotated(rotateY, 0.0, 1.0, 0.0);
	glRotated(rotateZ, 0.0, 0.0, 1.0);

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

	if(render)
	    render->draw(false,true);
}

void GLView::mousePressEvent(QMouseEvent* event)
{
	mouseDrag = true;
	last = event->globalPos();
	grabMouse();
	setFocus();
}

void GLView::NormalizeAngle(double& angle)
{
	while(angle < 0)
		angle += 360;
	while(angle > 360)
		angle -= 360;
}


void GLView::mouseMoveEvent(QMouseEvent* event)
{
	if(!mouseDrag)
		return;

	QPoint current = event->globalPos();

	rotateX += (current.y()-last.y()) * rotMulti;
	rotateZ += (current.x()-last.x()) * rotMulti;
	NormalizeAngle(rotateX);
	NormalizeAngle(rotateZ);
	updateGL();

	last = current;
}

void GLView::mouseReleaseEvent(QMouseEvent*)
{
	mouseDrag = false;
	releaseMouse();
}
