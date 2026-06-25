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

#ifndef CGALVERTEXINDEXER_H
#define CGALVERTEXINDEXER_H

#include "indexer.h"

#include "cgalprimitive.h"
#include <QHash>

using VertexHandle = CGAL::NefPolyhedron3::Vertex_const_handle;

class CGALVertexIndexer : public Indexer
{
public:
	CGALVertexIndexer(CGALPrimitive&);
	void create(const VertexHandle&);
	void calculateIndex(const VertexHandle&);
private:
	QHash<VertexHandle,qsizetype> map;
	CGALPrimitive& primitive;
};

namespace CGAL {
namespace internal {
	inline size_t qHash(const VertexHandle& key,size_t seed)
	{
		return ::qHash(&*key,seed);
	}
} // namespace internal
} // namespace CGAL

#endif // CGALVERTEXINDEXER_H
