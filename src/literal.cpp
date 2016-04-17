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

#include "literal.h"
#include "booleanvalue.h"
#include "numbervalue.h"
#include "textvalue.h"

Literal::Literal()
{
	type=Literal::Undef;
}

void Literal::setValue(bool value)
{
	this->type=Literal::Boolean;
	this->boolean = value;
}

void Literal::setValue(decimal value)
{
	this->type = Literal::Number;
	this->number = value;
}

void Literal::setValue(QString value)
{
	this->type = Literal::Text;
	this->text = value;
}

QString Literal::getValueString() const
{
	switch(this->type) {
	case Boolean:
		return this->boolean ? "true" : "false";
	case Number:
		return to_string(number);
	case Text:
		return QString("\"%1\"").arg(text);
	default:
		return "undef";
	}
}

Value* Literal::getValue() const
{
	switch(this->type) {
	case Boolean:
		return new BooleanValue(boolean);
	case Number:
		return new NumberValue(number);
	case Text:
		return new TextValue(text);
	default:
		return Value::undefined();
	}
}

void Literal::accept(TreeVisitor& v)
{
	v.visit(this);
}
