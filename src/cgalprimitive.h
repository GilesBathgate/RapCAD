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
#ifndef CGALPRIMITIVE_H
#define CGALPRIMITIVE_H

#include "cgal.h"

#include <QVector>
#include <QMap>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Nef_nary_union_3.h>
#include "cgalpolygon.h"
#include "cgalvolume.h"
#include "primitive.h"

namespace CGAL
{
typedef Polyhedron_3<Kernel3> Polyhedron3;
typedef Nef_polyhedron_3<Kernel3> NefPolyhedron3;
}

class CGALPrimitive : public Primitive
{
public:
	CGALPrimitive();
	~CGALPrimitive() override;
	explicit CGALPrimitive(CGAL::Polyhedron3&);
	explicit CGALPrimitive(const CGAL::NefPolyhedron3&);
	void setType(Primitive_t) override;
	Primitive_t getType() override;
	void setSanitized(bool) override;
	bool getSanitized() override;
	Polygon* createPolygon() override;
	CGALPolygon* createCGALPolygon();
	void createVertex(const CGAL::Point3&) override;
	void createVertex(const CGAL::Scalar&, const CGAL::Scalar&, const CGAL::Scalar&);
	void addVertex(const CGAL::Point3&,bool);
	void appendVertex(const CGAL::Point3&);
	bool overlaps(Primitive*) override;
	Primitive* group(Primitive*) override;
	Primitive* join(Primitive*) override;
	void add(Primitive*,bool) override;
	Primitive* combine() override;
	Primitive* intersection(Primitive*) override;
	Primitive* difference(Primitive*) override;
	Primitive* symmetric_difference(Primitive*) override;
	Primitive* minkowski(Primitive*) override;
	Primitive* inset(const CGAL::Scalar&) override;
	Primitive* decompose() override;
	Primitive* complement() override;
	Primitive* boundary() override;
	Primitive* copy() override;
	Primitive* triangulate() override;
	Primitive* simplify(const CGAL::Scalar&) override;
	CGAL::Cuboid3 getBounds();
	void transform(TransformMatrix*) override;
	/* Don't call this method instead use getCGALPolygons */
	Q_DECL_DEPRECATED QList<Polygon*> getPolygons() const override;
	QList<CGALPolygon*> getCGALPolygons() const;
	QList<CGAL::Point3> getPoints() const override;
	const CGAL::NefPolyhedron3& getNefPolyhedron();
	CGAL::Polyhedron3* getPolyhedron();
	bool isEmpty() override;
	bool isFullyDimentional() override;
	QList<Primitive*> getChildren() override;
	void appendChild(Primitive*) override;
	void discrete(int) override;
	CGAL::Circle3 getRadius();
	CGALVolume getVolume(bool);
	bool detectHoles(bool);
	void clear();
	void clearPolygons();
private:
	void buildPrimitive();
	CGAL::NefPolyhedron3* createVolume();
	CGAL::NefPolyhedron3* createPolyline();
	CGAL::NefPolyhedron3* createPolyline(QVector<CGAL::Point3>);
	CGAL::NefPolyhedron3* createPoints();

	/**
	 * @brief Find the index of the point or add it to the points list
	 * @param p The point to find or add.
	 * @return The index of the point.
	 */
	int findIndex(const CGAL::Point3& p);

	QList<Primitive*> children;
	QList<CGAL::Point3> points;
	QMap<CGAL::Point3,int> pointMap;
	QList<CGALPolygon*> polygons;
	CGAL::NefPolyhedron3* nefPolyhedron;
	Primitive_t type;
	bool sanitized;

	/* Simple wrapper class to enable Primitive
	 * to be used with CGAL::Nef_nary_union_3 */
	class Unionable
	{
	public:
		Unionable() : primitive(nullptr),force(false) {}
		Unionable(Primitive* p, bool f) { primitive=p; force=f; }
		Unionable& operator+(Unionable&);
		Primitive* primitive;
		bool force;
	};

	CGAL::Nef_nary_union_3<Unionable>* nUnion;
};

#endif // CGALPRIMITIVE_H
#endif
