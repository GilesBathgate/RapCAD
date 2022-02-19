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
#ifndef CGALGROUPMODIFIER_H
#define CGALGROUPMODIFIER_H

#include "cgal.h"
#include <CGAL/Modifier_base.h>
#include <CGAL/Nef_polyhedron_3.h>

namespace CGAL
{
	using NefPolyhedron3 = Nef_polyhedron_3<Kernel3>;
}

class CGALGroupModifier : public CGAL::Modifier_base<CGAL::NefPolyhedron3::SNC_structure>
{
public:
	CGALGroupModifier(const CGAL::NefPolyhedron3&);
	void operator()(CGAL::NefPolyhedron3::SNC_structure&);
private:
	const CGAL::NefPolyhedron3& primitive;
};

#endif // CGALGROUPMODIFIER_H
#endif
