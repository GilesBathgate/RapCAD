/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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

#ifndef ARGUMENT_H
#define ARGUMENT_H

#include "expression.h"
#include "variable.h"
#include "visitabletree.h"

class Argument : public VisitableTree
{
public:
	Argument();
	~Argument() override;
	void setVariable(Variable*);
	Variable* getVariable() const;
	void setExpression(Expression*);
	Expression* getExpression() const;
	void accept(TreeVisitor&) override;
private:
	Variable* variable;
	Expression* expression;
};

#endif // ARGUMENT_H
