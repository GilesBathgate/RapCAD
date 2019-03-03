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

#include <QApplication>
#include <math.h>
#include "glview.h"
#ifdef USE_QGLWIDGET
#include <CGAL/glu.h>
#endif

static const GLfloat farfarAway=100000.0;
static const GLfloat rulerLength=200.0;

GLView::GLView(QWidget* parent) :
#ifdef USE_QGLWIDGET
	QGLWidget(parent),
#else
	QOpenGLWidget(parent),
	projection(new QMatrix4x4()),
	modelview(new QMatrix4x4()),
#endif
	render(nullptr),
	distance(500.0),
	showAxes(true),
	showCross(true),
	showBase(true),
	showPrintArea(true),
	showRulers(true),
	showEdges(false),
	skeleton(false),
	printX(0.0),
	printY(0.0),
	printWidth(0.0),
	printLength(0.0),
	printHeight(0.0),
	appearance(Appearance_t::MK42),
	mouseDrag(false),
	rotateX(35.0),
	rotateY(0.0),
	rotateZ(35.0),
	viewportX(0.0),
	viewportZ(0.0)
{
}

GLView::~GLView()
{
#ifndef USE_QGLWIDGET
	delete projection;
	delete modelview;
#endif
	delete render;
}

void GLView::getViewport(GLfloat& rx,GLfloat& ry,GLfloat& rz,GLfloat& x,GLfloat& z,GLfloat& d)
{
	rx=rotateX;
	ry=rotateY;
	rz=rotateZ;
	x=viewportX;
	z=viewportZ;
	d=distance;
}

void GLView::setViewport(GLfloat rx, GLfloat ry, GLfloat rz, GLfloat x, GLfloat z, GLfloat d)
{
	rotateX=rx;
	rotateY=ry;
	rotateZ=rz;
	viewportX=x;
	viewportZ=z;
	distance=d;
	update();
}

void GLView::setPrintOrigin(GLfloat x, GLfloat y)
{
	printX = x;
	printY = y;
	update();
}

void GLView::setPrintVolume(GLfloat w, GLfloat l, GLfloat h)
{
	printWidth=w;
	printLength=l;
	printHeight=h;
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
	if(render)
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

void GLView::setBedAppearance(Appearance_t v)
{
	appearance=v;
	update();
}

void GLView::initializeGL()
{
#ifndef USE_QGLWIDGET
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

#ifdef USE_QGLWIDGET
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w/(GLfloat)h, +10.0, +farfarAway);
#else
	projection->setToIdentity();
	projection->perspective(45.0, (GLfloat)w/(GLfloat)h, +10.0, +farfarAway);
	glLoadMatrixf(projection->data());
#endif

}

void GLView::drawGradientBackground()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glBegin(GL_QUADS);
	glColor3ub(119,119,119);
	glVertex2f(-1.0,-1.0);
	glVertex2f(1.0,-1.0);
	glColor3ub(186,186,186);
	glVertex2f(1.0, 1.0);
	glVertex2f(-1.0, 1.0);
	glEnd();
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void GLView::drawAxes()
{
	glLineWidth(1);
	glColor3f(0.0, 0.0, 0.0);
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

void GLView::drawBase()
{
	glEnable(GL_CULL_FACE);

	const GLfloat z=-0.01;
	switch(appearance) {
		case Appearance_t::MK42: {
			const GLfloat baseX=-2.0;
			const GLfloat baseY=-9.4;
			const GLfloat baseWidth=254.0;
			const GLfloat baseLength=235.0;
			const GLfloat chamfer=4.0;
			glLineWidth(2);
			glColor3f(0.2, 0.2, 0.2);
			glBegin(GL_QUADS);
			glVertex3f(printX+baseX, printY+baseY+chamfer, z);
			glVertex3f(printX+baseX+chamfer, printY+baseY, z);
			glVertex3f(printX+baseX+baseWidth-chamfer, printY+baseY, z);
			glVertex3f(printX+baseX+baseWidth, printY+baseY+chamfer, z);

			glVertex3f(printX+baseX+baseWidth, printY+baseY+baseLength-chamfer, z);
			glVertex3f(printX+baseX+baseWidth-chamfer, printY+baseY+baseLength, z);
			glVertex3f(printX+baseX+chamfer, printY+baseY+baseLength, z);
			glVertex3f(printX+baseX, printY+baseY+baseLength-chamfer, z);

			glVertex3f(printX+baseX, printY+baseY+chamfer, z);
			glVertex3f(printX+baseX+baseWidth, printY+baseY+chamfer, z);
			glVertex3f(printX+baseX+baseWidth, printY+baseY+baseLength-chamfer, z);
			glVertex3f(printX+baseX, printY+baseY+baseLength-chamfer, z);
			glEnd();

			glLineWidth(2);
			glColor3f(0.6, 0.6, 0.6);
			glBegin(GL_LINES);
			for(GLfloat o=0.0; o<printWidth; o+=10.0) {
				glVertex3f(printX+o, printY, z);
				glVertex3f(printX+o, printY+printLength, z);
			}
			for(GLfloat j=5.0; j<printLength; j+=10.0) {
				glVertex3f(printX, printY+j, z);
				glVertex3f(printX+printWidth, printY+j, z);
			}
			glEnd();

			glLineWidth(4);
			glColor3f(0.8, 0.8, 0.8);
			glBegin(GL_LINES);
			for(GLfloat o=0.0; o<printWidth; o+=50.0) {
				glVertex3f(printX+o, printY, z);
				glVertex3f(printX+o, printY+printLength, z);
			}
			for(GLfloat j=5.0; j<printLength; j+=50.0) {
				glVertex3f(printX, printY+j, z);
				glVertex3f(printX+printWidth, printY+j, z);
			}
			glEnd();
		}
		break;
		case Appearance_t::MK2: {
			const GLfloat baseXY=-7.5;
			const GLfloat baseWL=215.0;
			glLineWidth(2);
			glColor3f(0.6, 0.2, 0.2);
			glBegin(GL_QUADS);
			glVertex3f(printX+baseXY, printY+baseXY, z);
			glVertex3f(printX+baseXY+baseWL, printY+baseXY, z);
			glVertex3f(printX+baseXY+baseWL, printY+baseXY+baseWL, z);
			glVertex3f(printX+baseXY, printY+baseXY+baseWL, z);
			glEnd();
		}
		break;
	}

	glLineWidth(4);
	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_LINE_LOOP);
	glVertex3f(printX, printY, z);
	glVertex3f(printX+printWidth,printY, z);
	glVertex3f(printX+printWidth,printY+printLength, z);
	glVertex3f(printX, printY+printLength, z);
	glEnd();

	glDisable(GL_CULL_FACE);
}

void GLView::drawPrintArea()
{
	glLineWidth(1);
	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_LINE_LOOP);
	glVertex3f(printX, printY, 0);
	glVertex3f(printX+printWidth,printY, 0);
	glVertex3f(printX+printWidth,printY+printLength, 0);
	glVertex3f(printX, printY+printLength, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(printX,printY, 0);
	glVertex3f(printX,printY, printHeight);

	glVertex3f(printX+printWidth,printY+printLength, 0);
	glVertex3f(printX+printWidth,printY+printLength, printHeight);

	glVertex3f(printX, printY+printLength, 0);
	glVertex3f(printX, printY+printLength, printHeight);

	glVertex3f(printX+printWidth, printY, 0);
	glVertex3f(printX+printWidth, printY, printHeight);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(printX, printY, printHeight);
	glVertex3f(printX+printWidth,printY, printHeight);
	glVertex3f(printX+printWidth,printY+printLength, printHeight);
	glVertex3f(printX, printY+printLength, printHeight);
	glEnd();
}

void GLView::drawRulers()
{
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

void GLView::drawCross()
{
	const GLfloat n = 0.2;
	glLineWidth(5.0);
	glBegin(GL_LINES);
	glColor3f(1.0,0.0,0.0);
	glVertex3f(printX,printY,n);
	glVertex3f(printX+10.0,printY,n);

	glColor3f(0.0,1.0,0.0);
	glVertex3f(printX,printY,n);
	glVertex3f(printX,printY+10.0,n);

	glColor3f(0.0,0.0,1.0);
	glVertex3f(printX-n,printY-n,n);
	glVertex3f(printX-n,printY-n,10.0);
	glEnd();

	glLineWidth(2.0);
	renderX(printX+15.0,printY,3.0);
	renderY(printX,printY+15.0,3.0);
	renderZ(printX-n,printY-n,15.0);
}

void GLView::paintGL()
{
	glDisable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawGradientBackground();

	glMatrixMode(GL_MODELVIEW);

#ifdef USE_QGLWIDGET
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

	if(showAxes) drawAxes();
	if(showBase) drawBase();
	if(showPrintArea) drawPrintArea();
	if(showRulers) drawRulers();
	if(showCross) drawCross();

	glEnable(GL_LIGHTING);

	if(render)
		render->paint(skeleton,showEdges);
}

void GLView::renderX(double x,double y,double z)
{
	const float d=2.0;
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_LINES);
	glVertex3f(x-d,y,z-d);
	glVertex3d(x+d,y,z+d);
	glVertex3f(x-d,y,z+d);
	glVertex3d(x+d,y,z-d);
	glEnd();
}

void GLView::renderY(double x,double y,double z)
{
	const float d=2.0;
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
	glVertex3d(x+d,y,z+d);
	glVertex3f(x,y,z);
	glVertex3f(x-d,y,z+d);
	glVertex3d(x,y,z);
	glVertex3d(x,y,z-d);
	glVertex3d(x,y,z);
	glEnd();
}

void GLView::renderZ(double x,double y,double z)
{
	const float d=2.0;
	glColor3f(0.0,0.0,1.0);
	glBegin(GL_LINES);
	glVertex3f(x-d,y,z-d);
	glVertex3d(x+d,y,z-d);
	glVertex3f(x-d,y,z+d);
	glVertex3d(x+d,y,z+d);
	glVertex3f(x-d,y,z-d);
	glVertex3d(x+d,y,z+d);
	glEnd();
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
	bool shift = QApplication::keyboardModifiers() & Qt::ShiftModifier;
	if(event->buttons() & Qt::LeftButton) {
		rotateX += (GLfloat)dy;
		if(shift) {
			rotateY += (GLfloat)dx;
		} else {
			rotateZ += (GLfloat)dx;
		}
		normalizeAngle(rotateX);
		normalizeAngle(rotateY);
		normalizeAngle(rotateZ);
	} else {
		if(shift) {
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
