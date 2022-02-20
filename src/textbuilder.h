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

#ifndef TEXTBUILDER_H
#define TEXTBUILDER_H

#include "primitive.h"
#include <QString>

class TextBuilder
{
public:
	TextBuilder() {}
	virtual ~TextBuilder() {}
	virtual void setText(const QString&)=0;
	virtual decimal getHeight()=0;
	virtual void setLocation(const Point&)=0;
	virtual Primitive* buildPrimitive() const=0;
};

#endif // TEXTBUILDER_H
