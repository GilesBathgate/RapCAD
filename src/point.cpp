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

#include "point.h"

bool Point::operator ==(const Point that) const
{
	return this->x==that.x&&this->y==that.y&&this->z==that.z;
}

decimal Point::getX() const
{
	return x;
}

decimal Point::getY() const
{
	return y;
}

decimal Point::getZ() const
{
	return z;
}

Point Point::transform(TransformMatrix* matrix)
{
	decimal* m=matrix->getValues();
	decimal nx,ny,nz;
	nx=(m[ 0]*x+m[ 1]*y+m[ 2]*z+m[ 3]);
	ny=(m[ 4]*x+m[ 5]*y+m[ 6]*z+m[ 7]);
	nz=(m[ 8]*x+m[ 9]*y+m[10]*z+m[11]);
	return Point(nx,ny,nz);
}

void Point::getXYZ(decimal& x, decimal& y, decimal& z) const
{
	x=this->x;
	y=this->y;
	z=this->z;
}

void Point::getXY(decimal& x, decimal& y) const
{
	x=this->x;
	y=this->y;
}

QString Point::toString() const
{
	return toString(true);
}

QString Point::toString(bool trim) const
{
	QString res;
	res.append("[");
	res.append(to_string(x,trim));
	res.append(",");
	res.append(to_string(y,trim));
	res.append(",");
	res.append(to_string(z,trim));
	res.append("]");

	return res;
}

