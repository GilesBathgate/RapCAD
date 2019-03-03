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
	virtual ~Cache();
	virtual Primitive* fetch(Primitive*);
protected:
	typedef QVector<int> i_Point;
	typedef QVector<i_Point> i_PointList;
	typedef QVector<int> i_Polygon;
	typedef QVector<i_Polygon> i_PolygonList;
	typedef QPair<i_PointList,i_PolygonList> i_Primitive;

	int hashValue(const decimal&);
	i_Point hashPoint(const Point&);
	i_Polygon hashPolygon(Polygon*);
	virtual i_Primitive hashPrimitive(Primitive*);
private:
	int index;
	QMap<decimal,int> map;
	QHash<i_Primitive,Primitive*> allPrimitives;
};

#if QT_VERSION < 0x050600
template <class T>
inline uint qHash(const QVector<T>& l,uint seed)
{
	uint h = seed;
	foreach(T i,l)
		h = 31 * h + qHash(i,seed);

	return h;
}
#endif

#endif // CACHE_H
