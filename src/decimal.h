/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2020 Giles Bathgate
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

#ifdef USE_CGAL
#include "cgal.h"
using decimal = CGAL::Scalar;
using Point = CGAL::Point3;
#else
using decimal = double;
#endif

enum class NumberFormats {
	Decimal,
	Scientific,
	Rational
};

enum class Rounding {
	Decimal,
	Base2,
	None
};

enum class Precision {
	Single,
	Double,
	Custom
};

QString to_string(const decimal&);
decimal to_decimal(const QString&,bool* ok=nullptr);
int to_integer(const decimal&);
bool to_boolean(const decimal&);
decimal parse_rational(const QString&,bool* ok=nullptr);
decimal parse_numberexp(const QString&,bool* ok=nullptr);

#ifdef USE_CGAL
void to_glcoord(const Point&,float&,float&,float&);
CGAL::Gmpfr to_gmpfr(const decimal&);
#endif

#endif // DECIMAL_H
