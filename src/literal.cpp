/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2019 Giles Bathgate
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

Literal::Literal() :
	boolean(false),
	type(Undef),
	unit(1)
{
}

void Literal::setValue(bool value)
{
	type = Boolean;
	boolean = value;
}

void Literal::setValue(decimal value)
{
	type = Number;
	number = value;
}

void Literal::setValue(const QString& value)
{
	type = Text;
	text = value;
}

void Literal::setUnit(const QString& value)
{
	text=value;
	if(value=="m")
		unit=1000;
	else if(value=="cm")
		unit=10;
	else if(value=="mm")
		unit=1;
	else if(value=="um")
		unit=decimal(1)/1000;
	else if(value=="ft")
		unit=decimal(3048)/10;
	else if(value=="in")
		unit=decimal(254)/10;
	else if(value=="th")
		unit=decimal(254)/10000;
	else
		type=Undef;
}

QString Literal::getValueString() const
{
	switch(type) {
		case Boolean:
			return boolean ? "true" : "false";
		case Number:
			return to_string(number).append(text);
		case Text:
			return QString("\"%1\"").arg(text);
		default:
			return "undef";
	}
}

Value* Literal::getValue() const
{
	switch(type) {
		case Boolean:
			return new BooleanValue(boolean);
		case Number:
			return new NumberValue(number*unit);
		case Text:
			return new TextValue(text);
		default:
			return Value::undefined();
	}
}

void Literal::accept(TreeVisitor& v)
{
	v.visit(*this);
}
