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

#ifndef POLYGON_H
#define POLYGON_H

class Primitive;
#include <QtGlobal>

#ifdef Q_OS_WIN
#include <windows.h>
#define Polygon _Polygon
#endif

#include "point.h"
#include <QList>

class Polygon
{
public:
	using size_type=QList<Point>::size_type;
	explicit Polygon(Primitive&);
	void append(size_type);
	void prepend(size_type);
	QList<Point> getPoints() const;
	const QList<size_type>& getIndexes() const;
	void setIndexes(const QList<size_type>& value);
protected:
	QList<size_type> indexes;
	Primitive& parent;
};

#endif // POLYGON_H
