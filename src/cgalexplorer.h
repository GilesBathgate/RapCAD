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
#ifdef USE_CGAL
#ifndef CGALEXPLORER_H
#define CGALEXPLORER_H

#include "cgalprimitive.h"
#include <QList>

class CGALExplorer
{
	Q_DISABLE_COPY_MOVE(CGALExplorer)
public:
	explicit CGALExplorer(Primitive*);
	explicit CGALExplorer(CGALPrimitive*);
	~CGALExplorer();
	CGALPrimitive* getPrimitive();
	QList<CGALPolygon*> getBase();
	CGALVolume getVolume(bool);
private:
	bool explore();
	bool evaluate();
	CGALPrimitive* primitive;
	class ShellExplorer* explorer;
};

using HalfEdgeHandle = CGAL::NefPolyhedron3::Halfedge_const_handle;

namespace CGAL {
namespace internal {
	inline size_t qHash(const HalfEdgeHandle& key,size_t seed)
	{
		return ::qHash(&*key,seed);
	}
} // namespace internal
} // namespace CGAL

#endif // CGALEXPLORER_H
#endif
