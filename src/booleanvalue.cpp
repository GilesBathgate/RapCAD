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

#include "booleanvalue.h"
#include "numbervalue.h"
#include "valuefactory.h"

BooleanValue::BooleanValue(bool value) :
	boolean(value)
{
}

QString BooleanValue::getValueString() const
{
	return boolean ? "true" : "false";
}

Value& BooleanValue::toNumber()
{
	decimal result=boolean?1.0:0.0;
	return ValueFactory::createNumber(result);
}

bool BooleanValue::isTrue() const
{
	return boolean;
}

Value& BooleanValue::operation(Operators e)
{
	bool result=basicOperation(boolean,e);
	return ValueFactory::createBoolean(result);
}

Value& BooleanValue::operation(Value& v,Operators e)
{
	auto* that=dynamic_cast<BooleanValue*>(&v);
	if(that)
		return operation(*that,e);

	auto* num=dynamic_cast<NumberValue*>(&v);
	if(num)
		return operation(*num,e);

	return Value::operation(v,e);
}

Value& BooleanValue::operation(BooleanValue& that,Operators e)
{
	bool result=basicOperation(this->boolean,e,that.boolean);
	return ValueFactory::createBoolean(result);
}

Value& BooleanValue::operation(NumberValue& num,Operators e)
{
	if(isComparison(e)) {
		bool result=basicOperation(this->boolean?1:0,e,num.toInteger());
		return ValueFactory::createBoolean(result);
	}

	return Value::operation(num,e);
}
