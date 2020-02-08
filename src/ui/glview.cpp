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

static const GLfloat farfarAway=100000.0f;
static const int rulerLength=200;

GLView::GLView(QWidget* parent) :
#ifdef USE_QGLWIDGET
	QGLWidget(parent),
#else
	QOpenGLWidget(parent),
	projection(new QMatrix4x4()),
	modelview(new QMatrix4x4()),
#endif
	render(nullptr),
	distance(500.0f),
	showAxes(true),
	showCross(true),
	showBase(true),
	showPrintArea(true),
	showRulers(true),
	showEdges(false),
	skeleton(false),
	printX(0.0f),
	printY(0.0f),
	printWidth(0.0f),
	printLength(0.0f),
	printHeight(0.0f),
	appearance(Appearance_t::MK42),
	mouseDrag(false),
	rotateX(35.0f),
	rotateY(0.0f),
	rotateZ(35.0f),
	viewportX(0.0f),
	viewportZ(0.0f)
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

void GLView::setViewport(GLfloat rx,GLfloat ry,GLfloat rz,GLfloat x,GLfloat z,GLfloat d)
{
	rotateX=rx;
	rotateY=ry;
	rotateZ=rz;
	viewportX=x;
	viewportZ=z;
	distance=d;
	update();
}

void GLView::setPrintOrigin(GLfloat x,GLfloat y)
{
	printX=x;
	printY=y;
	update();
}

void GLView::setPrintVolume(GLfloat w,GLfloat l,GLfloat h)
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
			setViewport(90.0f,0.0f,0.0f,x,z,d);
			break;
		case Bottom:
			setViewport(-90.0f,0.0f,0.0f,x,z,d);
			break;
		case Left:
			setViewport(0.0f,0.0f,90.0f,x,z,d);
			break;
		case Right:
			setViewport(0.0f,0.0f,-90.0f,x,z,d);
			break;
		case Front:
			setViewport(0.0f,0.0f,0.0f,x,z,d);
			break;
		case Back:
			setViewport(0.0f,0.0f,-180.0f,x,z,d);
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
	GLfloat n=value?0.8f:1.0f;
	glClearColor(n,n,n,0.0f);
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
	glClearColor(1.0f,1.0f,1.0f,0.0f);
	GLfloat light_diffuse[]={1.0f,1.0f,1.0f,1.0f};
	GLfloat light_position0[]={-1.0f,-1.0f,+1.0f,0.0f};
	GLfloat light_position1[]={+1.0f,+1.0f,-1.0f,0.0f};

	glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
	glLightfv(GL_LIGHT0,GL_POSITION,light_position0);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,light_diffuse);
	glLightfv(GL_LIGHT1,GL_POSITION,light_position1);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}

void GLView::resizeGL(int w,int h)
{
	glViewport(0.0f,0.0f,static_cast<GLint>(w),static_cast<GLint>(h));

	glMatrixMode(GL_PROJECTION);

#ifdef USE_QGLWIDGET
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,+10.0f,+farfarAway);
#else
	projection->setToIdentity();
	projection->perspective(45.0f,static_cast<GLfloat>(w)/static_cast<GLfloat>(h),+10.0f,+farfarAway);
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
	glVertex2f(-1.0f,-1.0f);
	glVertex2f(1.0f,-1.0f);
	glColor3ub(186,186,186);
	glVertex2f(1.0f,1.0f);
	glVertex2f(-1.0f,1.0f);
	glEnd();
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void GLView::drawAxes()
{
	glLineWidth(1);
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_LINES);
	float c=fmax(distance/2,static_cast<GLfloat>(rulerLength));
	glVertex3f(-c,0.0f,0.0f);
	glVertex3f(+c,0.0f,0.0f);
	glVertex3f(0.0f,-c,0.0f);
	glVertex3f(0.0f,+c,0.0f);
	glVertex3f(0.0f,0.0f,-c);
	glVertex3f(0.0f,0.0f,+c);
	glEnd();
}

void GLView::drawBase()
{
	glEnable(GL_CULL_FACE);

	const GLfloat z=-0.01f;
	switch(appearance) {
		case Appearance_t::MK42: {
			const GLfloat baseX=-2.0f;
			const GLfloat baseY=-9.4f;
			const GLfloat baseWidth=254.0f;
			const GLfloat baseLength=235.0f;
			const GLfloat chamfer=4.0f;
			glLineWidth(2);
			glColor3f(0.2f,0.2f,0.2f);
			glBegin(GL_QUADS);
			glVertex3f(printX+baseX,printY+baseY+chamfer,z);
			glVertex3f(printX+baseX+chamfer,printY+baseY,z);
			glVertex3f(printX+baseX+baseWidth-chamfer,printY+baseY,z);
			glVertex3f(printX+baseX+baseWidth,printY+baseY+chamfer,z);

			glVertex3f(printX+baseX+baseWidth,printY+baseY+baseLength-chamfer,z);
			glVertex3f(printX+baseX+baseWidth-chamfer,printY+baseY+baseLength,z);
			glVertex3f(printX+baseX+chamfer,printY+baseY+baseLength,z);
			glVertex3f(printX+baseX,printY+baseY+baseLength-chamfer,z);

			glVertex3f(printX+baseX,printY+baseY+chamfer,z);
			glVertex3f(printX+baseX+baseWidth,printY+baseY+chamfer,z);
			glVertex3f(printX+baseX+baseWidth,printY+baseY+baseLength-chamfer,z);
			glVertex3f(printX+baseX,printY+baseY+baseLength-chamfer,z);
			glEnd();

			glLineWidth(2);
			glColor3f(0.6f,0.6f,0.6f);
			glBegin(GL_LINES);
			for(GLfloat o=0.0f; o<printWidth; o+=10.0f) {
				glVertex3f(printX+o,printY,z);
				glVertex3f(printX+o,printY+printLength,z);
			}
			for(GLfloat j=5.0f; j<printLength; j+=10.0f) {
				glVertex3f(printX,printY+j,z);
				glVertex3f(printX+printWidth,printY+j,z);
			}
			glEnd();

			glLineWidth(4);
			glColor3f(0.8f,0.8f,0.8f);
			glBegin(GL_LINES);
			for(GLfloat o=0.0f; o<printWidth; o+=50.0f) {
				glVertex3f(printX+o,printY,z);
				glVertex3f(printX+o,printY+printLength,z);
			}
			for(GLfloat j=5.0f; j<printLength; j+=50.0f) {
				glVertex3f(printX,printY+j,z);
				glVertex3f(printX+printWidth,printY+j,z);
			}
			glEnd();
		}
		break;
		case Appearance_t::MK2: {
			const GLfloat baseXY=-7.5;
			const GLfloat baseWL=215.0f;
			glLineWidth(2);
			glColor3f(0.6f,0.2f,0.2f);
			glBegin(GL_QUADS);
			glVertex3f(printX+baseXY,printY+baseXY,z);
			glVertex3f(printX+baseXY+baseWL,printY+baseXY,z);
			glVertex3f(printX+baseXY+baseWL,printY+baseXY+baseWL,z);
			glVertex3f(printX+baseXY,printY+baseXY+baseWL,z);
			glEnd();
		}
		break;
	}

	glLineWidth(4);
	glColor3f(0.8f,0.8f,0.8f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(printX,printY,z);
	glVertex3f(printX+printWidth,printY,z);
	glVertex3f(printX+printWidth,printY+printLength,z);
	glVertex3f(printX,printY+printLength,z);
	glEnd();

	glDisable(GL_CULL_FACE);
}

void GLView::drawPrintArea()
{
	glLineWidth(1);
	glColor3f(0.8f,0.8f,0.8f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(printX,printY,0.0f);
	glVertex3f(printX+printWidth,printY,0.0f);
	glVertex3f(printX+printWidth,printY+printLength,0.0f);
	glVertex3f(printX,printY+printLength,0.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(printX,printY,0.0f);
	glVertex3f(printX,printY,printHeight);

	glVertex3f(printX+printWidth,printY+printLength,0.0f);
	glVertex3f(printX+printWidth,printY+printLength,printHeight);

	glVertex3f(printX,printY+printLength,0.0f);
	glVertex3f(printX,printY+printLength,printHeight);

	glVertex3f(printX+printWidth,printY,0.0f);
	glVertex3f(printX+printWidth,printY,printHeight);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(printX,printY,printHeight);
	glVertex3f(printX+printWidth,printY,printHeight);
	glVertex3f(printX+printWidth,printY+printLength,printHeight);
	glVertex3f(printX,printY+printLength,printHeight);
	glEnd();
}

void GLView::drawRulers()
{
	glLineWidth(1);
	glColor3f(0.2f,0.2f,0.2f);
	glBegin(GL_LINES);
	int k=distance<200?1:10; //Only show milimeters when close up
	for(int i=-rulerLength; i<rulerLength; i+=k) {
		int j=i%10?2:5;
		glVertex3i(i,0,0);
		glVertex3i(i,j,0);
	}
	for(int i=-rulerLength; i<rulerLength; i+=k) {
		int j=i%10?2:5;
		glVertex3i(0,i,0);
		glVertex3i(j,i,0);
	}
	for(int i=-rulerLength; i<rulerLength; i+=k) {
		int j=i%10?2:5;
		glVertex3i(0,0,i);
		glVertex3i(j,0,i);
	}
	glEnd();
}

void GLView::drawCross()
{
	const GLfloat n=0.2f;
	glLineWidth(5.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f,0.0f,0.0f);
	glVertex3f(printX,printY,n);
	glVertex3f(printX+10.0f,printY,n);

	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(printX,printY,n);
	glVertex3f(printX,printY+10.0f,n);

	glColor3f(0.0f,0.0f,1.0f);
	glVertex3f(printX-n,printY-n,n);
	glVertex3f(printX-n,printY-n,10.0f);
	glEnd();

	glLineWidth(2.0f);
	renderX(printX+15.0f,printY,3.0f);
	renderY(printX,printY+15.0f,3.0f);
	renderZ(printX-n,printY-n,15.0f);
}

void GLView::paintGL()
{
	glDisable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawGradientBackground();

	glMatrixMode(GL_MODELVIEW);

#ifdef USE_QGLWIDGET
	glLoadIdentity();
	gluLookAt(-viewportX,-distance,-viewportZ,-viewportX,0.0f,-viewportZ,0.0f,0.0f,1.0f);

	glRotatef(rotateX,1.0f,0.0f,0.0f);
	glRotatef(rotateY,0.0f,1.0f,0.0f);
	glRotatef(rotateZ,0.0f,0.0f,1.0f);
#else
	modelview->setToIdentity();
	QVector3D eye(-viewportX,-distance,-viewportZ);
	QVector3D center(-viewportX,0.0f,-viewportZ);
	QVector3D up(0.0f,0.0f,1.0f);
	modelview->lookAt(eye,center,up);

	modelview->rotate(rotateX,1.0f,0.0f,0.0f);
	modelview->rotate(rotateY,0.0f,1.0f,0.0f);
	modelview->rotate(rotateZ,0.0f,0.0f,1.0f);

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

void GLView::renderX(GLfloat x,GLfloat y,GLfloat z)
{
	const float d=2.0f;
	glColor3f(1.0f,0.0f,0.0f);
	glBegin(GL_LINES);
	glVertex3f(x-d,y,z-d);
	glVertex3f(x+d,y,z+d);
	glVertex3f(x-d,y,z+d);
	glVertex3f(x+d,y,z-d);
	glEnd();
}

void GLView::renderY(GLfloat x,GLfloat y,GLfloat z)
{
	const float d=2.0f;
	glColor3f(0.0f,1.0f,0.0f);
	glBegin(GL_LINES);
	glVertex3f(x+d,y,z+d);
	glVertex3f(x,y,z);
	glVertex3f(x-d,y,z+d);
	glVertex3f(x,y,z);
	glVertex3f(x,y,z-d);
	glVertex3f(x,y,z);
	glEnd();
}

void GLView::renderZ(GLfloat x,GLfloat y,GLfloat z)
{
	const float d=2.0f;
	glColor3f(0.0f,0.0f,1.0f);
	glBegin(GL_LINES);
	glVertex3f(x-d,y,z-d);
	glVertex3f(x+d,y,z-d);
	glVertex3f(x-d,y,z+d);
	glVertex3f(x+d,y,z+d);
	glVertex3f(x-d,y,z-d);
	glVertex3f(x+d,y,z+d);
	glEnd();
}

void GLView::wheelEvent(QWheelEvent* event)
{
	zoomView(event->delta()/12);
	update();
}

void GLView::zoomView(float amt)
{
	distance*=static_cast<GLfloat>(pow(0.9,amt/10));
}

void GLView::mousePressEvent(QMouseEvent* event)
{
	mouseDrag=true;
	last=event->globalPos();
}

void GLView::normalizeAngle(GLfloat& angle)
{
	while(angle < 0.0f)
		angle+=360.0f;
	while(angle > 360.0f)
		angle-=360.0f;
}


void GLView::mouseMoveEvent(QMouseEvent* event)
{
	if(!mouseDrag)
		return;

	QPoint current=event->globalPos();
	int dx=current.x()-last.x();
	int dy=current.y()-last.y();
	bool shift=QApplication::keyboardModifiers() & Qt::ShiftModifier;
	if(event->buttons() & Qt::LeftButton) {
		rotateX+=static_cast<GLfloat>(dy);
		if(shift) {
			rotateY+=static_cast<GLfloat>(dx);
		} else {
			rotateZ+=static_cast<GLfloat>(dx);
		}
		normalizeAngle(rotateX);
		normalizeAngle(rotateY);
		normalizeAngle(rotateZ);
	} else {
		if(shift) {
			zoomView(-dy);
		} else {
			viewportX+=static_cast<GLfloat>(dx);
			viewportZ-=static_cast<GLfloat>(dy);
		}
	}
	update();

	last=current;
}

void GLView::mouseReleaseEvent(QMouseEvent*)
{
	mouseDrag=false;
}
