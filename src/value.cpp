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

#include "value.h"
#include "math.h"

Value::Value()
{
}

void Value::setName(QString name)
{
	this->name = name;
}

QString Value::getName()
{
	return this->name;
}

QString Value::getValueString()
{
	return "undef";
}


Value* Value::operator*(Value& v)
{
	return operation(v,Expression::Multiply);
}

Value* Value::componentwiseMultiply(Value& v)
{
	return operation(v,Expression::ComponentwiseMultiply);
}

Value* Value::operator/(Value& v)
{
	return operation(v,Expression::Divide);
}

Value* Value::componentwiseDivide(Value& v)
{
	return operation(v,Expression::ComponentwiseDivide);
}

Value* Value::operator%(Value& v)
{
	return operation(v,Expression::Modulus);
}

Value* Value::operator+()
{
	return operation(Expression::Add);
}

Value* Value::operator+(Value& v)
{
	return operation(v,Expression::Add);
}

Value* Value::operator-()
{
	return operation(Expression::Subtract);
}

Value* Value::operator-(Value& v)
{
	return operation(v,Expression::Subtract);
}

Value* Value::operator<(Value& v)
{
	return operation(v,Expression::LessThan);
}

Value* Value::operator<=(Value& v)
{
	return operation(v,Expression::LessOrEqual);
}

Value* Value::operator==(Value& v)
{
	return operation(v,Expression::Equal);
}

Value* Value::operator!=(Value& v)
{
	return operation(v,Expression::NotEqual);
}

Value* Value::operator>=(Value& v)
{
	return operation(v,Expression::GreaterOrEqual);
}

Value* Value::operator>(Value& v)
{
	return operation(v,Expression::GreaterThan);
}

Value* Value::operator&&(Value& v)
{
	return operation(v,Expression::LogicalAnd);
}

Value* Value::operator||(Value& v)
{
	return operation(v,Expression::LogicalOr);
}

Value* Value::operator!()
{
	return operation(Expression::Invert);
}

double Value::modulus(double left, double right)
{
	return fmod(left, right);
}

Value* Value::operation(Expression::Operator_e)
{
	return this;
}

Value* Value::operation(Value&, Expression::Operator_e)
{
	return this;
}

Value* Value::operation(Value* p_left, Expression::Operator_e e, Value* p_right)
{
	Value& left=*p_left;
	Value& right=*p_right;
	Value* result;
	switch(e) {
	case Expression::Multiply:
		result=left*right;
		break;
	case Expression::ComponentwiseMultiply:
		result=p_left->componentwiseMultiply(right);
		break;
	case Expression::Divide:
		result=left/right;
		break;
	case Expression::ComponentwiseDivide:
		result=p_left->componentwiseDivide(right);
		break;
	case Expression::Modulus:
		result=left%right;
		break;
	case Expression::Add:
		result=left+right;
		break;
	case Expression::Subtract:
		result=left-right;
		break;
	case Expression::LessThan:
		result=left<right;
		break;
	case Expression::LessOrEqual:
		result=left<=right;
		break;
	case Expression::Equal:
		result=left==right;
		break;
	case Expression::NotEqual:
		result=left!=right;
		break;
	case Expression::GreaterOrEqual:
		result=left>=right;
		break;
	case Expression::GreaterThan:
		result=left>right;
		break;
	case Expression::LogicalAnd:
		result=left&&right;
		break;
	case Expression::LogicalOr:
		result=left||right;
		break;
	default:
		result=&left;
	}
	return result;
}

Value* Value::operation(Value* p_left, Expression::Operator_e e)
{
	Value& left=*p_left;
	Value* result;
	switch(e) {
	case Expression::Add:
		result=+left;
		break;
	case Expression::Subtract:
		result=-left;
		break;
	case Expression::Invert:
		result=!left;
		break;
	default:
		result=&left;
	}
	return result;
}

bool Value::isComparison(Expression::Operator_e e)
{
	switch(e) {
	case Expression::LessThan:
	case Expression::LessOrEqual:
	case Expression::Equal:
	case Expression::NotEqual:
	case Expression::GreaterOrEqual:
	case Expression::GreaterThan:
		return true;
	default:
		return false;
	}
}
