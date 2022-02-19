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

#ifndef INSTANCE_H
#define INSTANCE_H

#include "argument.h"
#include "statement.h"
#include <QList>
#include <QString>

enum class InstanceTypes {
	Default,
	Root,
	Debug,
	Background,
	Disable,
	Auxilary
};

class Instance : public Statement
{
public:
	Instance();
	~Instance() override;
	void setName(const QString&);
	QString getName() const;
	void setArguments(const QList<Argument*>&);
	const QList<Argument*> getArguments() const;
	void setChildren(const QList <Statement*>&);
	const QList <Statement*> getChildren() const;
	void setType(InstanceTypes);
	InstanceTypes getType() const;
	void setNamespace(const QString&);
	QString getNamespace() const;
	void accept(TreeVisitor&) override;
private:
	QString name;
	QList<Argument*> arguments;
	QList<Statement*> children;
	InstanceTypes type;
	QString nameSpace;
};

#endif // INSTANCE_H
