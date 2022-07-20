/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2022 Giles Bathgate
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

#ifndef FUNCTIONSCOPE_H
#define FUNCTIONSCOPE_H

#include "expression.h"
#include "scope.h"
#include "statement.h"
#include <QList>

class FunctionScope : public Scope
{
public:
	FunctionScope();
	~FunctionScope() override;
	void setExpression(Expression*);
	Expression* getExpression() const;
	void setStatements(const QList<Statement*>&);
	const QList<Statement*> getStatements() const;
	void accept(TreeVisitor&) override;
private:
	Expression* expression;
	QList<Statement*> statements;
};

#endif // FUNCTIONSCOPE_H
