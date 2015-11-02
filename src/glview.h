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

#ifndef GLVIEW_H
#define GLVIEW_H

#include <QtGlobal>
#define REQUIRED QT_VERSION_CHECK(5, 4, 0)

#if (QT_VERSION >= REQUIRED)
#include <QOpenGLWidget>
#include <QOpenGLFunctions_1_0>
#include <QMatrix4x4>
#else
#include <QGLWidget>
#endif
#include <QMouseEvent>
#include <QWheelEvent>
#include "renderer.h"

class GLView :
#if (QT_VERSION >= REQUIRED)
	public QOpenGLWidget, private QOpenGLFunctions_1_0
#else
	public QGLWidget
#endif
{
	Q_OBJECT

public:
	GLView(QWidget* parent = NULL);
	void setRenderer(Renderer* r);
	void setCompiling(bool value);

#if (QT_VERSION < REQUIRED)
	inline QImage grabFramebuffer(){ return grabFrameBuffer(); }
#endif

public slots:
	void preferencesUpdated();
	void setViewport(double,double,double,double,double,double);
	void getViewport(double&,double&,double&,double&,double&,double&);
	void setShowAxes(bool);
	void setShowRulers(bool);
	void setShowBase(bool);
	void setShowPrintArea(bool);
	void setSkeleton(bool);
	void setShowEdges(bool);
private:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

#if (QT_VERSION < REQUIRED)
	inline void update(){ updateGL(); }
#endif

	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent* event);

	void normalizeAngle(GLfloat&);
	void zoomView(double);

	Renderer* render;
	GLfloat distance;
	bool showAxes;
	bool showBase;
	bool showPrintArea;
	bool showRulers;
	bool showEdges;
	bool skeleton;

	bool mouseDrag;
	QPoint last;
	GLfloat rotateX;
	GLfloat rotateY;
	GLfloat rotateZ;
	GLint viewportX;
	GLint viewportZ;
#if (QT_VERSION >= REQUIRED)
	QMatrix4x4* projection;
	QMatrix4x4* modelview;
#endif
};

#endif // GLVIEW_H
