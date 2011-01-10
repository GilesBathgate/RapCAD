/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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

#include "rangeiterator.h"

RangeIterator::RangeIterator(RangeValue* range)
{
	this->range=range;
	this->defaultStep=new NumberValue(1);
}

RangeIterator::~RangeIterator()
{
	delete defaultStep;
}

void RangeIterator::first()
{
	this->index=this->range->getStart();
}

void RangeIterator::next()
{
	Value& i = *this->index;

	Value* step = this->range->getStep();
	if(!step)
		step = defaultStep;

	Value& s = *step;
	this->index=i+s;
}

bool RangeIterator::isDone()
{
	Value& f = *this->range->getFinish();
	Value& i = *this->index;

	Value* v = i>f;

	return v->isTrue();
}

Value* RangeIterator::currentItem() const
{
	return index;
}
