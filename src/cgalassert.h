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
#ifndef CGALASSERT_H
#define CGALASSERT_H

#include <CGAL/assertions.h>

#if !defined(CGAL_NO_ASSERTIONS)

#include <CGAL/Random_access_adaptor.h>
#include <CGAL/HalfedgeDS_decorator.h>

/* Redefine CGAL_assertion so that it checks for an
 * uncaught exception before throwing an exception */
#undef CGAL_assertion
#define CGAL_assertion(EX) \
   (CGAL::possibly(EX)||std::uncaught_exception()?(static_cast<void>(0)): ::CGAL::assertion_fail( # EX , __FILE__, __LINE__))

#include <CGAL/Polyhedron_incremental_builder_3.h>

/* Restore CGAL_assertion to its original behaviour
 * as issue is only known in polyhedron incremental
 * builder. */
#undef CGAL_assertion
#define CGAL_assertion(EX) \
   (CGAL::possibly(EX)?(static_cast<void>(0)): ::CGAL::assertion_fail( # EX , __FILE__, __LINE__))

#endif //CGAL_NO_ASSERTIONS

#endif // CGALASSERT_H
#endif
