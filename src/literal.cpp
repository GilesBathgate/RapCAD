/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2020 Giles Bathgate
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
	value(nullptr),
	unit(1)
{
}

void Literal::setValue()
{
	value=Value::factory.createUndefined();
}

void Literal::setValue(bool v)
{
	value=Value::factory.createBoolean(v);
}

void Literal::setValue(decimal v)
{
	value=Value::factory.createNumber(v);
}

void Literal::setValue(const QString& v)
{
	value=Value::factory.createText(v);
}

void Literal::setUnit(const QString& s)
{
	suffix=s;
	if(s=="m")
		unit=1000;
	else if(s=="cm")
		unit=10;
	else if(s=="mm")
		unit=1;
	else if(s=="um")
		unit=decimal(1)/1000;
	else if(s=="ft")
		unit=decimal(3048)/10;
	else if(s=="in")
		unit=decimal(254)/10;
	else if(s=="th")
		unit=decimal(254)/10000;
	else
		unit=1;
}

QString Literal::getValueString() const
{
	NumberValue* num=dynamic_cast<NumberValue*>(value);
	if(num)
		return value->getValueString().append(suffix);

	return value->getValueString();
}

Value* Literal::getValue() const
{
	if(unit==1) return value;

	NumberValue* num=dynamic_cast<NumberValue*>(value);
	if(num)
		return Value::operation(num,Expression::Multiply,Value::factory.createNumber(unit));

	return value;
}

void Literal::accept(TreeVisitor& v)
{
	v.visit(*this);
}
