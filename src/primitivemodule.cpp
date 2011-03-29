/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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

#include "primitivemodule.h"
#include "tau.h"

PrimitiveModule::PrimitiveModule()
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


PrimitiveNode::Polygon PrimitiveModule::getCircle(double r, double f)
{
	PrimitiveNode::Polygon circle;
	for(int i=0; i<f; i++) {
		double phi = (M_TAU*i) / f;
		PrimitiveNode::Point p;
		if(r > 0) {
			p.x = r*cos(phi);
			p.y = r*sin(phi);
		} else {
			p.x=0;
			p.y=0;
		}
		circle.append(p);
	}

	return circle;
}
