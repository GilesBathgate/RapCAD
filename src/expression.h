/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010  Giles Bathgate
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

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QString>

class Expression
{
public:
    enum Operator_e
    {
	Multiply,
	Divide,
	Modulus,
        Dot,
	Add,
	Subtract,
	LessThan,
	LessOrEqual,
	Equal,
	NotEqual,
	GreaterOrEqual,
	GreaterThan,
	LogicalAnd,
        LogicalOr,
        Invert
    };

    Expression();
    virtual ~Expression();
    Operator_e getOp();
    void setOp(Operator_e);
    virtual QString toString();
protected:
    QString getOpString();
private:
    Operator_e op;
};

#endif // EXPRESSION_H
