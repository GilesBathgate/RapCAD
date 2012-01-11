/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2012 Giles Bathgate
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

#ifndef CGALBUILDER_H
#define CGALBUILDER_H

#include "cgalprimitive.h"

class CGALBuilder : public CGAL::Modifier_base<CGAL::HalfedgeDS>
{
public:
	CGALBuilder(CGALPrimitive*);
	CGALPrimitive* buildOffsetPolygons(double);
private:
	void operator()(CGAL::HalfedgeDS&);
	CGALPrimitive* primitive;
};

#endif // CGALBUILDER_H
