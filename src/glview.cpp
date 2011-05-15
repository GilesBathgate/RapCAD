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
static const int baseX=-16;
static const int baseY=-30;
static const int baseWidth=232;
static const int baseLength=230;
static const int notchX=130;
static const int notchWidth=40;
static const int notchLength=30;

static const int printX=0;
static const int printY=0;
static const int printWidth=200;
static const int printLength=200;

static const int rulerLength=200;

GLView::GLView(QWidget* parent) : QGLWidget(parent)
{
	render=NULL;
	distance=500.0;
	showAxes=true;
	showBase=true;
	showPrintArea=true;
	showTicks=true;
	rotateX=35.0;
	rotateY=0.0;
	rotateZ=35.0;
	viewportX=0;
	viewportZ=0;
}

void GLView::makeImage(const QString& filename)
{
	QGLPixelBuffer pbuffer(1000,1000);
	pbuffer.makeCurrent();
	initializeGL();
	resizeGL(1000,1000);
	paintGL();

	QImage i=pbuffer.toImage();
	i.save(filename);
	pbuffer.doneCurrent();
}

void GLView::setSkeleton(bool skel)
{
	skeleton=skel;
	updateGL();
}

void GLView::setShowEdges(bool edges)
{
	showEdges=edges;
	updateGL();
}

void GLView::setRenderer(Renderer* r)
{
	render=r;
	updateGL();
}

void GLView::initializeGL()
{
	glEnable(GL_DEPTH_TEST);
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
		double c=fmax(distance/2,rulerLength);
		glVertex3d(-c, 0, 0);
		glVertex3d(+c, 0, 0);
		glVertex3d(0, -c, 0);
		glVertex3d(0, +c, 0);
		glVertex3d(0, 0, -c);
		glVertex3d(0, 0, +c);
		glEnd();
	}
	if(showBase) {
		glLineWidth(1);
		glColor3d(0.0, 0.0, 1.0);
		glBegin(GL_LINE_LOOP);
		glVertex3i(baseX, baseY, 0);
		glVertex3i(baseX+notchX, baseY, 0);
		glVertex3i(baseX+notchX, baseY+notchLength, 0);
		glVertex3i(baseX+notchX+notchWidth, baseY+notchLength, 0);
		glVertex3i(baseX+notchX+notchWidth, baseY, 0);
		glVertex3i(baseX+baseWidth, baseY, 0);
		glVertex3i(baseX+baseWidth, baseY+baseLength, 0);
		glVertex3i(baseX, baseY+baseLength, 0);
		glEnd();
	}
	if(showPrintArea) {
		glLineWidth(1);
		glColor3d(0.0, 1.0, 0.0);
		glBegin(GL_LINE_LOOP);
		glVertex3i(printX, printY, 0);
		glVertex3i(printWidth-printX,printY, 0);
		glVertex3i(printWidth-printX,printLength-printY, 0);
		glVertex3i(printX, printLength-printY, 0);
		glEnd();
	}
	if(showTicks) {
		glLineWidth(1);
		glColor3d(0.2, 0.2, 0.2);
		glBegin(GL_LINES);
		int k=distance<200?1:10; //Only show milimeters when close up
		for(int i=-rulerLength; i<rulerLength; i+=k) {
			int j=i%10?2:5;
			glVertex3i(i, 0, 0);
			glVertex3i(i, j, 0);
		}
		for(int i=-rulerLength; i<rulerLength; i+=k) {
			int j=i%10?2:5;
			glVertex3i(0, i, 0);
			glVertex3i(j, i, 0);
		}
		for(int i=-rulerLength; i<rulerLength; i+=k) {
			int j=i%10?2:5;
			glVertex3i(0, 0, i);
			glVertex3i(j, 0, i);
		}
		glEnd();
	}

	if(render)
		render->draw(skeleton,showEdges);
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
