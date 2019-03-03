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
#include "cgalvolume.h"

CGALVolume::CGALVolume(CGAL::Cuboid3 b,CGAL::Scalar s,CGAL::Point3 c) :
	bounds(b),
	size(s),
	centroid(c)
{
}

CGAL::Scalar CGALVolume::getSize() const
{
	return size;
}

CGAL::Point3 CGALVolume::getCenter() const
{
	return centroid;
}

CGAL::Cuboid3 CGALVolume::getBounds() const
{
	return bounds;
}
#endif
