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

#include "textvalue.h"
#include "numbervalue.h"

TextValue::TextValue(QString value)
{
	this->text=value;
	this->defined=true;
}

QString TextValue::getValueString() const
{
	return this->text;
}

bool TextValue::isTrue() const
{
	return !this->text.isEmpty();
}

Value* TextValue::operation(Value& v,Expression::Operator_e e)
{
	TextValue* that=dynamic_cast<TextValue*>(&v);
	if(that)
		return new TextValue(operation(this->text,e,that->text));

	NumberValue* num=dynamic_cast<NumberValue*>(&v);
	if(num)
		if(e==Expression::Index)
			return new TextValue(this->text.at(num->getNumber()));

	return Value::operation(v,e);
}

QString TextValue::operation(QString left, Expression::Operator_e e, QString right)
{
	switch(e) {
	case Expression::Concatenate:
		return left.append(right);
	default:
		return this->text;
	}
}
