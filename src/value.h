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

#ifndef VALUE_H
#define VALUE_H

#include <QString>
#include "expression.h"

class Value
{
public:
	Value();
	void setName(QString);
	QString getName();
	virtual QString getValueString();
	virtual Value* operator+(Value&);
	virtual Value* operator-(Value&);
	virtual Value* operator&&(Value&);
	virtual Value* operator||(Value&);

	static Value* operation(Value*,Expression::Operator_e,Value*);

	template <class T>
	static T basicOperation(T,Expression::Operator_e,T);
private:
	QString name;
};

template <class T>
T Value::basicOperation(T left, Expression::Operator_e e, T right)
{
    switch(e)
    {
    case Expression::Add:
	return left+right;
    case Expression::Subtract:
	return left-right;
    case Expression::LogicalAnd:
	return left&&right;
    case Expression::LogicalOr:
	return left||right;
    }
}
#endif // VALUE_H
