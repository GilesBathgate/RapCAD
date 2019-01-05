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

#ifndef COMPOUNDSTATEMENT_H
#define COMPOUNDSTATEMENT_H

#include <QList>
#include "scope.h"

class CompoundStatement : public Scope
{
public:
	CompoundStatement();
	~CompoundStatement() override;
	void setChildren(const QList<Statement*>&);
	QList<Statement*> getChildren() const;
	void accept(TreeVisitor&) override;
private:
	QList<Statement*> children;
};

#endif // COMPOUNDSTATEMENT_H
