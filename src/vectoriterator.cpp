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

#include "vectoriterator.h"

VectorIterator::VectorIterator(QList<Value*> list)
{
	items=list;
}

void VectorIterator::first()
{
	index=0;
}

void VectorIterator::next()
{
	index++;
}

bool VectorIterator::isDone()
{
	return index>=items.size();
}

Value* VectorIterator::currentItem() const
{
	return items.at(index);
}
