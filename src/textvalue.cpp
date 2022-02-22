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

#include "textvalue.h"
#include "booleanvalue.h"
#include "numbervalue.h"
#include "textiterator.h"
#include "valuefactory.h"

TextValue::TextValue(const QString& value) :
	text(value)
{
}

QString TextValue::getValueString() const
{
	return text;
}

TextValue& TextValue::toText()
{
	return *this;
}

Value& TextValue::toNumber()
{
	bool ok=false;
	decimal n=to_decimal(text,&ok);
	if(ok)
		return ValueFactory::createNumber(n);

	return ValueFactory::createUndefined();
}

ValueIterator* TextValue::createIterator()
{
	return new TextIterator(text);
}

bool TextValue::isTrue() const
{
	return !text.isEmpty();
}

Value& TextValue::operation(Operators op)
{
	if(op==Operators::Length) {
		return ValueFactory::createNumber(text.length());
	}
	return *this;
}

Value& TextValue::operation(Value& v,Operators e)
{
	auto* that=dynamic_cast<TextValue*>(&v);
	if(that)
		return operation(*that,e);

	auto* num=dynamic_cast<NumberValue*>(&v);
	if(num)
		return operation(*num,e);

	return Value::operation(v,e);
}

Value& TextValue::operation(TextValue& that,Operators e)
{
	if(isComparison(e)) {
		switch(e) {
			case Operators::Equal:
				return ValueFactory::createBoolean(text==that.text);
			case Operators::NotEqual:
				return ValueFactory::createBoolean(text!=that.text);
			default:
				return ValueFactory::createBoolean(basicOperation(isTrue(),e,that.isTrue()));
		}
	}

	if(e==Operators::Concatenate)
		return ValueFactory::createText(text.append(that.text));

	return Value::operation(that,e);
}

Value& TextValue::operation(NumberValue& num,Operators e)
{
	if(e==Operators::Index)
		return ValueFactory::createText(text.at(num.toInteger()));

	return Value::operation(num,e);
}
