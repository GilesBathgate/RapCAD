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

#include "cgalvolume.h"

CGALVolume::CGALVolume(CGAL::Cuboid3 b,CGAL::FT s)
{
	bounds=b;
	size=s;
}

CGAL::FT CGALVolume::getSize() const
{
	return size;
}

CGAL::Point3 CGALVolume::getCenter() const
{
	CGAL::FT cx=0.0,cy=0.0,cz=0.0;
	cx=(bounds.xmin()+bounds.xmax())/2;
	cy=(bounds.ymin()+bounds.ymax())/2;
	cz=(bounds.zmin()+bounds.zmax())/2;

	return CGAL::Point3(cx,cy,cz);
}

CGAL::Cuboid3 CGALVolume::getBounds() const
{
	return bounds;
}
