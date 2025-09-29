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

#include "camera.h"
#include <QVector3D>
#include <cmath>

Camera::Camera(GLfloat rx, GLfloat ry, GLfloat rz, GLfloat px, GLfloat py, GLfloat pz) :
	rotateX(rx),
	rotateY(ry),
	rotateZ(rz),
	positionX(px),
	positionY(py),
	positionZ(pz)
{
}

void Camera::normalizeAngle(GLfloat& angle)
{
	while(angle < 0.0F)
		angle+=360.0F;
	while(angle > 360.0F)
		angle-=360.0F;
}

GLfloat Camera::getRotateX() const
{
	return rotateX;
}

GLfloat Camera::getRotateY() const
{
	return rotateY;
}

GLfloat Camera::getRotateZ() const
{
	return rotateZ;
}

GLfloat Camera::getPositionX() const
{
	return positionX;
}

GLfloat Camera::getPositionY() const
{
	return positionY;
}

GLfloat Camera::getPositionZ() const
{
	return positionZ;
}

void Camera::applyTo(QMatrix4x4& modelview) const
{
	const QVector3D eye(-positionX,-positionY,-positionZ);
	const QVector3D center(-positionX,0.0F,-positionZ);
	const QVector3D up(0.0F,0.0F,1.0F);
	modelview.lookAt(eye,center,up);

	modelview.rotate(rotateX,1.0F,0.0F,0.0F);
	modelview.rotate(rotateY,0.0F,1.0F,0.0F);
	modelview.rotate(rotateZ,0.0F,0.0F,1.0F);
}

void Camera::setRotation(GLfloat x,GLfloat y,GLfloat z)
{
	rotateX=x;
	rotateY=y;
	rotateZ=z;
}

void Camera::changeViewDirection(ViewDirections d)
{
	switch(d) {
		case ViewDirections::Top:
			setRotation(90.0F,0.0F,0.0F);
			break;
		case ViewDirections::Bottom:
			setRotation(-90.0F,0.0F,0.0F);
			break;
		case ViewDirections::North:
			setRotation(0.0F,0.0F,-180.0F);
			break;
		case ViewDirections::South:
			setRotation(0.0F,0.0F,0.0F);
			break;
		case ViewDirections::West:
			setRotation(0.0F,0.0F,90.0F);
			break;
		case ViewDirections::East:
			setRotation(0.0F,0.0F,-90.0F);
			break;
	}
}

void Camera::pitch(GLfloat dx)
{
	rotateX+=dx;
	normalizeAngle(rotateX);
}

void Camera::roll(GLfloat dy)
{
	rotateY+=dy;
	normalizeAngle(rotateY);
}

void Camera::yaw(GLfloat dz)
{
	rotateZ+=dz;
	normalizeAngle(rotateZ);
}

void Camera::pan(GLfloat dx,GLfloat dz)
{
	positionX+=dx;
	positionZ+=dz;
}

void Camera::zoom(GLfloat amt)
{
	positionY*=powf(0.9F,amt/10.0F);
}
