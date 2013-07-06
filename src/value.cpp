/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2013 Giles Bathgate
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
#include "valueiterator.h"
#include "vectorvalue.h"
#include "booleanvalue.h"

Value::Value()
{
	this->storageClass=Variable::Const;
	this->defined=false;
	values.append(this);
}

Value::~Value()
{
	values.removeAll(this);
}

void Value::cleanup()
{
	foreach(Value* val, values) {
		delete val;
	}
}

QList<Value*> Value::values;

void Value::setStorageClass(Variable::StorageClass_e c)
{
	this->storageClass=c;
}

Variable::StorageClass_e Value::getStorageClass() const
{
	return storageClass;
}

void Value::setName(QString name)
{
	this->name = name;
}

QString Value::getName() const
{
	return this->name;
}

QString Value::getValueString() const
{
	return "undef";
}

bool Value::isTrue() const
{
	return false;
}

VectorValue* Value::toVector(int size)
{
	QList<Value*> children;
	for(int i=0; i<size; i++)
		children.append(this);

	return new VectorValue(children);
}

Iterator<Value*>* Value::createIterator()
{
	return new ValueIterator(this);
}
Value* Value::operator^(Value& v)
{
	return operation(v,Expression::Exponent);
}

Value* Value::operator*(Value& v)
{
	return operation(v,Expression::Multiply);
}

Value* Value::concatenate(Value& v)
{
	return operation(v,Expression::Concatenate);
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

Value* Value::outerProduct(Value& v)
{
	return operation(v,Expression::OuterProduct);
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

Value* Value::operator+=(Value& v)
{
	return operation(v,Expression::AddAssign);
}

Value* Value::operator++(int)
{
	return operation(Expression::Increment);
}

Value* Value::operator-()
{
	return operation(Expression::Subtract);
}

Value* Value::operator-(Value& v)
{
	return operation(v,Expression::Subtract);
}

Value* Value::operator-=(Value& v)
{
	return operation(v,Expression::SubAssign);
}

Value* Value::operator--(int)
{
	return operation(Expression::Decrement);
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

Value* Value::operator[](Value& v)
{
	return operation(v,Expression::Index);
}

Value* Value::operator!()
{
	return operation(Expression::Invert);
}

double Value::modulus(double left, double right)
{
	return fmod(left,right);
}

double Value::exponent(double left, double right)
{
	return pow(left,right);
}

Value* Value::operation(Expression::Operator_e)
{
	return this;
}

Value* Value::operation(Value& v, Expression::Operator_e e)
{
	bool left=this->defined;
	bool right=v.defined;
	if((!left||!right) && isComparison(e)) {
		bool result=basicOperation<bool>(left,e,right);
		return new BooleanValue(result);
	}

	return this;
}

bool Value::isDefined() const
{
	return defined;
}

Value* Value::operation(Value* p_left, Expression::Operator_e e, Value* p_right)
{
	Value& left=*p_left;
	Value& right=*p_right;
	switch(e) {
	case Expression::Exponent:
		return left^right;
	case Expression::Multiply:
		return left*right;
	case Expression::Append:
	case Expression::Concatenate:
		return left.concatenate(right);
	case Expression::ComponentwiseMultiply:
		return left.componentwiseMultiply(right);
	case Expression::Divide:
		return left/right;
	case Expression::ComponentwiseDivide:
		return left.componentwiseDivide(right);
	case Expression::OuterProduct:
		return left.outerProduct(right);
	case Expression::Modulus:
		return left%right;
	case Expression::Add:
		return left+right;
	case Expression::Subtract:
		return left-right;
	case Expression::AddAssign:
		return left+=right;
	case Expression::SubAssign:
		return left-=right;
	case Expression::LessThan:
		return left<right;
	case Expression::LessOrEqual:
		return left<=right;
	case Expression::Equal:
		return left==right;
	case Expression::NotEqual:
		return left!=right;
	case Expression::GreaterOrEqual:
		return left>=right;
	case Expression::GreaterThan:
		return left>right;
	case Expression::LogicalAnd:
		return left&&right;
	case Expression::LogicalOr:
		return left||right;
	case Expression::Index:
		return left[right];
	default:
		return &left;
	}
}

Value* Value::operation(Value* p_left, Expression::Operator_e e)
{
	Value& left=*p_left;
	switch(e) {
	case Expression::Add:
		return +left;
	case Expression::Subtract:
		return -left;
	case Expression::Invert:
		return !left;
	case Expression::Increment:
		return left++;
	case Expression::Decrement:
		return left--;
	default:
		return &left;
	}
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
		//The following are not really comparisons but
		//we expect them to return a boolean result
	case Expression::LogicalAnd:
	case Expression::LogicalOr:
		return true;
	default:
		return false;
	}
}
