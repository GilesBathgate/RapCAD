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

#ifndef FRAGMENT_H
#define FRAGMENT_H

#include "decimal.h"
#include "context.h"

class Fragment
{
public:
	virtual ~Fragment() {}
	static Fragment* createFragment(const Context&);
	static int getFragments(const Context&,const decimal&);
	virtual int getFragments(const decimal&);
protected:
	explicit Fragment(const Context&);
	int fragmentNumber;
	decimal fragmentSize;
	decimal fragmentAngle;
	decimal fragmentError;
};

#endif // FRAGMENT_H
