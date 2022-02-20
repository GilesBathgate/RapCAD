/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2021 Giles Bathgate
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
#ifndef CGALVOLUME_H
#define CGALVOLUME_H

#include "cgal.h"
#include <QString>

class CGALVolume
{
public:
	CGALVolume();
	CGALVolume(const CGAL::Cuboid3&,const CGAL::Scalar&,const CGAL::Point3&);
	const CGAL::Point3& getCenter() const;
	const CGAL::Scalar& getSize() const;
	const CGAL::Cuboid3& getBounds() const;
	const QString getSizeString() const;
	const QString getCenterString() const;
private:
	CGAL::Cuboid3 bounds;
	CGAL::Scalar size;
	CGAL::Point3 centroid;
};

#endif // CGALVOLUME_H
#endif
