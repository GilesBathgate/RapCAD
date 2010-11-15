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

#include "value.h"

Value::Value()
{
}

Value::~Value()
{
}

QString Value::getName()
{
    return this->name;
}

void Value::setName(QString name)
{
    this->name = name;
}

void Value::setType(Type_e type)
{
    this->type = type;
}

Value::Type_e Value::getType()
{
    return this->type;
}

QString Value::toString()
{
    QString result;
    switch(type)
    {
    case Const:
	result.append("ConstValue: ");
	break;
    case Param:
	result.append("ParamValue: ");
	break;
    default:
	result.append("Value: ");
	break;
    }

    result.append(this->name);
    result.append(" ");
    return result;
}
