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
#ifndef CGAL_H
#define CGAL_H

#include "config.h"

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

namespace CGAL
{
typedef Exact_predicates_exact_constructions_kernel Kernel3;
typedef Kernel3::FT Scalar;
typedef Kernel3::Iso_cuboid_3 Cuboid3;
typedef Kernel3::Point_3 Point3;
typedef Kernel3::Triangle_3 Triangle3;
typedef Kernel3::Point_2 Point2;
typedef Kernel3::Vector_3 Vector3;
typedef Kernel3::Direction_3 Direction3;
typedef Kernel3::Plane_3 Plane3;
typedef Kernel3::Circle_3 Circle3;
typedef Kernel3::Aff_transformation_3 AffTransformation3;
}

#endif // CGAL_H
#endif
