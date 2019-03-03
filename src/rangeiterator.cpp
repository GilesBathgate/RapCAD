/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2019 Giles Bathgate
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

RangeIterator::RangeIterator(RangeValue* rng, Value* i, Value* s) :
	ValueIterator(rng),
	index(i),
	step(s)
{
}

RangeIterator::~RangeIterator()
{
}

ValueIterator& RangeIterator::operator++()
{
	index=Value::operation(index,Expression::Add,step);
	return *this;
}

bool RangeIterator::operator!=(const Iterator&) const
{
	auto* range=static_cast<RangeValue*>(value);
	return range->inRange(index);
}

Value* RangeIterator::operator*() const
{
	return index;
}
