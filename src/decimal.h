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
#include <math.h>

typedef double decimal;

QString to_string(const decimal);
QString to_string(const decimal,const int);
QString to_string(const decimal,const int,const bool);

#if USE_CGAL

#include <CGAL/number_utils.h>
#include <CGAL/gmp.h>

template<class NT>
inline QString to_rational(NT const& n)
{
	char* r=mpq_get_str(NULL, 10, n.exact().mpq());
	QString s(r);
	free(r);
	return s.append('r');
}

template<class NT>
inline decimal to_decimal(NT n)
{
	return to_double(n);
}

template<class NT>
inline decimal inexact_sqrt(NT const& n)
{
	return sqrt(to_decimal(n));
}

template<class NT>
inline QString to_string(NT const& n)
{
	return to_string(to_decimal(n));
}

#endif

inline decimal to_decimal(QString s,bool* ok)
{
	return s.toDouble(ok);
}

#endif // DECIMAL_H
