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

#include "value.h"
#include "valueiterator.h"
#include "vectorvalue.h"
#include "booleanvalue.h"
#include "textvalue.h"
#include "numbervalue.h"
#include "rangevalue.h"
#include "rmath.h"
#include "valuefactory.h"

Value::Value() :
	defined(true),
	storageClass(Storage::Constant)
{
}

Value::~Value()
{
	factory.deleteValue(this);
}

ValueFactory& Value::factory=ValueFactory::getInstance();

void Value::setStorage(Storage c)
{
	storageClass=c;
}

Storage Value::getStorage() const
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

VectorValue& Value::toVector(int size)
{
	QList<Value*> children;
	for(auto i=0; i<size; ++i)
		children.append(this);

	return factory.createVector(children);
}

TextValue& Value::toText()
{
	return factory.createText(getValueString());
}

Value& Value::toNumber()
{
	return *this;
}

ValueIterator* Value::createIterator()
{
	return new ValueIterator(this);
}

Value& Value::operator^(Value& v)
{
	return operation(v,Operators::Exponent);
}

Value& Value::operator*(Value& v)
{
	return operation(v,Operators::Multiply);
}

Value& Value::concatenate(Value& v)
{
	return operation(v,Operators::Concatenate);
}

Value& Value::componentwiseMultiply(Value& v)
{
	return operation(v,Operators::ComponentwiseMultiply);
}

Value& Value::operator/(Value& v)
{
	return operation(v,Operators::Divide);
}

Value& Value::componentwiseDivide(Value& v)
{
	return operation(v,Operators::ComponentwiseDivide);
}

Value& Value::crossProduct(Value& v)
{
	return operation(v,Operators::CrossProduct);
}

Value& Value::operator%(Value& v)
{
	return operation(v,Operators::Modulus);
}

Value& Value::operator+()
{
	return operation(Operators::Add);
}

Value& Value::operator+(Value& v)
{
	return operation(v,Operators::Add);
}

Value& Value::operator+=(Value& v)
{
	return operation(v,Operators::AddAssign);
}

Value& Value::operator++(int)
{
	return operation(Operators::Increment);
}

Value& Value::length()
{
	return operation(Operators::Length);
}

Value& Value::length(Value& v)
{
	return operation(v,Operators::Length);
}

Value& Value::operator-()
{
	return operation(Operators::Subtract);
}

Value& Value::operator-(Value& v)
{
	return operation(v,Operators::Subtract);
}

Value& Value::operator-=(Value& v)
{
	return operation(v,Operators::SubAssign);
}

Value& Value::operator--(int)
{
	return operation(Operators::Decrement);
}

Value& Value::operator<(Value& v)
{
	return operation(v,Operators::LessThan);
}

Value& Value::operator<=(Value& v)
{
	return operation(v,Operators::LessOrEqual);
}

Value& Value::operator==(Value& v)
{
	return operation(v,Operators::Equal);
}

Value& Value::operator!=(Value& v)
{
	return operation(v,Operators::NotEqual);
}

Value& Value::operator>=(Value& v)
{
	return operation(v,Operators::GreaterOrEqual);
}

Value& Value::operator>(Value& v)
{
	return operation(v,Operators::GreaterThan);
}

Value& Value::operator&&(Value& v)
{
	return operation(v,Operators::LogicalAnd);
}

Value& Value::operator||(Value& v)
{
	return operation(v,Operators::LogicalOr);
}

Value& Value::operator[](Value& v)
{
	return operation(v,Operators::Index);
}

Value& Value::operator!()
{
	return operation(Operators::Invert);
}

bool Value::modulus(bool, bool)
{
	return false;
}

bool Value::multiply(bool left, bool right)
{
	return left&&right;
}

bool Value::exponent(bool left, bool right)
{
	return left^right;
}

bool Value::logic(bool a)
{
	return a;
}

#if USE_CGAL
decimal Value::modulus(const decimal& left, const decimal& right)
{
	return r_mod(left,right);
}

decimal Value::multiply(const decimal& left, const decimal& right)
{
	return left*right;
}

decimal Value::exponent(const decimal& left, const decimal& right)
{
	return r_pow(left,right);
}

bool Value::logic(const decimal& a)
{
	return to_boolean(a);
}
#endif

bool Value::length(bool left)
{
	return left;
}

decimal Value::length(const decimal& left)
{
	return r_abs(left);
}

Value& Value::operation(Operators e)
{
	if(e==Operators::Invert) {
		bool result=basicOperation(defined,e);
		return factory.createBoolean(result);
	}

	return *this;
}

Value& Value::operation(Value& v, Operators e)
{
	bool left=defined;
	bool right=v.defined;
	if((!left||!right) && isComparison(e)) {
		bool result=basicOperation(left,e,right);
		return factory.createBoolean(result);
	}
	if(e==Operators::Concatenate) {
		return v;
	}

	return factory.createUndefined();
}

bool Value::isDefined() const
{
	return defined;
}

bool Value::isUndefined() const
{
	return !defined;
}

Value* Value::operation(Value* p_left, Operators e, Value* p_right)
{
	Value& left=*p_left;
	Value& right=*p_right;
	switch(e) {
		case Operators::Exponent:
			return &(left^right);
		case Operators::DotProduct:
		case Operators::Multiply:
			return &(left*right);
		case Operators::Append:
		case Operators::Concatenate:
			return &left.concatenate(right);
		case Operators::ComponentwiseMultiply:
			return &left.componentwiseMultiply(right);
		case Operators::Divide:
			return &(left/right);
		case Operators::ComponentwiseDivide:
			return &left.componentwiseDivide(right);
		case Operators::CrossProduct:
			return &left.crossProduct(right);
		case Operators::Modulus:
			return &(left%right);
		case Operators::Add:
			return &(left+right);
		case Operators::Subtract:
			return &(left-right);
		case Operators::AddAssign:
			return &(left+=right);
		case Operators::SubAssign:
			return &(left-=right);
		case Operators::LessThan:
			return &(left<right);
		case Operators::LessOrEqual:
			return &(left<=right);
		case Operators::Equal:
			return &(left==right);
		case Operators::NotEqual:
			return &(left!=right);
		case Operators::GreaterOrEqual:
			return &(left>=right);
		case Operators::GreaterThan:
			return &(left>right);
		case Operators::LogicalAnd:
			return &(left&&right);
		case Operators::LogicalOr:
			return &(left||right);
		case Operators::Index:
			return &left[right];
		case Operators::Length:
			return &left.length(right);
		default:
			return &left;
	}
}

Value* Value::operation(Value* p_left, Operators e)
{
	Value& left=*p_left;
	switch(e) {
		case Operators::Add:
			return &(+left);
		case Operators::Subtract:
			return &(-left);
		case Operators::Invert:
			return &(!left);
		case Operators::Increment:
			return &(left++);
		case Operators::Decrement:
			return &(left--);
		case Operators::Length:
			return &left.length();
		default:
			return &left;
	}
}

bool Value::isComparison(Operators e)
{
	switch(e) {
		case Operators::LessThan:
		case Operators::LessOrEqual:
		case Operators::Equal:
		case Operators::NotEqual:
		case Operators::GreaterOrEqual:
		case Operators::GreaterThan:
		//The following are not really comparisons but
		//we expect them to return a boolean result
		case Operators::LogicalAnd:
		case Operators::LogicalOr:
			return true;
		default:
			return false;
	}
}

bool Value::compare(Value* left, Operators op, Value* right)
{
	return Value::operation(left,op,right)->isTrue();
}

Value* Value::compareAll(const QList<Value*>& values, Operators op)
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
			Value* c=compareAll(vecVal->getElements(),op);
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
		return &factory.createUndefined();

	return result;
}
