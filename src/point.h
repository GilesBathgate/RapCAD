/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2017 Giles Bathgate
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

#ifndef POINT_H
#define POINT_H

#include <QString>
#include "decimal.h"
#include "transformmatrix.h"

class Point
{
public:
	Point() : _x(0), _y(0), _z(0) { }
	Point(decimal x,decimal y,decimal z) : _x(x), _y(y), _z(z) { }
	bool operator==(const Point that) const;
	QString toString() const;
	QString toString(const bool) const;
	decimal x() const;
	decimal y() const;
	decimal z() const;
	Point transform(TransformMatrix*) const;
private:
	decimal _x, _y, _z;
};

#endif // POINT_H
