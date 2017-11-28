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

#include "point.h"

bool Point::operator ==(const Point that) const
{
	return _x==that._x&&_y==that._y&&_z==that._z;
}

decimal Point::getX() const
{
	return _x;
}

decimal Point::getY() const
{
	return _y;
}

decimal Point::getZ() const
{
	return _z;
}

Point Point::transform(TransformMatrix* matrix) const
{
	decimal* m=matrix->getValues();
	decimal nx,ny,nz;
	nx=(m[ 0]*_x+m[ 1]*_y+m[ 2]*_z+m[ 3]);
	ny=(m[ 4]*_x+m[ 5]*_y+m[ 6]*_z+m[ 7]);
	nz=(m[ 8]*_x+m[ 9]*_y+m[10]*_z+m[11]);
	return Point(nx,ny,nz);
}

void Point::getXYZ(decimal& x, decimal& y, decimal& z) const
{
	x=_x;
	y=_y;
	z=_z;
}

void Point::getXY(decimal& x, decimal& y) const
{
	x=_x;
	y=_y;
}

QString Point::toString() const
{
	return toString(true);
}

QString Point::toString(bool trim) const
{
	QString res;
	res.append("[");
	res.append(to_string(_x,trim));
	res.append(",");
	res.append(to_string(_y,trim));
	res.append(",");
	res.append(to_string(_z,trim));
	res.append("]");

	return res;
}

