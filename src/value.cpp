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

#include "value.h"
#include "valueiterator.h"
#include "vectorvalue.h"
#include "booleanvalue.h"
#include "textvalue.h"
#include "numbervalue.h"
#include "rangevalue.h"
#include "rmath.h"

Value::Value() :
	defined(true),
	storageClass(Variable::Const)
{
	values.append(this);
}

Value* Value::undefined()
{
	auto* v=new Value();
	v->defined=false;
	return v;
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

void Value::setStorage(Variable::Storage_e c)
{
	storageClass=c;
}

Variable::Storage_e Value::getStorage() const
{
	return storageClass;
}

QString Value::getValueString() const
{
	return "undef";
}

bool Value::isTrue() const
{
	return false;
}

bool Value::isFalse() const
{
	return !isTrue();
}

VectorValue* Value::toVector(int size)
{
	QList<Value*> children;
	for(auto i=0; i<size; ++i)
		children.append(this);

	return new VectorValue(children);
}

TextValue* Value::toText()
{
	return new TextValue(getValueString());
}

Value* Value::toNumber()
{
	return this;
}

ValueIterator* Value::createIterator()
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

Value* Value::crossProduct(Value& v)
{
	return operation(v,Expression::CrossProduct);
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

Value* Value::length()
{
	return operation(Expression::Length);
}

Value* Value::length(Value& v)
{
	return operation(v,Expression::Length);
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

bool Value::modulus(bool, bool)
{
	return false;
}

decimal Value::modulus(const decimal& left, const decimal& right)
{
	return r_mod(left,right);
}

bool Value::multiply(bool left, bool right)
{
	return left&&right;
}

decimal Value::multiply(const decimal& left, const decimal& right)
{
	return left*right;
}

bool Value::exponent(bool left, bool right)
{
	return left^right;
}

decimal Value::exponent(const decimal& left, const decimal& right)
{
	return r_pow(left,right);
}

bool Value::logic(bool a)
{
	return a;
}

bool Value::logic(const decimal& a)
{
	return to_boolean(a);
}

bool Value::length(bool left)
{
	return left;
}

decimal Value::length(const decimal& left)
{
	return r_abs(left);
}

Value* Value::operation(Expression::Operator_e e)
{
	if(e==Expression::Invert) {
		bool result=basicOperation(defined,e);
		return new BooleanValue(result);
	}

	return this;
}

Value* Value::operation(Value& v, Expression::Operator_e e)
{
	bool left=defined;
	bool right=v.defined;
	if((!left||!right) && isComparison(e)) {
		bool result=basicOperation(left,e,right);
		return new BooleanValue(result);
	}
	if(e==Expression::Concatenate) {
		return &v;
	}

	return Value::undefined();
}

bool Value::isDefined() const
{
	return defined;
}

bool Value::isUndefined() const
{
	return !defined;
}

Value* Value::operation(Value* p_left, Expression::Operator_e e, Value* p_right)
{
	Value& left=*p_left;
	Value& right=*p_right;
	switch(e) {
		case Expression::Exponent:
			return left^right;
		case Expression::DotProduct:
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
		case Expression::CrossProduct:
			return left.crossProduct(right);
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
		case Expression::Length:
			return left.length(right);
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
		case Expression::Length:
			return left.length();
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

bool Value::compare(Value* left, Expression::Operator_e op, Value* right)
{
	return Value::operation(left,op,right)->isTrue();
}

Value* Value::compareAll(const QList<Value*>& values, Expression::Operator_e op)
{
	Value* result=nullptr;
	for(Value* a: values) {
		auto* numVal=dynamic_cast<NumberValue*>(a);
		if(numVal) {
			if(!result||compare(a,op,result))
				result=a;
		}
		auto* vecVal=dynamic_cast<VectorValue*>(a);
		if(vecVal) {
			Value* c=compareAll(vecVal->getChildren(),op);
			if(!result||compare(c,op,result))
				result=c;
		}
		auto* rngVal=dynamic_cast<RangeValue*>(a);
		if(rngVal) {
			QList<Value*> rng;
			rng.append(rngVal->getStart());
			rng.append(rngVal->getFinish());
			Value* c=compareAll(rng,op);
			if(!result||compare(c,op,result))
				result=c;
		}
	}
	if(!result)
		return Value::undefined();

	return result;
}
