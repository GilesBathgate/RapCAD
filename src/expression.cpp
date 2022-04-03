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

#include "expression.h"

Expression::Expression() :
	lineNumber(0),
	op(Operators::None)
{
}

QString Expression::getOpString() const
{
	QString result;
	switch(getOp()) {
		case Operators::Exponent:
			result.append('^');
			break;
		case Operators::DotProduct:
		case Operators::Multiply:
			result.append('*');
			break;
		case Operators::Concatenate:
			result.append("~");
			break;
		case Operators::Append:
			result.append("~=");
			break;
		case Operators::ComponentwiseMultiply:
			result.append(".*");
			break;
		case Operators::Divide:
			result.append('/');
			break;
		case Operators::ComponentwiseDivide:
			result.append("./");
			break;
		case Operators::Increment:
			result.append("++");
			break;
		case Operators::Decrement:
			result.append("--");
			break;
		case Operators::CrossProduct:
			result.append("**");
			break;
		case Operators::Modulus:
			result.append('%');
			break;
		case Operators::Dot:
			result.append('.');
			break;
		case Operators::Add:
			result.append('+');
			break;
		case Operators::Subtract:
			result.append('-');
			break;
		case Operators::LessThan:
			result.append('<');
			break;
		case Operators::LessOrEqual:
			result.append("<=");
			break;
		case Operators::Equal:
			result.append("==");
			break;
		case Operators::NotEqual:
			result.append("!=");
			break;
		case Operators::GreaterOrEqual:
			result.append(">=");
			break;
		case Operators::GreaterThan:
			result.append('>');
			break;
		case Operators::LogicalAnd:
			result.append("&&");
			break;
		case Operators::LogicalOr:
			result.append("||");
			break;
		case Operators::Invert:
			result.append('!');
			break;
		case Operators::Index:
			result.append("[]");
			break;
		case Operators::AddAssign:
			result.append("+=");
			break;
		case Operators::SubAssign:
			result.append("-=");
			break;
		case Operators::Length:
			result.append("|");
		case Operators::None:
			break;
	}
	return result;
}

bool Expression::postFix() const
{
	return op==Operators::Increment||op==Operators::Decrement;
}

void Expression::setOp(Operators o)
{
	op = o;
}

Operators Expression::getOp() const
{
	return op;
}

int Expression::getLineNumber() const
{
	return lineNumber;
}

void Expression::setLineNumber(int value)
{
	lineNumber=value;
}
