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

#include "rangevalue.h"
#include "rangeiterator.h"
#include "vectorvalue.h"
#include "booleanvalue.h"

RangeValue::RangeValue(Value* s,Value* st, Value* f) :
	start(s),
	step(st),
	finish(f)
{
	Value* v=Value::operation(start,Expression::GreaterThan,finish);
	reverse = v->isTrue();

	if(!step) {
		decimal i=reverse?-1.0:1.0;
		step=new NumberValue(i);
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

	Value*	lower = Value::operation(index,Expression::LessThan,reverse?finish:start);
	Value*	upper = Value::operation(index,Expression::GreaterThan,reverse?start:finish);

	return lower->isFalse() && upper->isFalse();
}

Value* RangeValue::getIndex(NumberValue* n)
{
	Value* x=reverse?Value::operation(step,Expression::Subtract):step;
	Value* a=Value::operation(n,Expression::Multiply,x);
	Value* b=Value::operation(start,reverse?Expression::Subtract:Expression::Add,a);

	if(!inRange(b)) return undefined();
	return b;
}

ValueIterator* RangeValue::createIterator()
{
	return new RangeIterator(this,start,step);
}

QList<Value*> RangeValue::getChildren()
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

Value* RangeValue::operation(Expression::Operator_e op)
{
	if(op==Expression::Invert) {
		return new RangeValue(finish,step,start);
	} else if(op==Expression::Length) {
		Value* size=Value::operation(finish,Expression::Subtract,start);
		size=Value::operation(size,op);
		return Value::operation(size,Expression::Increment);
	}

	Value* upper=Value::operation(start,op);
	Value* lower=Value::operation(finish,op);

	Value* increment=nullptr;
	if(op==Expression::Add||op==Expression::Subtract) {
		if(step)
			increment=Value::operation(step,op);
	}

	return new RangeValue(upper,increment,lower);
}

Value* RangeValue::operation(Value& v, Expression::Operator_e op)
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
		if(op==Expression::Equal) {
			bool result=compare(a,op,c)&&compare(b,op,d);
			return new BooleanValue(result);
		} else if(op==Expression::NotEqual) {
			bool result=compare(a,op,c)||compare(b,op,d);
			return new BooleanValue(result);
		} else if(op==Expression::Add||op==Expression::Subtract) {

			Value* lower=Value::operation(a,op,c);
			Value* upper=Value::operation(b,op,d);

			return new RangeValue(lower,nullptr,upper);

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

			return new RangeValue(lower,nullptr,upper);

		} else if(op==Expression::Concatenate) {

			QList<Value*> vals;
			vals.append(start);
			vals.append(finish);
			vals.append(range->start);
			vals.append(range->finish);
			Value* lower=compareAll(vals,Expression::LessThan);
			Value* upper=compareAll(vals,Expression::GreaterThan);

			return new RangeValue(lower,nullptr,upper);
		}
	}
	return VectorValue::operation(v,op);
}
