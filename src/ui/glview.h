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

#ifndef GLVIEW_H
#define GLVIEW_H

#ifdef USE_QGLWIDGET
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
#ifdef USE_QGLWIDGET
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

	enum Appearance_t {
		MK42 = 0,
		MK2 = 1
	};

	explicit GLView(QWidget* parent = nullptr);
	~GLView() override;
	void setRenderer(Renderer* r);
	void setCompiling(bool value);
	void setBedAppearance(Appearance_t);
	void preferencesUpdated();

#ifdef USE_QGLWIDGET
	inline QImage grabFramebuffer(){ return grabFrameBuffer(); }
#endif

public slots:
	void setViewport(GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat);
	void setPrintOrigin(GLfloat,GLfloat);
	void setPrintVolume(GLfloat,GLfloat,GLfloat);
	void getViewport(GLfloat&,GLfloat&,GLfloat&,GLfloat&,GLfloat&,GLfloat&);
	void setShowAxes(bool);
	void setShowRulers(bool);
	void setShowBase(bool);
	void setShowPrintArea(bool);
	void setSkeleton(bool);
	void setShowEdges(bool);
	void changeViewport(int);
private:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void renderX(double,double,double);
	void renderY(double,double,double);
	void renderZ(double,double,double);

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

	void normalizeAngle(GLfloat&);
	void zoomView(float);
	void drawGradientBackground();
	void drawAxes();
	void drawBase();
	void drawPrintArea();
	void drawRulers();
	void drawCross();

#ifndef USE_QGLWIDGET
	QMatrix4x4* projection;
	QMatrix4x4* modelview;
#endif
	Renderer* render;
	GLfloat distance;
	bool showAxes;
	bool showCross;
	bool showBase;
	bool showPrintArea;
	bool showRulers;
	bool showEdges;
	bool skeleton;
	GLfloat printX;
	GLfloat printY;
	GLfloat printWidth;
	GLfloat printLength;
	GLfloat printHeight;
	Appearance_t appearance;

	bool mouseDrag;
	QPoint last;
	GLfloat rotateX;
	GLfloat rotateY;
	GLfloat rotateZ;
	GLint viewportX;
	GLint viewportZ;
};

#endif // GLVIEW_H
