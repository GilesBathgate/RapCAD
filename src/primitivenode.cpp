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

#include "primitivenode.h"

PrimitiveNode::PrimitiveNode()
{
}

void PrimitiveNode::createPolygon()
{
	polygons.append(Polygon());
}

void PrimitiveNode::appendVertex(double x, double y, double z)
{
	polygons.last().append(Point(x,y,z));
}

QString PrimitiveNode::toString()
{
	QString res;
	foreach(Polygon pg, polygons) {
		res.append("[");
		foreach(Point p, pg)
			res.append(
				QString("[%1,%2,%3]")
				.arg(p.x,0,'g',16)
				.arg(p.y,0,'g',16)
				.arg(p.z,0,'g',16)
			);

		res.append("]\n");
	}

	return res;
}
