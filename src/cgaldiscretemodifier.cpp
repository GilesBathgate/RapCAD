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

#include "cgaldiscretemodifier.h"
#include "rmath.h"

CGALDiscreteModifier::CGALDiscreteModifier(int p) : places(p)
{
}

CGAL::Point3 CGALDiscreteModifier::discretePoint(const CGAL::Point3& pt, int places)
{
	CGAL::Scalar x=r_round(pt.x(),places);
	CGAL::Scalar y=r_round(pt.y(),places);
	CGAL::Scalar z=r_round(pt.z(),places);
	return CGAL::Point3(x,y,z);
}

void CGALDiscreteModifier::operator()(CGAL::NefPolyhedron3::SNC_structure& snc)
{
	CGAL::NefPolyhedron3::Vertex_iterator v;
	CGAL_forall_vertices(v,snc) {
		v->point()=discretePoint(v->point(),places);
	}
}

#endif
