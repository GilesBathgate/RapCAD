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

#include <QApplication>
#include <math.h>
#include "glview.h"
#if USE_QGLWIDGET
#include <CGAL/glu.h>
#endif

static const float farfarAway=100000.0;
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

GLView::GLView(QWidget* parent) :
#if USE_QGLWIDGET
	QGLWidget(parent)
#else
	QOpenGLWidget(parent)
#endif
{
	render=NULL;
	distance=500.0;
	showAxes=true;
	showBase=true;
	showPrintArea=true;
	showRulers=true;
	showEdges=false;
	skeleton=false;
	rotateX=35.0;
	rotateY=0.0;
	rotateZ=35.0;
	viewportX=0;
	viewportZ=0;
#if !USE_QGLWIDGET
	projection=new QMatrix4x4();
	modelview=new QMatrix4x4();
#endif
}

GLView::~GLView()
{
#if !USE_QGLWIDGET
	delete projection;
	delete modelview;
#endif
	delete render;
}

void GLView::getViewport(float& rx,float& ry,float& rz,float& x, float& z,float& d)
{
	rx=rotateX;
	ry=rotateY;
	rz=rotateZ;
	x=viewportX;
	z=viewportZ;
	d=distance;
}

void GLView::setViewport(float rx,float ry,float rz,float x, float z,float d)
{
	rotateX=rx;
	rotateY=ry;
	rotateZ=rz;
	viewportX=x;
	viewportZ=z;
	distance=d;
	update();
}

void GLView::setSkeleton(bool skel)
{
	skeleton=skel;
	update();
}

void GLView::setShowEdges(bool edges)
{
	showEdges=edges;
	update();
}

void GLView::changeViewport(int t)
{
	float rx,ry,rz,x,z,d;
	getViewport(rx,ry,rz,x,z,d);

	switch(t) {
	case Top:
		setViewport(90,0,0,x,z,d);
		break;
	case Bottom:
		setViewport(-90,0,0,x,z,d);
		break;
	case Left:
		setViewport(0,0,90,x,z,d);
		break;
	case Right:
		setViewport(0,0,-90,x,z,d);
		break;
	case Front:
		setViewport(0,0,0,x,z,d);
		break;
	case Back:
		setViewport(0,0,-180,x,z,d);
		break;
	}
}

void GLView::setShowAxes(bool axes)
{
	showAxes=axes;
	update();
}

void GLView::setShowRulers(bool rulers)
{
	showRulers=rulers;
	update();
}

void GLView::setShowBase(bool base)
{
	showBase=base;
	update();
}

void GLView::setShowPrintArea(bool print)
{
	showPrintArea=print;
	update();
}

void GLView::setRenderer(Renderer* r)
{
	delete render;
	render=r;
	update();
}

void GLView::preferencesUpdated()
{
	if(!render) return;

	render->preferencesUpdated();
	update();
}

void GLView::setCompiling(bool value)
{
	GLfloat n=value?0.8:1.0;
	glClearColor(n, n, n, 0.0);
	if(render)
		render->setCompiling(value);
	update();
}

void GLView::initializeGL()
{
#if !USE_QGLWIDGET
	initializeOpenGLFunctions();
#endif

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

#if USE_QGLWIDGET
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w/(GLfloat)h, +10.0, +farfarAway);
#else
	projection->setToIdentity();
	projection->perspective(45.0, (GLfloat)w/(GLfloat)h, +10.0, +farfarAway);
	glLoadMatrixf(projection->data());
#endif
}

void GLView::paintGL()
{
	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if USE_QGLWIDGET
	glLoadIdentity();
	gluLookAt(-viewportX, -distance, -viewportZ, -viewportX, 0.0, -viewportZ, 0.0, 0.0, 1.0);

	glRotatef(rotateX, 1.0, 0.0, 0.0);
	glRotatef(rotateY, 0.0, 1.0, 0.0);
	glRotatef(rotateZ, 0.0, 0.0, 1.0);
#else
	modelview->setToIdentity();
	QVector3D eye(-viewportX,-distance,-viewportZ);
	QVector3D center(-viewportX,0.0,-viewportZ);
	QVector3D up(0.0,0.0,1.0);
	modelview->lookAt(eye,center,up);

	modelview->rotate(rotateX, 1.0, 0.0, 0.0);
	modelview->rotate(rotateY, 0.0, 1.0, 0.0);
	modelview->rotate(rotateZ, 0.0, 0.0, 1.0);

	glLoadMatrixf(modelview->data());
#endif

	if(showAxes) {
		glLineWidth(1);
		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_LINES);
		float c=fmax(distance/2,rulerLength);
		glVertex3f(-c, 0, 0);
		glVertex3f(+c, 0, 0);
		glVertex3f(0, -c, 0);
		glVertex3f(0, +c, 0);
		glVertex3f(0, 0, -c);
		glVertex3f(0, 0, +c);
		glEnd();
	}
	if(showBase) {
		glLineWidth(1);
		glColor3f(0.0, 0.0, 1.0);
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
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_LINE_LOOP);
		glVertex3i(printX, printY, 0);
		glVertex3i(printWidth-printX,printY, 0);
		glVertex3i(printWidth-printX,printLength-printY, 0);
		glVertex3i(printX, printLength-printY, 0);
		glEnd();
	}
	if(showRulers) {
		glLineWidth(1);
		glColor3f(0.2, 0.2, 0.2);
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
	zoomView(event->delta()/12);
	update();
}

void GLView::zoomView(float amt)
{
	distance*=(GLfloat)pow(0.9,amt/10);
}

void GLView::mousePressEvent(QMouseEvent* event)
{
	mouseDrag = true;
	last = event->globalPos();
}

void GLView::normalizeAngle(GLfloat& angle)
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
		rotateX += (GLfloat)dy;
		if(QApplication::keyboardModifiers() & Qt::ShiftModifier) {
			rotateY += (GLfloat)dx;
		} else {
			rotateZ += (GLfloat)dx;
		}
		normalizeAngle(rotateX);
		normalizeAngle(rotateY);
		normalizeAngle(rotateZ);
	} else {
		if(QApplication::keyboardModifiers() & Qt::ShiftModifier) {
			zoomView(-dy);
		} else {
			viewportX += (GLint)dx;
			viewportZ -= (GLint)dy;
		}
	}
	update();

	last = current;
}

void GLView::mouseReleaseEvent(QMouseEvent*)
{
	mouseDrag = false;
}
