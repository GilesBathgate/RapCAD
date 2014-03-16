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

template< class T >
inline decimal to_decimal(T n)
{
	return to_double(n);
}

template<class NT>
inline decimal inexact_sqrt(NT const& n)
{
	return sqrt(to_decimal(n));
}

inline decimal to_decimal(QString s,bool* ok)
{
	return s.toDouble(ok);
}

QString to_string(const decimal);

#endif // DECIMAL_H
