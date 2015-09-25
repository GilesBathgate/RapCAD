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

typedef double decimal;

QString to_string(const decimal);
QString to_string(const decimal,const bool);

#if USE_CGAL

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
inline QString to_string(NT const& n)
{
	return to_string(to_double(n));
}

template<class NT>
inline int to_integer(NT const& n)
{
	return int(to_double(n));
}

template<class NT>
inline bool to_boolean(NT const& n)
{
	return bool(to_integer(n));
}

#endif

inline decimal to_decimal(QString s)
{
	return s.toDouble();
}

inline decimal to_decimal(QString s,bool* ok)
{
	return s.toDouble(ok);
}

#endif // DECIMAL_H
