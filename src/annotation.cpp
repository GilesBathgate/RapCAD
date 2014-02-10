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
#include "annotation.h"

Polygon* Annotation::createPolygon()
{
	Polygon* pg = new Polygon();
	polygons.append(pg);
	return pg;
}

void Annotation::appendVertex(Point p)
{
	if(polygons.count()>0)
		polygons.last()->append(p);
}

void Annotation::prependVertex(Point p)
{
	if(polygons.count()>0)
		polygons.last()->prepend(p);
}

Primitive* Annotation::copy()
{
	Annotation* c=new Annotation();
	foreach(Polygon* pg, polygons) {
		c->createPolygon();
		foreach(Point p, pg->getPoints())
			c->appendVertex(p);
	}
	return c;
}

bool Annotation::isEmpty()
{
	return polygons.count()<=0;
}
