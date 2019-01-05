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
#ifndef CGALPROJECTION_H
#define CGALPROJECTION_H

#include "cgal.h"

typedef CGAL::Point2 (*ProjectFunc)(const CGAL::Point3&);

class CGALProjection
{
public:
	explicit CGALProjection(const CGAL::Vector3&);
	CGAL::Point2 project(const CGAL::Point3&) const;
	CGAL::Direction3 getDirection(const CGAL::Vector3&) const;
	bool operator==(const CGALProjection&) const;
private:
	ProjectFunc projectFunc;
	int ortho;
};

#endif // CGALPROJECTION_H
#endif
