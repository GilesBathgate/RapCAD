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

#include "cgalindexer.h"
#include "cgalprimitive.h"

CGALIndexer::CGALIndexer(CGALPrimitive & p) :
	primitive(p)
{
}

void CGALIndexer::create(const CGAL::Point3& p)
{
	index=primitive.pointsSize();
	pointMap.insert(p,index);
	primitive.createVertex(p);
}

void CGALIndexer::calculateIndex(const CGAL::Point3& p)
{
	/* Using pointMap.find allows to check whether the map contains the value
	 * whilst also providing a way to access it instead of doing two lookups */
	const auto& it=pointMap.constFind(p);
	if(it!=pointMap.constEnd()) {
		index=*it;
		return;
	}

	create(p);
}
