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

#include "functionscope.h"

FunctionScope::FunctionScope()
{
	expression=NULL;
}

FunctionScope::~FunctionScope()
{
	foreach(Statement* s,statements)
		delete s;

	delete expression;
}

void FunctionScope::setExpression(Expression* exp)
{
	this->expression = exp;
}

Expression* FunctionScope::getExpression() const
{
	return this->expression;
}

void FunctionScope::setStatements(QList<Statement*> stmts)
{
	this->statements = stmts;
}

QList<Statement*> FunctionScope::getStatements() const
{
	return this->statements;
}

void FunctionScope::accept(TreeVisitor& v)
{
	v.visit(this);
}
