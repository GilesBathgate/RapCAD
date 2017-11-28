#include "cache.h"

Cache::Cache()
{
	index=0;
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
