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

#include "rangevalue.h"
#include "rangeiterator.h"
#include "vectorvalue.h"
#include "booleanvalue.h"

RangeValue::RangeValue(Value* start,Value* step, Value* finish)
{
	this->start=start;
	this->step=step;
	this->finish=finish;
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

Value* RangeValue::operation(Expression::Operator_e op)
{
	if(op==Expression::Invert) {
		return new RangeValue(this->finish,this->step,this->start);
	} else if(op==Expression::Length) {
		Value* size=Value::operation(this->finish,Expression::Subtract,this->start);
		size=Value::operation(size,op);
		return Value::operation(size,Expression::Increment);
	}

	Value* upper=Value::operation(this->start,op);
	Value* lower=Value::operation(this->finish,op);

	Value* increment=NULL;
	if(op==Expression::Add||op==Expression::Subtract) {
		if(this->step)
			increment=Value::operation(this->step,op);
	}

	return new RangeValue(upper,increment,lower);
}

Value* RangeValue::operation(Value& v, Expression::Operator_e op)
{
	RangeValue* range=dynamic_cast<RangeValue*>(&v);
	if(range) {
		/* Interval arithmetic has the following rules:
		 * when x = [a:b] and y = [c:d] then
		 * x+y = [a+c:b+d]
		 * x-y = [a-d:b-c]
		 * x*y = [min(a*c,a*d,b*c,b*d):max(a*c,a*d,b*c,b*d)]
		 * x/y = [min(a/c,a/d,b/c,b/d):max(a/c,a/d,b/c,b/d)]
		 */
		Value* a=this->start;
		Value* b=this->finish;
		Value* c=range->start;
		Value* d=range->finish;
		if(op==Expression::Equal) {
			bool result=compare(a,op,c)&&compare(b,op,d);
			return new BooleanValue(result);
		} else if(op==Expression::NotEqual) {
			bool result=compare(a,op,c)||compare(b,op,d);
			return new BooleanValue(result);
		} else if(op==Expression::Add||op==Expression::Subtract) {

			Value* lower=Value::operation(a,op,c);
			Value* upper=Value::operation(b,op,d);

			return new RangeValue(lower,NULL,upper);

		} else if(op==Expression::Multiply||op==Expression::Divide) {

			Value* ac=Value::operation(a,op,c);
			Value* ad=Value::operation(a,op,d);
			Value* bc=Value::operation(b,op,c);
			Value* bd=Value::operation(b,op,d);
			QList<Value*> vals;
			vals.append(ac);
			vals.append(ad);
			vals.append(bc);
			vals.append(bd);
			Value* lower=compareAll(vals,Expression::LessThan);
			Value* upper=compareAll(vals,Expression::GreaterThan);

			return new RangeValue(lower,NULL,upper);

		} else if(op==Expression::Concatenate) {

			QList<Value*> vals;
			vals.append(this->start);
			vals.append(this->finish);
			vals.append(range->start);
			vals.append(range->finish);
			Value* lower=compareAll(vals,Expression::LessThan);
			Value* upper=compareAll(vals,Expression::GreaterThan);

			return new RangeValue(lower,NULL,upper);
		}
	}
	return VectorValue::operation(v,op);
}
