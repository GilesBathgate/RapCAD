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

#include "rangeiterator.h"

RangeIterator::RangeIterator(RangeValue* rng)
{
	range=rng;

	Value& s=*range->getStart();
	Value& f=*range->getFinish();

	Value* v=s>f;
	reverse=v->isTrue();

	step=range->getStep();
	if(!step) {
		decimal i=reverse?-1.0:1.0;
		defaultStep=new NumberValue(i);
		step=defaultStep;
	} else {
		defaultStep=NULL;
	}
}

RangeIterator::~RangeIterator()
{
	delete defaultStep;
}

void RangeIterator::first()
{
	index=range->getStart();
	done=false;
}

void RangeIterator::next()
{
	Value& i=*index;
	Value& s=*step;
	Value* r=i+s;
	Value* c=*r==i;
	if(c->isTrue())
		done=true;
	index=r;
}

bool RangeIterator::isDone()
{
	if(done)
		return true;

	Value& s=*range->getStart();
	Value& f=*range->getFinish();
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

	return lower->isTrue() || upper->isTrue();
}

Value* RangeIterator::currentItem() const
{
	return index;
}
