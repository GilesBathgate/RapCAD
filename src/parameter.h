/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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

#ifndef PARAMETER_H
#define PARAMETER_H
#include <QString>
#include "expression.h"
#include "visitabletree.h"

class Parameter : public VisitableTree
{
public:
	Parameter();
	~Parameter();
	QString getName() const;
	void setName(QString);
	Expression* getExpression() const;
	void setExpression(Expression*);
	void addDescription(QString);
	QString getDescription() const;
	void accept(TreeVisitor&);
private:
	QString name;
	QString description;
	Expression* expression;
};

#endif // PARAMETER_H
