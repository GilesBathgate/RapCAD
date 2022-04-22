/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2022 Giles Bathgate
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

#include "cgalpolygon.h"
#include "cgalvolume.h"
#include "primitive.h"
#include <CGAL/Nef_nary_union_3.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Polyhedron_3.h>
#include <QMap>
#include <QVector>

namespace CGAL
{
using Polyhedron3 = Polyhedron_3<Kernel3>;
using NefPolyhedron3 = Nef_polyhedron_3<Kernel3>;
using Segment3 = CGAL::Segment_3<CGAL::Kernel3>;
}

class CGALPrimitive : public Primitive
{
public:
	CGALPrimitive();
	~CGALPrimitive() override;
	explicit CGALPrimitive(const CGAL::Polyhedron3&);
	explicit CGALPrimitive(const CGAL::NefPolyhedron3&);
	bool getSanitized() override;
	bool isEmpty() override;
	bool isFullyDimentional() override;
	bool overlaps(Primitive*) override;
	CGALPolygon& createPolygon() override;
	const QList<CGAL::Point3> getPoints() const override;
	const QList<Primitive*> getChildren() const override;
	Primitive* boundary() override;
	Primitive* chain_hull(Primitive*,Primitive*) override;
	Primitive* combine() override;
	Primitive* complement() override;
	Primitive* copy() override;
	Primitive* decompose() override;
	Primitive* difference(Primitive*) override;
	Primitive* glide(Primitive*) override;
	Primitive* group(Primitive*) override;
	Primitive* hull(bool) override;
	Primitive* hull(QList<CGAL::Point3>) override;
	Primitive* inset(const CGAL::Scalar&) override;
	Primitive* intersection(Primitive*) override;
	Primitive* join(Primitive*) override;
	Primitive* linear_extrude(const CGAL::Scalar&,const CGAL::Point3&) override;
	Primitive* minkowski(Primitive*) override;
	Primitive* projection(bool) override;
	Primitive* rotate_extrude(const CGAL::Scalar&,const CGAL::Scalar&,const CGAL::Scalar&,const Fragment*,const CGAL::Point3&) override;
	Primitive* simplify(const CGAL::Scalar&) override;
	Primitive* slice(const CGAL::Scalar&,const CGAL::Scalar&) override;
	Primitive* subdivide(int) override;
	Primitive* symmetric_difference(Primitive*) override;
	Primitive* triangulate() override;
	PrimitiveTypes getType() override;
	/* Don't call this method instead use getCGALPolygons */
	Q_DECL_DEPRECATED const QList<Polygon*> getPolygons() const override;
	void align(bool,QList<ViewDirections>) override;
	void appendChild(Primitive*) override;
	void appendChildren(QList<Primitive*>) override;
	void createVertex(const CGAL::Point3&) override;
	void discrete(int) override;
	void groupLater(Primitive*) override;
	void joinLater(Primitive*) override;
	void resize(bool,const CGAL::Point3&) override;
	void setSanitized(bool) override;
	void setType(PrimitiveTypes) override;
	void transform(TransformMatrix*) override;
	CGAL::Circle3 getRadius();
	CGAL::Cuboid3 getBounds();
	CGALPolygon& createPerimeter();
	CGAL::Polyhedron3* getPolyhedron();
	CGALVolume getVolume(bool);
	const CGAL::NefPolyhedron3& getNefPolyhedron();
	const QList<CGALPolygon*> getCGALPerimeter() const;
	const QList<CGALPolygon*> getCGALPolygons() const;
	void appendVertex(CGALPolygon*,const CGAL::Point3&,bool);
	void appendVertex(const CGAL::Point3&);
	void clearPolygons();
	void createVertex(const CGAL::Scalar&,const CGAL::Scalar&,const CGAL::Scalar&);
	void detectPerimeterHoles();
private:
	bool overlaps(Primitive*,Primitive*) const;
	Primitive* groupAppend(Primitive*);
	Primitive* groupAll(const QList<Primitive*>&) const;
	Primitive* joinAll(const QList<Primitive*>&) const;
	void buildPrimitive();
	void convertBoundary();
	CGAL::NefPolyhedron3* createVolume();
	CGAL::NefPolyhedron3* createPolyline();
	static CGAL::NefPolyhedron3* createPolyline(CGALPolygon*);
	static CGAL::NefPolyhedron3* createPolyline(const CGAL::Segment3&);
	static CGAL::NefPolyhedron3* createPolyline(QVector<CGAL::Point3>);
	CGAL::NefPolyhedron3* createPoints();
	bool detectHoles(QList<CGALPolygon*>,bool);
	bool hasHoles();

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
	QList<CGALPolygon*> perimeters;
	CGAL::NefPolyhedron3* nefPolyhedron;
	PrimitiveTypes type;
	bool sanitized;
	QList<Primitive*> joinable;
	QList<Primitive*> groupable;
};

#endif // CGALPRIMITIVE_H
#endif
