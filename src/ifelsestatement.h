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

#ifndef IFELSESTATEMENT_H
#define IFELSESTATEMENT_H

#include <QString>
#include "statement.h"
#include "expression.h"

class IfElseStatement : public Statement
{
public:
	IfElseStatement();
	~IfElseStatement() override;
	void setExpression(Expression*);
	Expression* getExpression() const;
	void setTrueStatement(Statement*);
	Statement* getTrueStatement() const;
	void setFalseStatement(Statement*);
	Statement* getFalseStatement() const;
	void accept(TreeVisitor&) override;
private:
	Expression* expression;
	Statement* trueStatement;
	Statement* falseStatement;
};

#endif // IFELSESTATEMENT_H
