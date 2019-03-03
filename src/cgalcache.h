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
#ifndef CGALCACHE_H
#define CGALCACHE_H

#include "cache.h"
#include "cgalprimitive.h"

class CGALCache : public Cache
{
public:
	CGALCache();
private:
	i_Point hashPoint(const CGAL::Point3&);
	i_Primitive hashPrimitive(Primitive*) override;
	i_Primitive hashPrimitive(CGALPrimitive*);
};

#endif // CGALCACHE_H
#endif
