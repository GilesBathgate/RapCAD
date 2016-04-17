/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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
#include <QString>
#include "iterator.h"
#include "expression.h"
#include "variable.h"
#include "decimal.h"

class Value
{
public:
	static Value* undefined();
	virtual ~Value();
	static void cleanup();
	void setStorage(Variable::Storage_e);
	Variable::Storage_e getStorage() const;
	void setName(QString);
	QString getName() const;
	virtual QString getValueString() const;
	virtual bool isTrue() const;
	bool isDefined() const;
	virtual VectorValue* toVector(int);
	virtual TextValue* toText();
	virtual Value* toNumber();
	virtual Iterator<Value*>* createIterator();
	Value* operator^(Value&);
	Value* operator*(Value&);
	Value* concatenate(Value& v);
	Value* componentwiseMultiply(Value&);
	Value* operator/(Value&);
	Value* componentwiseDivide(Value&);
	Value* crossProduct(Value& v);
	Value* operator%(Value&);
	Value* operator+();
	Value* operator+(Value&);
	Value* operator+=(Value&);
	Value* operator++(int);
	Value* length();
	Value* length(Value& v);
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
	static bool compare(Value*,Expression::Operator_e,Value*);
	static Value* compareAll(QList<Value*>,Expression::Operator_e);

protected:
	Value();
	bool isComparison(Expression::Operator_e);
	template <class T>
	T basicOperation(T,Expression::Operator_e,T);
	template <class T>
	T basicOperation(T,Expression::Operator_e);

	virtual Value* operation(Expression::Operator_e);
	virtual Value* operation(Value&,Expression::Operator_e);
private:
	bool defined;
	static QList<Value*> values;
	Variable::Storage_e storageClass;
	QString name;
	bool modulus(bool,bool);
	decimal modulus(decimal,decimal);
	bool exponent(bool,bool);
	decimal exponent(decimal,decimal);
	bool logic(bool);
	bool logic(decimal);
	bool length(bool);
	decimal length(decimal);
};

template <class T>
T Value::basicOperation(T left, Expression::Operator_e e, T right)
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
		return logic(left)&&logic(right);
	case Expression::LogicalOr:
		return logic(left)||logic(right);
	default:
		return left;
	}
}

template <class T>
T Value::basicOperation(T left, Expression::Operator_e e)
{
	switch(e) {
	case Expression::Add:
		return +left;
	case Expression::Subtract:
		return -left;
	case Expression::Invert:
		return !logic(left);
	case Expression::Increment:
		return left+1;
	case Expression::Decrement:
		return left-1;
	case Expression::Length:
		return length(left);
	default:
		return left;
	}
}

#endif // VALUE_H
