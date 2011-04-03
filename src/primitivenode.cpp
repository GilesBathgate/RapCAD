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
	res.append("polyhedron([");
	QVector<QString> ptlist;
	foreach(Polygon pg, polygons) {
		foreach(Point p, pg) {
			QString pt = toString(p);
			if(!ptlist.contains(pt))
				ptlist.append(pt);
		}
	}

	for(int i=0; i<ptlist.size(); i++) {
		QString pt = ptlist.at(i);
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
			QString pt = toString(p);
			int i = ptlist.indexOf(pt);
			res.append(QString().setNum(i));
		}
		res.append("]");
	}
	res.append("]);");
	return res;
}

QString PrimitiveNode::toString(Point p)
{
	QString res;
	res.append("[");
	res.append(toString(p.x));
	res.append(",");
	res.append(toString(p.y));
	res.append(",");
	res.append(toString(p.z));
	res.append("]");

	return res;
}

QString PrimitiveNode::toString(double d)
{
	QString res;
	res.setNum(d,'f',16);
	int j=0;
	//Trim trailing zeros. res will always be
	//in the form X.XX.. so we can cheat here
	for(int i=res.size()-1; i>=0; i--) {
		if(res.at(i)!='0') {
			if(res.at(i)=='.')
				j++;
			break;
		} else {
			j++;
		}
	}
	res.chop(j);

	return res;
}
