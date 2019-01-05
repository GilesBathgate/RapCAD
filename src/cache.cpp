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

#include "cache.h"

Cache::Cache() :
	index(0)
{
}

Cache::~Cache()
{
	map.clear();
	qDeleteAll(allPrimitives);
	allPrimitives.clear();
}

int Cache::hashValue(const decimal& v)
{
	int i=map.value(v,-1);
	if(i<0) {
		i=index;
		map.insert(v,i);
		index++;
	}

	return i;
}

Cache::i_Point Cache::hashPoint(const Point& p)
{
	i_Point ip;
	ip.append(hashValue(p.x()));
	ip.append(hashValue(p.y()));
	ip.append(hashValue(p.z()));

	return ip;
}

Cache::i_Polygon Cache::hashPolygon(Polygon* pg)
{
	i_Polygon ip;
	for(auto i: pg->getIndexes())
		ip.append(i);

	return ip;
}

Cache::i_Primitive Cache::hashPrimitive(Primitive* pr)
{
	i_PointList pi;
	for(const auto& pt: pr->getPoints())
		pi.append(hashPoint(pt));

	i_PolygonList pgi;
	for(Polygon* pg: pr->getPolygons())
		pgi.append(hashPolygon(pg));

	return i_Primitive(pi,pgi);
}

Primitive* Cache::fetch(Primitive* pr)
{
	if(pr) {
		i_Primitive ip=hashPrimitive(pr);
		Primitive* np=allPrimitives.value(ip,nullptr);
		if(np) {
			return np->copy();
		}
		allPrimitives.insert(ip,pr->copy());
	}
	return pr;
}
