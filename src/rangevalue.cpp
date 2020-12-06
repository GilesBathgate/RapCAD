/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2020 Giles Bathgate
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
#include "valuefactory.h"

RangeValue::RangeValue(Value* s,Value* st, Value* f) :
	start(s),
	step(st),
	finish(f)
{
	Value* v=Value::operation(start,Operators::GreaterThan,finish);
	reverse = v->isTrue();

	if(!step) {
		decimal i=reverse?-1.0:1.0;
		step=factory.createNumber(i);
	}
}

QString RangeValue::getValueString() const
{
	QString result="[";
	result.append(start->getValueString());
	result.append(":");
	if(step) {
		result.append(step->getValueString());
		result.append(":");
	}
	result.append(finish->getValueString());
	result.append("]");
	return result;
}

bool RangeValue::inRange(Value* index)
{
	//Check for a valid range
	if(start->isUndefined()||finish->isUndefined()||step->isFalse())
		return false;

	Value*	lower = Value::operation(index,Operators::LessThan,reverse?finish:start);
	Value*	upper = Value::operation(index,Operators::GreaterThan,reverse?start:finish);

	return lower->isFalse() && upper->isFalse();
}

Value* RangeValue::getIndex(NumberValue* n)
{
	Value* x=reverse?Value::operation(step,Operators::Subtract):step;
	Value* a=Value::operation(n,Operators::Multiply,x);
	Value* b=Value::operation(start,reverse?Operators::Subtract:Operators::Add,a);

	if(!inRange(b)) return factory.createUndefined();
	return b;
}

ValueIterator* RangeValue::createIterator()
{
	return new RangeIterator(this,start,step);
}

QList<Value*> RangeValue::getElements()
{
	QList<Value*> result;
	ValueIterator* it=createIterator();
	for(Value* v: *it)
		result.append(v);
	delete it;
	return result;
}

Value* RangeValue::getStart() const
{
	return start;
}

Value* RangeValue::getFinish() const
{
	return finish;
}

Value* RangeValue::operation(Operators op)
{
	if(op==Operators::Invert) {
		return factory.createRange(finish,step,start);
	}
	if(op==Operators::Length) {
		Value* size=Value::operation(finish,Operators::Subtract,start);
		size=Value::operation(size,op);
		return Value::operation(size,Operators::Increment);
	}

	Value* upper=Value::operation(start,op);
	Value* lower=Value::operation(finish,op);

	Value* increment=nullptr;
	if(op==Operators::Add||op==Operators::Subtract) {
		if(step)
			increment=Value::operation(step,op);
	}

	return factory.createRange(upper,increment,lower);
}

Value* RangeValue::operation(Value& v, Operators op)
{
	auto* range=dynamic_cast<RangeValue*>(&v);
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
		if(op==Operators::Equal) {
			bool result=compare(a,op,c)&&compare(b,op,d);
			return factory.createBoolean(result);
		}
		if(op==Operators::NotEqual) {
			bool result=compare(a,op,c)||compare(b,op,d);
			return factory.createBoolean(result);
		}
		if(op==Operators::Add||op==Operators::Subtract) {

			Value* lower=Value::operation(a,op,c);
			Value* upper=Value::operation(b,op,d);

			return factory.createRange(lower,nullptr,upper);

		}
		if(op==Operators::Multiply||op==Operators::Divide) {

			Value* ac=Value::operation(a,op,c);
			Value* ad=Value::operation(a,op,d);
			Value* bc=Value::operation(b,op,c);
			Value* bd=Value::operation(b,op,d);
			QList<Value*> vals;
			vals.append(ac);
			vals.append(ad);
			vals.append(bc);
			vals.append(bd);
			Value* lower=compareAll(vals,Operators::LessThan);
			Value* upper=compareAll(vals,Operators::GreaterThan);

			return factory.createRange(lower,nullptr,upper);
		}
		if(op==Operators::Concatenate) {

			QList<Value*> vals;
			vals.append(start);
			vals.append(finish);
			vals.append(range->start);
			vals.append(range->finish);
			Value* lower=compareAll(vals,Operators::LessThan);
			Value* upper=compareAll(vals,Operators::GreaterThan);

			return factory.createRange(lower,nullptr,upper);
		}
	}
	return VectorValue::operation(v,op);
}
