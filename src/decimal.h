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

#ifndef DECIMAL_H
#define DECIMAL_H

#include <QString>

#if USE_CGAL
#include "cgal.h"
typedef CGAL::Scalar decimal;
#else
typedef double decimal;
#endif

QString to_string(const decimal&);
QString to_string(const decimal&,const bool);
decimal to_decimal(QString s,bool* ok=0);
int to_integer(const decimal&);
bool to_boolean(const decimal&);

#if USE_CGAL
class Point;
void to_glcoord(const Point&,float&,float&,float&);
QString to_rational(const decimal&);
CGAL::Gmpfr to_gmpfr(const decimal&);
#endif

#endif // DECIMAL_H
