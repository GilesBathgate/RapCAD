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

#include "argument.h"

Argument::Argument()
{
}

void Argument::setVariable(Variable* var)
{
    this->variable = var;
}

Variable* Argument::getVariable()
{
    return this->variable;
}

void Argument::setExpression(Expression* exp)
{
    this->expression = exp;
}

Expression* Argument::getExpression()
{
    return this->expression;
}

QString Argument::toString()
{
    QString result;
    result.append("Argument: ");
    if(variable)
	result.append(this->variable->toString());

    result.append(" ");
    if(expression)
	result.append(this->expression->toString());

    return result;
}
