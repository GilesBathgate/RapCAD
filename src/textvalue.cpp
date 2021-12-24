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
#include "numbervalue.h"
#include "booleanvalue.h"
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
		return factory.createNumber(n);

	return factory.createUndefined();
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
		return factory.createNumber(text.length());
	}
	return *this;
}

Value& TextValue::operation(Value& v,Operators e)
{
	auto* that=dynamic_cast<TextValue*>(&v);
	if(that) {
		if(isComparison(e)) {
			return factory.createBoolean(operation(this,e,that));
		}
		return factory.createText(operation(this->text,e,that->text));
	}

	auto* num=dynamic_cast<NumberValue*>(&v);
	if(num)
		if(e==Operators::Index)
			return factory.createText(text.at(num->toInteger()));

	return Value::operation(v,e);
}

QString TextValue::operation(QString& left, Operators e,QString& right)
{
	switch(e) {
		case Operators::Concatenate:
			return left.append(right);
		default:
			return text;
	}
}

bool TextValue::operation(TextValue* left, Operators e, TextValue* right)
{
	switch(e) {
		case Operators::Equal:
			return left->text==right->text;
		case Operators::NotEqual:
			return left->text!=right->text;
		default:
			return basicOperation(left->isTrue(),e,right->isTrue());
	}
}
