/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2012 Giles Bathgate
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

class Point
{
public:
	Point() : x(0), y(0), z(0) { }
	Point(double x,double y,double z) : x(x), y(y), z(z) { }
	bool operator==(const Point that) const;
	QString toString() const;
	void getXYZ(double& x, double& y, double& z) const;
	void getXY(double& x, double& y) const;
	double getX() const;
	double getY() const;
	double getZ() const;
private:
	QString toString(const double) const;
	double x, y, z;
};

#endif // POINT_H
