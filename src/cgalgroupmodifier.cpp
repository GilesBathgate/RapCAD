/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2022 Giles Bathgate
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
#include "cgalgroupmodifier.h"
#include <CGAL/Nef_3/shell_to_nef_3.h>

CGALGroupModifier::CGALGroupModifier(const CGAL::NefPolyhedron3& p) : primitive(p)
{
}

void CGALGroupModifier::operator()(CGAL::NefPolyhedron3::SNC_structure& snc)
{
	CGAL::NefPolyhedron3::Shell_entry_const_iterator si;
	CGAL_forall_shells_of(si,primitive.volumes_begin())
		CGAL::shell_to_nef_3(primitive,si,snc);
}
#endif
