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

#ifdef USE_CGAL

#include "cgalcache.h"

CGALCache::CGALCache()
{
}

Cache::i_Point CGALCache::hashPoint(const CGAL::Point3& pt)
{
	i_Point ip;
	ip.append(hashValue(pt.x()));
	ip.append(hashValue(pt.y()));
	ip.append(hashValue(pt.z()));

	return ip;
}

Cache::i_Primitive CGALCache::hashPrimitive(Primitive* pr)
{
	auto* cg=dynamic_cast<CGALPrimitive*>(pr);
	if(cg) {
		return hashPrimitive(cg);
	}
	return Cache::hashPrimitive(pr);
}

Cache::i_Primitive CGALCache::hashPrimitive(CGALPrimitive* cg)
{
	i_PointList pi;
	for(const auto& pt: cg->getPoints())
		pi.append(hashPoint(pt));

	i_PolygonList pgi;
	for(CGALPolygon* pg: cg->getCGALPolygons())
		pgi.append(hashPolygon(pg));

	return i_Primitive(pi,pgi);
}

#endif
