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

#ifndef ITERATOR_H
#define ITERATOR_H

template <typename T>
struct Iterator
{
	Iterator& p;
protected:
	Iterator() : p(*this) {}
public:
	virtual ~Iterator()=default;
	virtual bool operator!=(const Iterator& o) const { return p.operator!=(o); }
	virtual Iterator& operator++() { p.operator++(); return *this; }
	virtual T& operator*() const { return p.operator*(); }
	Iterator& begin() { return *this; }
	Iterator& end() { return *this; }
};

#endif // ITERATOR_H
