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

#include "instance.h"

Instance::Instance()
{
	type = Default;
}

Instance::~Instance()
{
	for(Argument* a: arguments)
		delete a;

	for(Statement* s: children)
		delete s;
}

void Instance::setName(const QString& n)
{
	name = n;
}

QString Instance::getName() const
{
	return name;
}

void Instance::setArguments(const QList<Argument*>& args)
{
	arguments = args;
}

QList<Argument*> Instance::getArguments() const
{
	return arguments;
}

void Instance::setChildren(const QList <Statement*>& childs)
{
	children = childs;
}

void Instance::setType(Type_e t)
{
	type = t;
}

Instance::Type_e Instance::getType() const
{
	return type;
}

QList <Statement*> Instance::getChildren() const
{
	return children;
}

void Instance::setNamespace(const QString& name)
{
	name_space = name;
}

QString Instance::getNamespace() const
{
	return name_space;
}

void Instance::accept(TreeVisitor& v)
{
	v.visit(*this);
}
