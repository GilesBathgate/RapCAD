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

#ifndef DECIMAL_H
#define DECIMAL_H

#ifdef USE_CGAL
#include "cgal.h"
#endif
#include <QString>

#ifdef USE_CGAL
using decimal = CGAL::Scalar;
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
decimal get_unit(const QString&,QString&);

#ifdef USE_CGAL
mpq_srcptr to_mpq(const decimal&);
void to_mpfr(mpfr_t&,const decimal&);
decimal to_decimal(mpfr_t&);
decimal to_decimal(mpq_t&);
#endif

#endif // DECIMAL_H
