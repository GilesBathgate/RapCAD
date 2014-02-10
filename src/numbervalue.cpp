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

#include "numbervalue.h"
#include "vectorvalue.h"
#include "booleanvalue.h"

NumberValue::NumberValue(decimal value)
{
	this->number=value;
	this->defined=true;
}

QString NumberValue::getValueString() const
{
	return QString().setNum(this->number,'g',16);
}

bool NumberValue::isTrue() const
{
	return this->number!=0;
}

decimal NumberValue::getNumber() const
{
	return this->number;
}

Value* NumberValue::operation(Expression::Operator_e e)
{
	decimal result = basicOperation<decimal,decimal>(this->number,e);
	return new NumberValue(result);
}

Value* NumberValue::operation(Value& v, Expression::Operator_e e)
{
	NumberValue* num = dynamic_cast<NumberValue*>(&v);
	if(num) {
		if(isComparison(e)) {
			bool result=basicOperation<bool,decimal>(this->number,e,num->number);
			return new BooleanValue(result);
		} else {
			decimal result=basicOperation<decimal,decimal>(this->number,e,num->number);
			return new NumberValue(result);
		}
	}
	VectorValue* vec = dynamic_cast<VectorValue*>(&v);
	if(vec) {
		if(e==Expression::Concatenate) {
			QList<Value*> r=vec->getChildren();
			r.prepend(this);
			return new VectorValue(r);
		} else {
			//operations between scalars and vectors are commutative e.g.
			// [1,2,3]-1  is the same as 1 - [1,2,3]
			return Value::operation(vec,e,this);
		}
	}

	return Value::operation(v,e);
}
