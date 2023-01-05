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

#ifndef VALUE_H
#define VALUE_H

class VectorValue;
class TextValue;
class ValueIterator;
class ValueFactory;
#include "decimal.h"
#include "variable.h"
#include <QString>

class Value
{
public:
	virtual ~Value();
	void setStorage(Storage);
	Storage getStorage() const;
	virtual QString getValueString() const;
	virtual bool isTrue() const;
	bool isFalse() const;
	bool isDefined() const;
	bool isUndefined() const;
	virtual VectorValue& toVector(int);
	virtual TextValue& toText();
	virtual Value& toNumber();
	virtual ValueIterator* createIterator();
	Value& operator^(Value&);
	Value& operator*(Value&);
	Value& concatenate(Value& v);
	Value& componentwiseMultiply(Value&);
	Value& operator/(Value&);
	Value& componentwiseDivide(Value&);
	Value& crossProduct(Value& v);
	Value& operator%(Value&);
	Value& operator+();
	Value& operator+(Value&);
	Value& operator+=(Value&);
	Value& operator++(int);
	Value& length();
	Value& length(Value& v);
	Value& operator-();
	Value& operator-(Value&);
	Value& operator-=(Value&);
	Value& operator--(int);
	Value& operator<(Value&);
	Value& operator<=(Value&);
	Value& operator==(Value&);
	Value& operator!=(Value&);
	Value& operator>=(Value&);
	Value& operator>(Value&);
	Value& operator&&(Value&);
	Value& operator||(Value&);
	Value& operator[](Value&);
	Value& operator!();

	static Value* evaluate(Value*,Operators);
	static Value& evaluate(Value&,Operators);
	static Value* evaluate(Value*,Operators,Value*);
	static Value& evaluate(Value&,Operators,Value&);
	static bool compare(Value&,Operators,Value&);
	static Value& compareAll(const QList<Value*>&,Operators);
protected:
	Value();
	static bool isComparison(Operators);
	template <class T>
	static T basicOperation(T,Operators,T);
	template <class T>
	static T basicOperation(T,Operators);

	virtual Value& operation(Operators);
	virtual Value& operation(Value&,Operators);
private:
	friend class ValueFactory;
	bool defined;
	Storage storageClass;
	QString name;

	static bool modulus(bool,bool);
	static bool multiply(bool,bool);
	static bool exponent(bool,bool);
	static bool logic(bool);
#if USE_CGAL
	static decimal modulus(const decimal&,const decimal&);
	static decimal multiply(const decimal&,const decimal&);
	static decimal exponent(const decimal&,const decimal&);
	static bool logic(const decimal&);
#endif
	static bool length(bool);
	static decimal length(const decimal&);
};

template <class T>
T Value::basicOperation(T left, Operators e, T right)
{
	switch(e) {
		case Operators::Exponent:
			return exponent(left,right);
		case Operators::Multiply:
			return multiply(left,right);
		case Operators::Divide:
			return left/right;
		case Operators::Modulus:
			return modulus(left,right);
		case Operators::Add:
			return left+right;
		case Operators::Subtract:
			return left-right;
		case Operators::AddAssign:
			return left+=right;
		case Operators::SubAssign:
			return left-=right;
		case Operators::LessThan:
			return left<right;
		case Operators::LessOrEqual:
			return left<=right;
		case Operators::Equal:
			return left==right;
		case Operators::NotEqual:
			return left!=right;
		case Operators::GreaterOrEqual:
			return left>=right;
		case Operators::GreaterThan:
			return left>right;
		case Operators::LogicalAnd:
			return logic(left)&&logic(right);
		case Operators::LogicalOr:
			return logic(left)||logic(right);
		default:
			return left;
	}
}

template <class T>
T Value::basicOperation(T left, Operators e)
{
	switch(e) {
		case Operators::Add:
			return +left;
		case Operators::Subtract:
			return -left;
		case Operators::Invert:
			return !logic(left);
		case Operators::Increment:
			return left+1;
		case Operators::Decrement:
			return left-1;
		case Operators::Length:
			return length(left);
		default:
			return left;
	}
}

#endif // VALUE_H
