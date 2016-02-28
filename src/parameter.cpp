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

#include "parameter.h"

Parameter::Parameter()
{
	expression=NULL;
}

Parameter::~Parameter()
{
	delete expression;
}

QString Parameter::getName() const
{
	return this->name;
}

void Parameter::setName(QString name)
{
	this->name = name;
}

Expression* Parameter::getExpression() const
{
	return this->expression;
}

void Parameter::setExpression(Expression* expr)
{
	this->expression = expr;
}

void Parameter::addDescription(QString d)
{
	description=d;
}

QString Parameter::getDescription() const
{
	return description;
}

void Parameter::accept(TreeVisitor& v)
{
	v.visit(this);
}
