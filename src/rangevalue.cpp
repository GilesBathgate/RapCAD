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

#include "rangevalue.h"
#include "rangeiterator.h"
#include "vectorvalue.h"

RangeValue::RangeValue(Value* start,Value* step, Value* finish)
{
	this->start=start;
	this->step=step;
	this->finish=finish;
	this->defined=true;
}

QString RangeValue::getValueString() const
{
	QString result="[";
	result.append(this->start->getValueString());
	result.append(":");
	if(this->step) {
		result.append(this->step->getValueString());
		result.append(":");
	}
	result.append(this->finish->getValueString());
	result.append("]");
	return result;
}

Iterator<Value*>* RangeValue::createIterator()
{
	return new RangeIterator(this);
}

QList<Value*> RangeValue::getChildren()
{
	QList<Value*> result;
	Iterator<Value*>* i=this->createIterator();
	for(i->first(); !i->isDone(); i->next())
		result.append(i->currentItem());

	return result;
}

Value* RangeValue::getStart() const
{
	return this->start;
}

Value* RangeValue::getStep() const
{
	return this->step;
}

Value* RangeValue::getFinish() const
{
	return this->finish;
}

Value* RangeValue::operation(Value& v,Expression::Operator_e e)
{
	VectorValue* vec=dynamic_cast<VectorValue*>(&v);
	if(vec) {
		if(e==Expression::Concatenate) {
			QList<Value*> result;
			foreach(Value* v, this->getChildren())
				result.append(v);
			foreach(Value* v, vec->getChildren())
				result.append(v);
			return new VectorValue(result);
		}
	}

	return Value::operation(v,e);
}
