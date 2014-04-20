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
#if USE_CGAL
#ifndef CGALPRIMITIVE_H
#define CGALPRIMITIVE_H

#include <QVector>
#include "cgalpolygon.h"
#include "cgalvolume.h"
#include "primitive.h"

class CGALPrimitive : public Primitive
{
public:
	CGALPrimitive();
	CGALPrimitive(CGAL::Polyhedron3 poly);
	void setType(Primitive_t);
	Polygon* createPolygon();
	void appendVertex(Point);
	void appendVertex(CGAL::Point3);
	void prependVertex(Point);
	void prependVertex(CGAL::Point3);
	bool overlaps(Primitive*);
	Primitive* group(Primitive*);
	Primitive* join(Primitive*);
	void add(Primitive*,bool);
	Primitive* combine();
	Primitive* intersection(Primitive*);
	Primitive* difference(Primitive*);
	Primitive* symmetric_difference(Primitive*);
	Primitive* minkowski(Primitive*);
	Primitive* inset(const decimal);
	Primitive* decompose();
	Primitive* complement();
	Primitive* copy();
	CGAL::Cuboid3 getBounds();
	void transform(const CGAL::AffTransformation3&);
	/* Don't call this method instead use getCGALPolygons */
	QList<Polygon*> getPolygons() const;
	QList<CGALPolygon*> getCGALPolygons() const;
	/* Don't call this method instead use getCGALPoints */
	QList<Point> getPoints() const;
	QList<CGAL::Point3> getCGALPoints() const;
	const CGAL::NefPolyhedron3& getNefPolyhedron();
	CGAL::Polyhedron3* getPolyhedron();
	bool isEmpty();
	bool isFullyDimentional();
	QList<Primitive*> getChildren();
	void appendChild(Primitive*);
	CGAL::Circle3 getRadius();
	CGALVolume getVolume(bool);
private:
	void init();
	void buildPrimitive();
	QList<Primitive*> children;
	CGAL::NefPolyhedron3* createPolyline(QVector<CGAL::Point3> pl);
	QList<CGALPolygon*> polygons;
	CGAL::NefPolyhedron3* nefPolyhedron;
	Primitive_t type;

	/* Simple wrapper class to enable Primitive
	 * to be used with CGAL::Nef_nary_union_3 */
	class Unionable
	{
	public:
		Unionable() {}
		Unionable(Primitive* p, bool f) { primitive=p; force=f; }
		Unionable& operator+(Unionable&);
		Primitive* primitive;
		bool force;
	};

	CGAL::Nef_nary_union_3<Unionable>* nUnion;
	static CGAL::NefPolyhedron3* singlePoint;
};

#endif // CGALPRIMITIVE_H
#endif
