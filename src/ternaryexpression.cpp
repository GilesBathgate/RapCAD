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

#include "ternaryexpression.h"

TernaryExpression::TernaryExpression() :
	condition(nullptr),
	trueExpression(nullptr),
	falseExpression(nullptr)
{
}

TernaryExpression::~TernaryExpression()
{
	delete condition;
	delete trueExpression;
	delete falseExpression;
}

void TernaryExpression::setCondition(Expression* exp)
{
	condition = exp;
}

Expression* TernaryExpression::getCondition() const
{
	return condition;
}

void TernaryExpression::setTrueExpression(Expression* exp)
{
	trueExpression = exp;
}

Expression* TernaryExpression::getTrueExpression() const
{
	return trueExpression;
}

void TernaryExpression::setFalseExpression(Expression* exp)
{
	falseExpression = exp;
}

Expression* TernaryExpression::getFalseExpression() const
{
	return falseExpression;
}

void TernaryExpression::accept(TreeVisitor& v)
{
	v.visit(*this);
}
