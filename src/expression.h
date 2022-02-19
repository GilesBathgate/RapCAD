/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2021 Giles Bathgate
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

#include "operators.h"
#include "visitabletree.h"
#include <QString>

class Expression : public VisitableTree
{
public:


	Expression();
	~Expression() override = default;

	Operators getOp() const;
	void setOp(Operators);
	QString getOpString() const;
	bool postFix() const;

	int getLineNumber() const;
	void setLineNumber(int value);
private:
	int lineNumber;
	Operators op;
};

#endif // EXPRESSION_H
