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

#include <QApplication>
#include <math.h>
#include "GLView.h"

static const double farfarAway=100000.0;
static const int baseWidth=232;
static const int baseX=baseWidth/2;
static const int baseLength=230;
static const int baseY=baseLength/2;

GLView::GLView(QWidget* parent) : QGLWidget(parent)
{
	render=NULL;
	distance=500.0;
	showAxes=true;
	showBase=true;
	rotateX=35.0;
	rotateY=0.0;
	rotateZ=35.0;
	viewportX=0;
	viewportZ=0;
}

void GLView::setRenderer(Renderer* r)
{
	render=r;
}

void GLView::initializeGL()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_position0[] = {-1.0, -1.0, +1.0, 0.0};
	GLfloat light_position1[] = {+1.0, +1.0, -1.0, 0.0};

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}

void GLView::resizeGL(int w, int h)
{
	glViewport(0,0,(GLint)w,(GLint)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, (GLdouble)w/(GLdouble)h, +10.0, +farfarAway);
}

void GLView::paintGL()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	gluLookAt(-viewportX, -distance, -viewportZ, -viewportX, 0.0, -viewportZ, 0.0, 0.0, 1.0);

	glRotated(rotateX, 1.0, 0.0, 0.0);
	glRotated(rotateY, 0.0, 1.0, 0.0);
	glRotated(rotateZ, 0.0, 0.0, 1.0);

	if(showAxes) {
		glLineWidth(1);
		glColor3d(0.5, 0.5, 0.5);
		glBegin(GL_LINES);
		glVertex3d(-distance/2, 0, 0);
		glVertex3d(+distance/2, 0, 0);
		glVertex3d(0, -distance/2, 0);
		glVertex3d(0, +distance/2, 0);
		glVertex3d(0, 0, -distance/2);
		glVertex3d(0, 0, +distance/2);
		glEnd();
	}
	if(showBase) {
		glLineWidth(1);
		glColor3d(0.0, 0.0, 1.0);
		glBegin(GL_LINE_LOOP);
		glVertex3d(-baseX, -baseY, 0);
		glVertex3d(+baseX, -baseY, 0);
		glVertex3d(+baseX, +baseY, 0);
		glVertex3d(-baseX, +baseY, 0);
		glEnd();
	}

	if(render)
		render->draw(false,false);
}

void GLView::wheelEvent(QWheelEvent* event)
{
	//Increment distance 10 units per wheel step
	distance += (GLdouble)(event->delta() / 12.0);
	updateGL();
}

void GLView::mousePressEvent(QMouseEvent* event)
{
	mouseDrag = true;
	last = event->globalPos();
	grabMouse();
	setFocus();
}

void GLView::normalizeAngle(GLdouble& angle)
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
	int dx = current.x()-last.x();
	int dy = current.y()-last.y();
	if(event->buttons() & Qt::LeftButton) {
		if(QApplication::keyboardModifiers() & Qt::ShiftModifier) {
			distance += (GLdouble)dy;
		} else {
			rotateX += (GLdouble)dy;
			rotateZ += (GLdouble)dx;
			normalizeAngle(rotateX);
			normalizeAngle(rotateZ);
		}
	} else {
		viewportX += (GLint)dx;
		viewportZ -= (GLint)dy;
	}
	updateGL();

	last = current;
}

void GLView::mouseReleaseEvent(QMouseEvent*)
{
	mouseDrag = false;
	releaseMouse();
}
