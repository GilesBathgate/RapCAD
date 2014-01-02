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

#include "ifelsestatement.h"

IfElseStatement::IfElseStatement()
{
	expression=NULL;
	trueStatement=NULL;
	falseStatement=NULL;
}

IfElseStatement::~IfElseStatement()
{
	delete expression;
	delete trueStatement;
	delete falseStatement;
}

void IfElseStatement::setExpression(Expression* expr)
{
	this->expression = expr;
}

Expression* IfElseStatement::getExpression() const
{
	return this->expression;
}

void IfElseStatement::setTrueStatement(Statement* stmt)
{
	this->trueStatement = stmt;
}

Statement* IfElseStatement::getTrueStatement() const
{
	return this->trueStatement;
}

void IfElseStatement::setFalseStatement(Statement* stmt)
{
	this->falseStatement = stmt;
}

Statement* IfElseStatement::getFalseStatement() const
{
	return this->falseStatement;
}

void IfElseStatement::accept(TreeVisitor& v)
{
	v.visit(this);
}
