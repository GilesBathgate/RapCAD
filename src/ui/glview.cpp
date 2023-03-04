/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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

#include "glview.h"
#include <QApplication>
#include <cmath>

static const GLfloat farfarAway=100000.0F;
static const int rulerLength=200;

GLView::GLView(QWidget* parent) :
	QOpenGLWidget(parent),
	render(nullptr),
	camera(35.0F,0.0F,35.0F,0.0F,500.0F,0.0F),
	showAxes(true),
	showCross(true),
	showBase(true),
	showPrintArea(true),
	showRulers(true),
	showEdges(false),
	skeleton(false),
	printX(0),
	printY(0),
	printWidth(0),
	printLength(0),
	printHeight(0),
	appearance(BedAppearance::MK42),
	mouseDrag(false)
{
	setCursor(Qt::CrossCursor);
}

GLView::~GLView()
{
	delete render;
}

const Camera& GLView::getCamera() const
{
	return camera;
}

void GLView::setCamera(const Camera& c)
{
	camera=c;
	update();
}

void GLView::setPrintOrigin(GLint x,GLint y)
{
	printX=x;
	printY=y;
	update();
}

void GLView::setPrintVolume(GLint w,GLint l,GLint h)
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

void GLView::changeViewDirection(ViewDirections d)
{
	camera.changeViewDirection(d);
	update();
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
	const GLfloat n=value?0.8F:1.0F;
	glClearColor(n,n,n,0.0F);
	if(render)
		render->setCompiling(value);
	update();
}

void GLView::setBedAppearance(BedAppearance v)
{
	appearance=v;
	update();
}

void GLView::initializeGL()
{
	initializeOpenGLFunctions();

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0F,1.0F,1.0F,0.0F);
	GLfloat light_diffuse[]={1.0F,1.0F,1.0F,1.0F};
	GLfloat light_position0[]={-1.0F,-1.0F,+1.0F,0.0F};
	GLfloat light_position1[]={+1.0F,+1.0F,-1.0F,0.0F};

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
	glViewport(0,0,GLint(w),GLint(h));

	glMatrixMode(GL_PROJECTION);

	projection.setToIdentity();
	projection.perspective(45.0F,GLfloat(w)/GLfloat(h),+10.0F,+farfarAway);
	glLoadMatrixf(projection.data());

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
	glVertex2f(-1.0F,-1.0F);
	glVertex2f(1.0F,-1.0F);
	glColor3ub(186,186,186);
	glVertex2f(1.0F,1.0F);
	glVertex2f(-1.0F,1.0F);
	glEnd();
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void GLView::drawAxes()
{
	glLineWidth(1);
	glColor3f(0.0F,0.0F,0.0F);
	glBegin(GL_LINES);
	const int distance=camera.getPositionY();
	const GLfloat c=fmaxf(distance/2.0F,GLfloat(rulerLength));
	glVertex3f(-c,0.0F,0.0F);
	glVertex3f(+c,0.0F,0.0F);
	glVertex3f(0.0F,-c,0.0F);
	glVertex3f(0.0F,+c,0.0F);
	glVertex3f(0.0F,0.0F,-c);
	glVertex3f(0.0F,0.0F,+c);
	glEnd();
}

void GLView::drawBase()
{
	glEnable(GL_CULL_FACE);

	const GLfloat z=-0.01F;
	switch(appearance) {
		case BedAppearance::MK42: {
			const GLfloat baseX=-2.0F;
			const GLfloat baseY=-9.4F;
			const GLfloat baseWidth=254.0F;
			const GLfloat baseLength=235.0F;
			const GLfloat chamfer=4.0F;
			glLineWidth(2);
			glColor3f(0.2F,0.2F,0.2F);
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
			glColor3f(0.6F,0.6F,0.6F);
			glBegin(GL_LINES);
			for(GLint o=0; o<printWidth; o+=10) {
				glVertex3f(printX+o,printY,z);
				glVertex3f(printX+o,printY+printLength,z);
			}
			for(GLint j=5; j<printLength; j+=10) {
				glVertex3f(printX,printY+j,z);
				glVertex3f(printX+printWidth,printY+j,z);
			}
			glEnd();

			glLineWidth(4);
			glColor3f(0.8F,0.8F,0.8F);
			glBegin(GL_LINES);
			for(GLint o=0; o<printWidth; o+=50) {
				glVertex3f(printX+o,printY,z);
				glVertex3f(printX+o,printY+printLength,z);
			}
			for(GLint j=5; j<printLength; j+=50) {
				glVertex3f(printX,printY+j,z);
				glVertex3f(printX+printWidth,printY+j,z);
			}
			glEnd();
		}
		break;
		case BedAppearance::MK2: {
			const GLfloat baseXY=-7.5;
			const GLfloat baseWL=215.0F;
			glLineWidth(2);
			glColor3f(0.6F,0.2F,0.2F);
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
	glColor3f(0.8F,0.8F,0.8F);
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
	glColor3f(0.8F,0.8F,0.8F);
	glBegin(GL_LINE_LOOP);
	glVertex3f(printX,printY,0.0F);
	glVertex3f(printX+printWidth,printY,0.0F);
	glVertex3f(printX+printWidth,printY+printLength,0.0F);
	glVertex3f(printX,printY+printLength,0.0F);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(printX,printY,0.0F);
	glVertex3f(printX,printY,printHeight);

	glVertex3f(printX+printWidth,printY+printLength,0.0F);
	glVertex3f(printX+printWidth,printY+printLength,printHeight);

	glVertex3f(printX,printY+printLength,0.0F);
	glVertex3f(printX,printY+printLength,printHeight);

	glVertex3f(printX+printWidth,printY,0.0F);
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
	glColor3f(0.2F,0.2F,0.2F);
	glBegin(GL_LINES);
	const int distance=camera.getPositionY();
	const int k=distance<200?1:10; //Only show milimeters when close up
	for(int i=-rulerLength; i<rulerLength; i+=k) {
		const int j=i%10?2:5;
		glVertex3i(i,0,0);
		glVertex3i(i,j,0);
	}
	for(int i=-rulerLength; i<rulerLength; i+=k) {
		const int j=i%10?2:5;
		glVertex3i(0,i,0);
		glVertex3i(j,i,0);
	}
	for(int i=-rulerLength; i<rulerLength; i+=k) {
		const int j=i%10?2:5;
		glVertex3i(0,0,i);
		glVertex3i(j,0,i);
	}
	glEnd();
}

void GLView::drawCross()
{
	const GLfloat n=0.2F;
	glLineWidth(5.0F);
	glBegin(GL_LINES);
	glColor3f(1.0F,0.0F,0.0F);
	glVertex3f(printX,printY,n);
	glVertex3f(printX+10.0F,printY,n);

	glColor3f(0.0F,1.0F,0.0F);
	glVertex3f(printX,printY,n);
	glVertex3f(printX,printY+10.0F,n);

	glColor3f(0.0F,0.0F,1.0F);
	glVertex3f(printX-n,printY-n,n);
	glVertex3f(printX-n,printY-n,10.0F);
	glEnd();

	glLineWidth(2.0F);
	renderX(printX+15.0F,printY,3.0F);
	renderY(printX,printY+15.0F,3.0F);
	renderZ(printX-n,printY-n,15.0F);
}

void GLView::paintGL()
{
	glDisable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawGradientBackground();

	glMatrixMode(GL_MODELVIEW);

	modelview.setToIdentity();
	camera.applyTo(modelview);
	glLoadMatrixf(modelview.data());

	if(showAxes) drawAxes();
	if(showBase) drawBase();
	if(showPrintArea) drawPrintArea();
	if(showRulers) drawRulers();
	if(showCross) drawCross();

	glEnable(GL_LIGHTING);

	if(render)
		render->paint(*this,skeleton,showEdges);
}

void GLView::renderX(GLfloat x,GLfloat y,GLfloat z)
{
	const GLfloat d=2.0F;
	glColor3f(1.0F,0.0F,0.0F);
	glBegin(GL_LINES);
	glVertex3f(x-d,y,z-d);
	glVertex3f(x+d,y,z+d);
	glVertex3f(x-d,y,z+d);
	glVertex3f(x+d,y,z-d);
	glEnd();
}

void GLView::renderY(GLfloat x,GLfloat y,GLfloat z)
{
	const GLfloat d=2.0F;
	glColor3f(0.0F,1.0F,0.0F);
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
	const GLfloat d=2.0F;
	glColor3f(0.0F,0.0F,1.0F);
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
#if QT_VERSION < QT_VERSION_CHECK(5,5,0)
	const int delta=event->delta();
#else
	const int delta=event->angleDelta().y();
#endif
	camera.zoom(static_cast<GLfloat>(delta)/12.0F);
	update();
}

void GLView::mouseDoubleClickEvent(QMouseEvent* event)
{
	if(render) {
		const QPointF& mousePos=event->position();
		const int w=width();
		const int h=height();
		const auto x=static_cast<GLfloat>(mousePos.x());
		const auto y=static_cast<GLfloat>(h-mousePos.y());
		makeCurrent();
		GLfloat z;
		glReadPixels(static_cast<GLint>(x),static_cast<GLint>(y),1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&z);
		doneCurrent();
		const QRect viewport(0,0,w,h);
		QVector3D target(x,y,z);
		target = target.unproject(modelview,projection,viewport);
		QVector3D source(x,y,0);
		source = source.unproject(modelview,projection,viewport);

		render->locate(source,target);
	}
}


void GLView::mousePressEvent(QMouseEvent* event)
{
	mouseDrag=true;
	setCursor(Qt::SizeAllCursor);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
	last=event->globalPos();
#else
	last=event->globalPosition();
#endif
}

void GLView::mouseMoveEvent(QMouseEvent* event)
{
	if(!mouseDrag)
		return;

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
	const QPoint current=event->globalPos();
#else
	const QPointF current=event->globalPosition();
#endif
	const auto dx=static_cast<GLfloat>(current.x()-last.x());
	const auto dy=static_cast<GLfloat>(current.y()-last.y());
	const bool shift=QApplication::keyboardModifiers() & Qt::ShiftModifier;
	if(event->buttons() & Qt::LeftButton) {
		camera.pitch(dy);
		if(shift) {
			camera.roll(dx);
		} else {
			camera.yaw(dx);
		}
	} else {
		if(shift) {
			camera.zoom(-dy);
		} else {
			camera.pan(dx,-dy);
		}
	}
	update();

	last=current;
}

void GLView::mouseReleaseEvent(QMouseEvent*)
{
	mouseDrag=false;
	setCursor(Qt::CrossCursor);
}
