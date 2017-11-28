/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2017 Giles Bathgate
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

RangeIterator::RangeIterator(RangeValue* rng, Value* s, Value* st, Value* f,bool r) : ValueIterator(rng)
{
	start=s;
	step=st;
	finish=f;
	index=s;
	reverse=r;
}

RangeIterator::~RangeIterator()
{
}

ValueIterator& RangeIterator::operator++()
{
	Value& i=*index;
	Value& s=*step;
	Value* r=i+s;
	Value* c=*r==i;
	if(c->isTrue())
		done=true;
	index=r;
	return *this;
}

bool RangeIterator::operator!=(const Iterator&) const
{
	if(done)
		return false;

	Value& s=*start;
	Value& f=*finish;
	Value& i=*index;

	Value* lower;
	Value* upper;
	if(reverse) {
		lower=i<f;
		upper=i>s;
	} else {
		lower=i<s;
		upper=i>f;
	}

	return !lower->isTrue() && !upper->isTrue();
}

Value* RangeIterator::operator*() const
{
	return index;
}
