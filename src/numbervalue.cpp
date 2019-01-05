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

#include "numbervalue.h"
#include "vectorvalue.h"
#include "booleanvalue.h"

NumberValue::NumberValue(decimal value) : number(value)
{
}

QString NumberValue::getValueString() const
{
	return to_string(number);
}

bool NumberValue::isTrue() const
{
	return number!=0;
}

decimal NumberValue::getNumber() const
{
	return number;
}

Value* NumberValue::toNumber()
{
	return this;
}

int NumberValue::toInteger() const
{
	return to_integer(number);
}

Value* NumberValue::operation(Expression::Operator_e e)
{
	if(e==Expression::Invert)
		return new BooleanValue(this->isFalse());

	decimal result=basicOperation(number,e);
	return new NumberValue(result);
}

Value* NumberValue::operation(Value& v, Expression::Operator_e e)
{
	auto* num = dynamic_cast<NumberValue*>(&v);
	if(num) {
		if(isComparison(e)) {
			bool result=to_boolean(basicOperation(number,e,num->number));
			return new BooleanValue(result);
		}
		if(e==Expression::Divide||e==Expression::Modulus) {
			if(num->number==0)
				return Value::undefined();
		} else if(e==Expression::Exponent) {
			if(number==0&&num->number<=0)
				return Value::undefined();
		}

		decimal result=basicOperation(number,e,num->number);
		return new NumberValue(result);
	}
	auto* vec = dynamic_cast<VectorValue*>(&v);
	if(vec) {
		if(e==Expression::Concatenate) {
			QList<Value*> r=vec->getChildren();
			r.prepend(this);
			return new VectorValue(r);
		} else if(e==Expression::Exponent) {
			QList<Value*> result;
			for(Value* c: vec->getChildren())
				result.append(Value::operation(this,e,c));
			return new VectorValue(result);
		} else {
			// most operations between scalars and vectors are commutative e.g.
			// [1,2,3]-1  is the same as 1-[1,2,3]
			return Value::operation(vec,e,this);
		}
	}
	auto* flag = dynamic_cast<BooleanValue*>(&v);
	if(flag && isComparison(e)) {
		//Use 0 for false and 1 for true to ensure 2>true
		bool result=basicOperation(this->toInteger(),e,flag->isTrue()?1:0);
		return new BooleanValue(result);
	}

	return Value::operation(v,e);
}
