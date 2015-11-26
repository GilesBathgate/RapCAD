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

Cache::i_Point Cache::hashPoint(const Point& pt)
{
	i_Point ip;
	decimal x,y,z;
	pt.getXYZ(x,y,z);
	ip.append(hashValue(x));
	ip.append(hashValue(y));
	ip.append(hashValue(z));

	return ip;
}

Cache::i_Polygon Cache::hashPolygon(Polygon* pg)
{
	i_Polygon ip;
	foreach(int i, pg->getIndexes())
		ip.append(i);

	return ip;
}

Cache::i_Primitive Cache::hashPrimitive(Primitive* pr)
{
	i_PointList pi;
	foreach(Point pt, pr->getPoints())
		pi.append(hashPoint(pt));

	i_PolygonList pgi;
	foreach(Polygon* pg, pr->getPolygons())
		pgi.append(hashPolygon(pg));

	return i_Primitive(pi,pgi);
}

Primitive* Cache::fetch(Primitive* pr)
{
	if(pr) {
		i_Primitive ip=hashPrimitive(pr);
		Primitive* np=allPrimitives.value(ip,NULL);
		if(np) {
			return np->copy();
		}
		allPrimitives.insert(ip,pr->copy());
	}
	return pr;
}
