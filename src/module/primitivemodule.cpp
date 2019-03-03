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

#include "rmath.h"
#include "primitivemodule.h"
#include "context.h"
#include "numbervalue.h"

PrimitiveModule::PrimitiveModule(Reporter& r, const QString &n) : Module(r,n)
{
}

QList<Point> PrimitiveModule::getCircle(const decimal& r,const decimal& f,const decimal& z) const
{
	QList<Point> circle;
	for(auto i=0; i<f; ++i) {
		decimal phi = (r_tau()*i) / f;
		decimal x,y;
		if(r > 0) {
			x = r*r_cos(phi);
			y = r*r_sin(phi);
		} else {
			x=0;
			y=0;
		}
		Point p(x,y,z);
		circle.append(p);
	}

	return circle;
}

QList<Point> PrimitiveModule::getPolygon(const decimal& a,const decimal& r,const decimal& n,const decimal& z) const
{
	QList<Point> poly;
	if(n==6) {
		//TODO modify this to cater for all even values of n
		decimal x=0,y=0;
		decimal s2=r*r_sin(r_pi()/n);
		for(auto i=0; i<n; ++i) {
			switch(i) {
				case 0: {
					y=a;
					x=-s2;
					break;
				}
				case 1: {
					x=s2;
					break;
				}
				case 2: {
					y=0;
					x=r;
					break;
				}
				case 3: {
					y=-a;
					x=s2;
					break;
				}
				case 4: {
					x=-s2;
					break;
				}
				case 5: {
					y=0;
					x=-r;
					break;
				}
			}
			poly.append(Point(x,y,z));
		}
		return poly;
	} else {
		return getCircle(r,n,z);
	}
}
