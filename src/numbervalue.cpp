/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2021 Giles Bathgate
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

#include "numbervalue.h"
#include "vectorvalue.h"
#include "booleanvalue.h"
#include "valuefactory.h"

NumberValue::NumberValue(const decimal& value) : number(value)
{
}

QString NumberValue::getValueString() const
{
	return to_string(number);
}

bool NumberValue::isTrue() const
{
	return to_boolean(number);
}

decimal NumberValue::getNumber() const
{
	return number;
}

Value& NumberValue::toNumber()
{
	return *this;
}

int NumberValue::toInteger() const
{
	return to_integer(number);
}

Value& NumberValue::operation(Operators e)
{
	if(e==Operators::Invert)
		return factory.createBoolean(this->isFalse());

	decimal result=basicOperation(number,e);
	return factory.createNumber(result);
}

Value& NumberValue::operation(Value& v, Operators e)
{
	auto* num=dynamic_cast<NumberValue*>(&v);
	if(num)
		return operation(*num,e);

	auto* vec=dynamic_cast<VectorValue*>(&v);
	if(vec)
		return operation(*vec,e);

	auto* flag=dynamic_cast<BooleanValue*>(&v);
	if(flag)
		return operation(*flag,e);

	return Value::operation(v,e);
}

Value& NumberValue::operation(NumberValue& num,Operators e)
{
	if(isComparison(e)) {
		bool result=to_boolean(basicOperation(number,e,num.number));
		return factory.createBoolean(result);
	}
	if(e==Operators::Divide||e==Operators::Modulus) {
		if(num.number==0.0)
			return factory.createUndefined();
	}
	if(e==Operators::Exponent) {
		if(number==0.0&&num.number<=0.0)
			return factory.createUndefined();
	}
	if(e==Operators::CrossProduct) {
		return factory.createUndefined();
	}

	decimal result=basicOperation(number,e,num.number);
	return factory.createNumber(result);
}

Value& NumberValue::operation(VectorValue& vec,Operators e)
{
	if(e==Operators::Concatenate) {
		QList<Value*> r=vec.getElements();
		r.prepend(this);
		return factory.createVector(r);
	}
	if(e==Operators::Exponent) {
		QList<Value*> result;
		for(Value* c: vec.getElements())
			result.append(Value::operation(this,e,c));

		return factory.createVector(result);
	}
	if(e==Operators::CrossProduct) {
		//Q: "What do you get when you cross a mountain-climber with a mosquito?"
		//A: "Nothing: you can't cross a scaler with a vector"
		return factory.createUndefined();
	}

	// most operations between scalars and vectors are commutative e.g.
	// [1,2,3]-1  is the same as 1-[1,2,3]
	return *Value::operation(&vec,e,this);
}

Value& NumberValue::operation(BooleanValue& flag,Operators e)
{
	if(isComparison(e)) {
		//Use 0 for false and 1 for true to ensure 2>true
		bool result=basicOperation(this->toInteger(),e,flag.isTrue()?1:0);
		return factory.createBoolean(result);
	}

	return Value::operation(flag,e);
}
