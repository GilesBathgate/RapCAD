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

#ifndef POLYGON_H
#define POLYGON_H

class Primitive;
#include <qglobal.h>

#ifdef Q_OS_WIN
#include <windows.h>
#define Polygon _Polygon
#endif

#include <QList>
#include "point.h"

class Polygon
{
public:
	explicit Polygon(const Primitive&);
	void append(int);
	void prepend(int);
	QList<Point> getPoints() const;
	QList<int> getIndexes() const;
	void setIndexes(const QList<int>& value);
protected:
	QList<int> indexes;
	const Primitive& parent;
};

#endif // POLYGON_H
