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
#ifndef CGALVOLUME_H
#define CGALVOLUME_H

#include "cgal.h"

class CGALVolume
{
public:
	CGALVolume(CGAL::Cuboid3,CGAL::Scalar,CGAL::Point3);
	CGAL::Point3 getCenter() const;
	CGAL::Scalar getSize() const;
	CGAL::Cuboid3 getBounds() const;
private:
	CGAL::Cuboid3 bounds;
	CGAL::Scalar size;
	CGAL::Point3 centroid;
};

#endif // CGALVOLUME_H
#endif
