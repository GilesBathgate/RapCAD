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

#ifndef GLVIEW_H
#define GLVIEW_H

#include "bedappearance.h"
#include "camera.h"
#include "renderer.h"
#include "viewdirections.h"
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLFunctions_2_0>
#include <QOpenGLWidget>
#include <QWheelEvent>

class GLView :
	public QOpenGLWidget, private QOpenGLFunctions_2_0
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE(GLView)
public:
	explicit GLView(QWidget* parent=nullptr);
	~GLView() override;
	void setRenderer(Renderer* r);
	void setCompiling(bool value);
	void setBedAppearance(BedAppearance);
	void preferencesUpdated();
	void setPrintOrigin(GLint,GLint);
	void setPrintVolume(GLint,GLint,GLint);
	const Camera& getCamera() const;
	void setCamera(const Camera&);

public slots:
	void setShowAxes(bool);
	void setShowRulers(bool);
	void setShowBase(bool);
	void setShowPrintArea(bool);
	void setSkeleton(bool);
	void setShowEdges(bool);
	void changeViewDirection(ViewDirections);

private:
	void initializeGL() override;
	void resizeGL(int w,int h) override;
	void paintGL() override;
	void renderX(GLfloat,GLfloat,GLfloat);
	void renderY(GLfloat,GLfloat,GLfloat);
	void renderZ(GLfloat,GLfloat,GLfloat);

	void mouseDoubleClickEvent(QMouseEvent*) override;
	void mousePressEvent(QMouseEvent*) override;
	void mouseMoveEvent(QMouseEvent*) override;
	void mouseReleaseEvent(QMouseEvent*) override;
	void wheelEvent(QWheelEvent*) override;

	void drawGradientBackground();
	void drawAxes();
	void drawBase();
	void drawPrintArea();
	void drawRulers();
	void drawCross();

	QMatrix4x4 projection;
	QMatrix4x4 modelview;
	Renderer* render;
	Camera camera;
	bool showAxes;
	bool showCross;
	bool showBase;
	bool showPrintArea;
	bool showRulers;
	bool showEdges;
	bool skeleton;
	GLint printX;
	GLint printY;
	GLint printWidth;
	GLint printLength;
	GLint printHeight;
	BedAppearance appearance;

	bool mouseDrag;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
	QPoint last;
#else
	QPointF last;
#endif

};

#endif // GLVIEW_H
