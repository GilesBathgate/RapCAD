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

#include <math.h>
#include "primitivemodule.h"
#include "tau.h"

PrimitiveModule::PrimitiveModule(const QString n) : Module(n)
{
}

/**
* Get the number of fragments of a circle, given radius and
* the three special variables $fn, $fs and $fa
*/
int PrimitiveModule::getFragments(double r, double fn, double fs, double fa)
{
	const double GRID_FINE = 0.000001;
	if(r < GRID_FINE)
		return 0;

	if(fn > 0.0)
		return (int)fn;

	return (int)ceil(fmax(fmin(360.0 / fa, r*M_PI / fs), 5));
}


Polygon PrimitiveModule::getCircle(double r, double f, double z)
{
	Polygon circle;
	for(int i=0; i<f; i++) {
		double phi = (M_TAU*i) / f;
		double x,y;
		if(r > 0) {
			x = r*cos(phi);
			y = r*sin(phi);
		} else {
			x=0;
			y=0;
		}
		Point p(x,y,z);
		circle.append(p);
	}

	return circle;
}

Polygon PrimitiveModule::getPolygon(double a,double r, double n, double z)
{
	Polygon poly;
	if(n==6) {
		//TODO modify this to cater for all even values of n
		double x=0,y=0;
		double s2=r*sin(M_PI/n);
		for(int i=0; i<n; i++){
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
