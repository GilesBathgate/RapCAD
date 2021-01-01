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

#include "literal.h"
#include "booleanvalue.h"
#include "numbervalue.h"
#include "textvalue.h"
#include "valuefactory.h"

Literal::Literal() :
	boolean(false),
	type(DataTypes::Undef),
	unit(1.0)
{
}

void Literal::setValue()
{
	type = DataTypes::Undef;
}

void Literal::setValue(bool value)
{
	type = DataTypes::Boolean;
	boolean = value;
}

void Literal::setValue(const decimal& value)
{
	type = DataTypes::Number;
	number = value;
}

void Literal::setValue(const QString& value)
{
	type = DataTypes::Text;
	text = value;
}

void Literal::setUnit(const QString& value)
{
	text=value;
	if(value=="m")
		unit=1000.0;
	else if(value=="cm")
		unit=10.0;
	else if(value=="mm")
		unit=1.0;
	else if(value=="um")
		unit=decimal(1.0)/1000.0;
	else if(value=="ft")
		unit=decimal(3048.0)/10.0;
	else if(value=="in")
		unit=decimal(254.0)/10.0;
	else if(value=="th")
		unit=decimal(254.0)/10000.0;
	else
		type=DataTypes::Undef;
}

QString Literal::getValueString() const
{
	switch(type) {
		case DataTypes::Boolean:
			return boolean ? "true" : "false";
		case DataTypes::Number:
			return to_string(number).append(text);
		case DataTypes::Text:
			return QString("\"%1\"").arg(text);
		default:
			return "undef";
	}
}

Value* Literal::getValue() const
{
	switch(type) {
		case DataTypes::Boolean:
			return Value::factory.createBoolean(boolean);
		case DataTypes::Number:
			return Value::factory.createNumber(number*unit);
		case DataTypes::Text:
			return Value::factory.createText(text);
		default:
			return Value::factory.createUndefined();
	}
}

void Literal::accept(TreeVisitor& v)
{
	v.visit(*this);
}
