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
#ifndef INVOCATION_H
#define INVOCATION_H

#include <QString>
#include <QList>
#include "expression.h"
#include "argument.h"

class Invocation : public Expression
{
public:
	Invocation();
	~Invocation() override;
	void setName(const QString&);
	QString getName() const;
	void setNamespace(const QString&);
	QString getNamespace() const;
	void setArguments(const QList<Argument*>&);
	QList<Argument*> getArguments() const;
	void accept(TreeVisitor&) override;
private:
	QString name;
	QString nameSpace;
	QList<Argument*> arguments;
};

#endif // INVOCATION_H
