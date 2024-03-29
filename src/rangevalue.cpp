/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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
#include "booleanvalue.h"
#include "rangeiterator.h"
#include "valuefactory.h"
#include "vectorvalue.h"
#include <QScopedPointer>

RangeValue::RangeValue(Value& s,Value& f) :
	start(s),
	finish(f),
	reverse(getReverse()),
	step(defaultStep())
{
}

RangeValue::RangeValue(Value& s,Value& st,Value& f) :
	start(s),
	finish(f),
	reverse(getReverse()),
	step(st)
{
}

QString RangeValue::getValueString() const
{
	QString result="[";
	result.append(start.getValueString());
	result.append(":");
	result.append(step.getValueString());
	result.append(":");
	result.append(finish.getValueString());
	result.append("]");
	return result;
}

bool RangeValue::inRange(Value& index)
{
	//Check for a valid range
	if(start.isUndefined()||finish.isUndefined()||step.isFalse())
		return false;

	const Value& lower = Value::evaluate(index,Operators::LessThan,reverse?finish:start);
	const Value& upper = Value::evaluate(index,Operators::GreaterThan,reverse?start:finish);

	return lower.isFalse() && upper.isFalse();
}

Value& RangeValue::getIndex(NumberValue& n)
{
	Value& x=reverse?Value::evaluate(step,Operators::Subtract):step;
	Value& a=Value::evaluate(n,Operators::Multiply,x);
	Value& b=Value::evaluate(start,reverse?Operators::Subtract:Operators::Add,a);

	if(inRange(b))
		return b;

	return ValueFactory::createUndefined();
}

ValueIterator* RangeValue::createIterator()
{
	return new RangeIterator(this,&start,&step);
}

QList<Value*> RangeValue::getElements()
{
	QList<Value*> result;
	QScopedPointer<ValueIterator> it(createIterator());
	for(Value& v: *it)
		result.append(&v);
	return result;
}

Value& RangeValue::getStart() const
{
	return start;
}

Value& RangeValue::getFinish() const
{
	return finish;
}

Value& RangeValue::operation(Operators op)
{
	if(op==Operators::Invert) {
		return ValueFactory::createRange(finish,step,start);
	}
	if(op==Operators::Length) {
		Value& difference=Value::evaluate(finish,Operators::Subtract,start);
		Value& size=Value::evaluate(difference,op);
		return Value::evaluate(size,Operators::Increment);
	}

	Value& upper=Value::evaluate(start,op);
	Value& lower=Value::evaluate(finish,op);

	if(op==Operators::Add||op==Operators::Subtract) {
		Value& increment=Value::evaluate(step,op);
		return ValueFactory::createRange(upper,increment,lower);
	}

	return ValueFactory::createRange(upper,lower);
}

Value& RangeValue::operation(Value& v,Operators op)
{
	auto* range=dynamic_cast<RangeValue*>(&v);
	if(range)
		return operation(*range,op);

	return VectorValue::operation(v,op);
}

Value& RangeValue::operation(RangeValue& range,Operators op)
{
	Value& a=start;
	Value& b=finish;
	Value& c=range.start;
	Value& d=range.finish;
	if(op==Operators::Equal) {
		const bool result=compare(a,op,c)&&compare(b,op,d);
		return ValueFactory::createBoolean(result);
	}
	if(op==Operators::NotEqual) {
		const bool result=compare(a,op,c)||compare(b,op,d);
		return ValueFactory::createBoolean(result);
	}
	if(op==Operators::Concatenate) {

		const QList<Value*> vals {&a,&b,&c,&d};
		Value& lower=compareAll(vals,Operators::LessThan);
		Value& upper=compareAll(vals,Operators::GreaterThan);

		return ValueFactory::createRange(lower,upper);
	}

	return VectorValue::operation(range,op);
}

bool RangeValue::getReverse()
{
	const Value& v=Value::evaluate(start,Operators::GreaterThan,finish);
	return v.isTrue();
}

Value& RangeValue::defaultStep() const
{
	const decimal& i=reverse?-1.0:1.0;
	return ValueFactory::createNumber(i);
}
