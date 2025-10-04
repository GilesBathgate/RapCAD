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

#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLFunctions_2_0>
#include <QVector3D>

class Renderer
{
	Q_DISABLE_COPY_MOVE(Renderer)
protected:
	Renderer()=default;
public:
	virtual ~Renderer()=default;
	virtual void paint(QOpenGLFunctions_2_0&,bool,bool)=0;
	virtual void locate(const QVector3D&,const QVector3D&)=0;
	virtual void preferencesUpdated()=0;
	virtual void setCompiling(bool)=0;
};

#endif // RENDERER_H
