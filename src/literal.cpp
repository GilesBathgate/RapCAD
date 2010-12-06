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

#include "literal.h"

Literal::Literal()
{
}

Literal::~Literal()
{
}

void Literal::setValue(bool value)
{
    this->type = Literal::Boolean;
    this->value.Boolean = value;
}

void Literal::setValue(double value)
{
    this->type = Literal::Number;
    this->value.Number = value;
}

void Literal::setValue(QString value)
{
    this->type = Literal::String;
    this->value_text = value;
}

QString Literal::getValueString()
{
    QString result;
    switch(this->type)
    {
    case Boolean:
	if(this->value.Boolean)
	    result="true";
	else
	    result="false";
	break;
    case Number:
	result=QString("%1").arg(this->value.Number);
	break;
    case String:
	result=value_text;
	break;
    }

    return result;
}

void Literal::accept(Visitor& v)
{
    v.visit(this);
}
