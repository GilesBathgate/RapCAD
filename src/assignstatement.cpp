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

#include "assignstatement.h"

AssignStatement::AssignStatement() :
	variable(nullptr),
	expression(nullptr),
	operation(Expression::None)
{
}

AssignStatement::~AssignStatement()
{
	delete variable;
	delete expression;
}

void AssignStatement::setVariable(Variable* var)
{
	variable = var;
}

Variable* AssignStatement::getVariable() const
{
	return variable;
}

void AssignStatement::setExpression(Expression* exp)
{
	expression = exp;
}

Expression* AssignStatement::getExpression() const
{
	return expression;
}

void AssignStatement::setOperation(Expression::Operator_e op)
{
	operation=op;
}

Expression::Operator_e AssignStatement::getOperation() const
{
	return operation;
}

void AssignStatement::accept(TreeVisitor& v)
{
	v.visit(*this);
}
