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

#include "vectorexpression.h"

VectorExpression::VectorExpression() :
	additionalCommas(0)
{
}

VectorExpression::~VectorExpression()
{
	qDeleteAll(children);
	children.clear();
}

void VectorExpression::setChildren(const QList<Expression*>& exp)
{
	children = exp;
}

const QList<Expression*>& VectorExpression::getChildren() const
{
	return children;
}

void VectorExpression::setAdditionalCommas(int count)
{
	additionalCommas=count;
}

int VectorExpression::getAdditionalCommas() const
{
	return additionalCommas;
}

void VectorExpression::accept(TreeVisitor& v)
{
	v.visit(*this);
}
