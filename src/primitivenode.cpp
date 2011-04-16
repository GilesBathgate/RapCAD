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
	appendVertex(Point(x,y,z));
}

void PrimitiveNode::appendVertex(Point p)
{
	polygons.last().append(p);
}

QString PrimitiveNode::toString()
{
	QString res;
	res.append("polyhedron([");
	QVector<Point> ptlist;
	foreach(Polygon pg, polygons) {
		foreach(Point p, pg) {
			if(!ptlist.contains(p))
				ptlist.append(p);
		}
	}

	for(int i=0; i<ptlist.size(); i++) {
		Point p = ptlist.at(i);
		QString pt = p.toString();
		if(i>0)
			res.append(",");
		res.append(pt);
	}
	res.append("],[");

	for(int i=0; i<polygons.size(); i++) {
		Polygon pg = polygons.at(i);
		if(i>0)
			res.append(",");
		res.append("[");
		for(int j=0; j<pg.size(); j++) {
			if(j>0)
				res.append(",");
			Point p = pg.at(j);
			int i = ptlist.indexOf(p);
			res.append(QString().setNum(i));
		}
		res.append("]");
	}
	res.append("]);");
	return res;
}
