/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010  Giles Bathgate
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

NumberValue::NumberValue(double value)
{
	this->number=value;
}

QString NumberValue::getValueString()
{
	return QString().setNum(this->number,'g',16);
}

Value* NumberValue::operator*(Value& v)
{
	return operation(v,Expression::Multiply);
}

Value* NumberValue::operator/(Value& v)
{
	return operation(v,Expression::Divide);
}

Value* NumberValue::operator%(Value& v)
{
	return operation(v,Expression::Modulus);
}

Value* NumberValue::operator+()
{
	return operation(Expression::Add);
}

Value* NumberValue::operator+(Value& v)
{
	return operation(v,Expression::Add);
}

Value* NumberValue::operator-()
{
	return operation(Expression::Subtract);
}

Value* NumberValue::operator-(Value& v)
{
	return operation(v,Expression::Subtract);
}

Value* NumberValue::operator<(Value& v)
{
	return operation(v,Expression::LessThan);
}

Value* NumberValue::operator<=(Value& v)
{
	return operation(v,Expression::LessOrEqual);
}

Value* NumberValue::operator==(Value& v)
{
	return operation(v,Expression::Equal);
}

Value* NumberValue::operator!=(Value& v)
{
	return operation(v,Expression::NotEqual);
}

Value* NumberValue::operator>=(Value& v)
{
	return operation(v,Expression::GreaterOrEqual);
}

Value* NumberValue::operator>(Value& v)
{
	return operation(v,Expression::GreaterThan);
}

Value* NumberValue::operator!()
{
	return operation(Expression::Invert);
}

Value* NumberValue::operation(Expression::Operator_e e)
{
	double result = Value::basicOperation<double,double>(this->number,e);
	return new NumberValue(result);
}

Value* NumberValue::operation(Value& v, Expression::Operator_e e)
{
	NumberValue* num = dynamic_cast<NumberValue*>(&v);
	if(num) {
		if(isComparison(e)) {
			bool result=basicOperation<bool,double>(this->number,e,num->number);
			return new BooleanValue(result);
		} else {
			double result=basicOperation<double,double>(this->number,e,num->number);
			return new NumberValue(result);
		}
	}
	VectorValue* vec = dynamic_cast<VectorValue*>(&v);
	if(vec) {
		//operations between scalars and vectors are commutative e.g.
		// [1,2,3]-1  is the same as 1 - [1,2,3]
		return Value::operation(vec,e,this);
	}
}
