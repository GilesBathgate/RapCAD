/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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

#include "primitive.h"
#include <QHash>
#include <QMap>
#include <QVector>
#include <QtGlobal>

class Cache
{
	Q_DISABLE_COPY_MOVE(Cache)
public:
	virtual ~Cache();
	virtual Primitive* fetch(Primitive*);
protected:
	Cache();
	using i_Point = QVector<int> ;
	using i_PointList = QVector<i_Point>;
	using i_Polygon = QVector<i_Point::size_type>;
	using i_PolygonList = QVector<i_Polygon>;
	using i_Primitive = QPair<i_PointList,i_PolygonList>;

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
