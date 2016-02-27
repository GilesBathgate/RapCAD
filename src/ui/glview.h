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

#if USE_QGLWIDGET
#include <QGLWidget>
#else
#include <QOpenGLWidget>
#include <QOpenGLFunctions_1_0>
#include <QMatrix4x4>
#endif
#include <QMouseEvent>
#include <QWheelEvent>
#include "renderer.h"

class GLView :
#if USE_QGLWIDGET
	public QGLWidget
#else
	public QOpenGLWidget, private QOpenGLFunctions_1_0
#endif
{
	Q_OBJECT

public:
	enum View_t {
		Top,
		Bottom,
		Left,
		Right,
		Back,
		Front
	};

	GLView(QWidget* parent = NULL);
	~GLView();
	void setRenderer(Renderer* r);
	void setCompiling(bool value);

#if USE_QGLWIDGET
	inline QImage grabFramebuffer(){ return grabFrameBuffer(); }
#endif

public slots:
	void preferencesUpdated();
	void setViewport(float,float,float,float,float,float);
	void getViewport(float&,float&,float&,float&,float&,float&);
	void setShowAxes(bool);
	void setShowRulers(bool);
	void setShowBase(bool);
	void setShowPrintArea(bool);
	void setSkeleton(bool);
	void setShowEdges(bool);
	void changeViewport(int);
private:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

#if USE_QGLWIDGET
	inline void update(){ updateGL(); }
#endif

	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent* event);

	void normalizeAngle(GLfloat&);
	void zoomView(float);

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
#if !USE_QGLWIDGET
	QMatrix4x4* projection;
	QMatrix4x4* modelview;
#endif
};

#endif // GLVIEW_H
