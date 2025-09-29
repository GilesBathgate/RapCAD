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

#ifndef CAMERA_H
#define CAMERA_H

#include "viewdirections.h"
#include <QMatrix4x4>
#include <qopengl.h>

class Camera
{
public:
	Camera(GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat);
	void applyTo(QMatrix4x4&) const;
	void setRotation(GLfloat,GLfloat,GLfloat);
	void changeViewDirection(ViewDirections);
	void pitch(GLfloat dx);
	void roll(GLfloat dy);
	void yaw(GLfloat dz);
	void pan(GLfloat dx,GLfloat dz);
	void zoom(GLfloat);

	GLfloat getRotateX() const;
	GLfloat getRotateY() const;
	GLfloat getRotateZ() const;
	GLfloat getPositionX() const;
	GLfloat getPositionY() const;
	GLfloat getPositionZ() const;

private:
	static void normalizeAngle(GLfloat&);
	GLfloat rotateX;
	GLfloat rotateY;
	GLfloat rotateZ;
	GLfloat positionX;
	GLfloat positionY;
	GLfloat positionZ;
};

#endif // CAMERA_H
