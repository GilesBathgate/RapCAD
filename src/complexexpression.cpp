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

#include "complexexpression.h"

ComplexExpression::ComplexExpression() :
	real(nullptr),
	imaginary(nullptr)
{
}

ComplexExpression::~ComplexExpression()
{
	delete real;
	delete imaginary;
}

Expression* ComplexExpression::getReal() const
{
	return real;
}

void ComplexExpression::setReal(Expression* value)
{
	real = value;
}

VectorExpression* ComplexExpression::getImaginary() const
{
	return imaginary;
}

void ComplexExpression::setImaginary(VectorExpression* value)
{
	imaginary = value;
}

void ComplexExpression::accept(TreeVisitor& v)
{
	v.visit(*this);
}
