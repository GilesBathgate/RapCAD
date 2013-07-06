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

#ifndef VALUE_H
#define VALUE_H

#include <QString>
#include "iterator.h"
#include "expression.h"
#include "variable.h"

class Value
{
public:
	Value();
	virtual ~Value();
	static void cleanup();
	void setStorageClass(Variable::StorageClass_e);
	Variable::StorageClass_e getStorageClass() const;
	void setName(QString);
	QString getName() const;
	virtual QString getValueString() const;
	virtual bool isTrue() const;
	bool isDefined() const;
	virtual class VectorValue* toVector(int);
	virtual Iterator<Value*>* createIterator();
	Value* operator^(Value&);
	Value* operator*(Value&);
	Value* concatenate(Value& v);
	Value* componentwiseMultiply(Value&);
	Value* operator/(Value&);
	Value* componentwiseDivide(Value&);
	Value* outerProduct(Value& v);
	Value* operator%(Value&);
	Value* operator+();
	Value* operator+(Value&);
	Value* operator+=(Value&);
	Value* operator++(int);
	Value* operator-();
	Value* operator-(Value&);
	Value* operator-=(Value&);
	Value* operator--(int);
	Value* operator<(Value&);
	Value* operator<=(Value&);
	Value* operator==(Value&);
	Value* operator!=(Value&);
	Value* operator>=(Value&);
	Value* operator>(Value&);
	Value* operator&&(Value&);
	Value* operator||(Value&);
	Value* operator[](Value&);
	Value* operator!();

	static Value* operation(Value*,Expression::Operator_e);
	static Value* operation(Value*,Expression::Operator_e,Value*);
protected:
	bool defined;
	bool isComparison(Expression::Operator_e);
	template <class A, class B>
	A basicOperation(B,Expression::Operator_e,B);
	template <class A, class B>
	A basicOperation(B,Expression::Operator_e);
	virtual Value* operation(Expression::Operator_e);
	virtual Value* operation(Value&,Expression::Operator_e);
private:
	static QList<Value*> values;
	Variable::StorageClass_e storageClass;
	QString name;
	template<class T>
	T modulus(T left, T right);
	double modulus(double left, double right);
	template<class T>
	T exponent(T left, T right);
	double exponent(double left, double right);
};

template<class T>
T Value::modulus(T left, T right)
{
	return left%right;
}

template<class T>
T Value::exponent(T left, T right)
{
	return left^right;
}

template <class A, class B>
A Value::basicOperation(B left, Expression::Operator_e e, B right)
{
	switch(e) {
	case Expression::Exponent:
		return exponent(left,right);
	case Expression::Multiply:
		return left*right;
	case Expression::Divide:
		return left/right;
	case Expression::Modulus:
		return modulus(left,right);
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
	default:
		return left;
	}
}

template <class A, class B>
A Value::basicOperation(B left, Expression::Operator_e e)
{
	switch(e) {
	case Expression::Add:
		return +left;
	case Expression::Subtract:
		return -left;
	case Expression::Invert:
		return !left;
	case Expression::Increment:
		return left+1;
	case Expression::Decrement:
		return left-1;
	default:
		return left;
	}
}
#endif // VALUE_H
