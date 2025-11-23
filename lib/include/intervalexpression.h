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
#ifndef INTERVALEXPRESSION_H
#define INTERVALEXPRESSION_H

#include "expression.h"
#include "decimal.h"

class IntervalExpression : public Expression
{
	Q_DISABLE_COPY_MOVE(IntervalExpression)
public:
	IntervalExpression();
	~IntervalExpression() override;

	const decimal& getValue() const;
	void setValue(const decimal&);

	Expression* getMore() const;
	void setMore(Expression*);

	Expression* getLess() const;
	void setLess(Expression*);

	void accept(TreeVisitor&) override;
private:
private:
	decimal number;
	Expression* more;
	Expression* less;
};

#endif // INTERVALEXPRESSION_H
