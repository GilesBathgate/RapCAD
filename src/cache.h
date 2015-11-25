#ifndef CACHE_H
#define CACHE_H

#include <QtGlobal>
#include <QVector>
#include <QMap>
#include <QHash>
#include "primitive.h"

class Cache
{
public:
	Cache();
	~Cache();
	Primitive* fetch(Primitive*);
	void store(Primitive*);
private:
	typedef QVector<int> i_Point;
	typedef QVector<i_Point> i_PointList;
	typedef QVector<int> i_Polygon;
	typedef QVector<i_Polygon> i_PolygonList;
	typedef QPair<i_PointList,i_PolygonList> i_Primitive;

	int hashValue(const decimal&);
	i_Point hashPoint(const Point&);
	i_Polygon hashPolygon(Polygon*);
	i_Primitive hashPrimitive(Primitive*);
	Primitive* duplicate(Primitive*);

	int index;
	QMap<decimal,int> map;
	QHash<i_Primitive,Primitive*> allPrimitives;
	i_Primitive iprimitive;
};

template <class T>
inline uint qHash(const QVector<T>& l,uint seed)
{
	uint h = seed;
	foreach(T i,l)
		h = 31 * h + qHash(i,seed);

	return h;
}
#endif // CACHE_H
