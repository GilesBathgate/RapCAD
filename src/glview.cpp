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
#include "math.h"

static const double farfarAway=100000.0;
static const double rotMulti=0.7;

GLView::GLView(QWidget* parent) : QGLWidget(parent)
{
	render=NULL;
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
	viewportW=(GLint)w;
	viewportH=(GLint)h;
	glViewport(viewportX,viewportY,viewportW,viewportH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, (GLdouble)w/(GLdouble)h, +10.0, +farfarAway);
}

void GLView::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	gluLookAt(0.0, -distance, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glViewport(viewportX,viewportY,viewportW,viewportH);

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
		render->draw(false,false);
}

void GLView::wheelEvent(QWheelEvent* event)
{
	distance *= pow(0.9, event->delta() / 120.0);
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
		rotateX += (GLdouble)(dy * rotMulti);
		rotateZ += (GLdouble)(dx * rotMulti);
		normalizeAngle(rotateX);
		normalizeAngle(rotateZ);
	} else {
		viewportX += (GLint)dx;
		viewportY -= (GLint)dy;
	}
	updateGL();

	last = current;
}

void GLView::mouseReleaseEvent(QMouseEvent*)
{
	mouseDrag = false;
	releaseMouse();
}
