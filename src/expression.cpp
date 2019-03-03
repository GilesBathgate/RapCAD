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

#include "expression.h"

Expression::Expression() :
	lineNumber(0),
	op(None)
{
}

Expression::~Expression()
{
}

QString Expression::getOpString() const
{
	QString result;
	switch(getOp()) {
		case Exponent:
			result.append('^');
			break;
		case DotProduct:
		case Multiply:
			result.append('*');
			break;
		case Concatenate:
			result.append("~");
			break;
		case Append:
			result.append("~=");
			break;
		case ComponentwiseMultiply:
			result.append(".*");
			break;
		case Divide:
			result.append('/');
			break;
		case ComponentwiseDivide:
			result.append("./");
			break;
		case Increment:
			result.append("++");
			break;
		case Decrement:
			result.append("--");
			break;
		case CrossProduct:
			result.append("**");
			break;
		case Modulus:
			result.append('%');
			break;
		case Dot:
			result.append('.');
			break;
		case Add:
			result.append('+');
			break;
		case Subtract:
			result.append('-');
			break;
		case LessThan:
			result.append('<');
			break;
		case LessOrEqual:
			result.append("<=");
			break;
		case Equal:
			result.append("==");
			break;
		case NotEqual:
			result.append("!=");
			break;
		case GreaterOrEqual:
			result.append(">=");
			break;
		case GreaterThan:
			result.append('>');
			break;
		case LogicalAnd:
			result.append("&&");
			break;
		case LogicalOr:
			result.append("||");
			break;
		case Invert:
			result.append('!');
			break;
		case Index:
			result.append("[]");
			break;
		case AddAssign:
			result.append("+=");
			break;
		case SubAssign:
			result.append("-=");
			break;
		case Length:
			result.append("|");
		case None:
			break;
	}
	return result;
}

bool Expression::postFix() const
{
	return op==Increment||op==Decrement;
}

void Expression::setOp(Operator_e o)
{
	op = o;
}

Expression::Operator_e Expression::getOp() const
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
