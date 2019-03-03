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

#ifndef ASSIGNSTATEMENT_H
#define ASSIGNSTATEMENT_H

#include "statement.h"
#include "variable.h"
#include "expression.h"

class AssignStatement : public Statement
{
public:
	AssignStatement();
	~AssignStatement() override;
	void setVariable(Variable*);
	Variable* getVariable() const;
	void setExpression(Expression*);
	Expression* getExpression() const;
	void setOperation(Expression::Operator_e);
	Expression::Operator_e getOperation() const;
	void accept(TreeVisitor&) override;
private:
	Variable* variable;
	Expression* expression;
	Expression::Operator_e operation;
};

#endif // ASSIGNSTATEMENT_H
