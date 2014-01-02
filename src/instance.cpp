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

#include "instance.h"

Instance::Instance()
{
	type = Default;
}

Instance::~Instance()
{
	foreach(Argument* a,arguments)
		delete a;

	foreach(Statement* s,children)
		delete s;
}

void Instance::setName(QString name)
{
	this->name = name;
}

QString Instance::getName() const
{
	return this->name;
}

void Instance::setArguments(QList<Argument*> args)
{
	this->arguments = args;
}

QList<Argument*> Instance::getArguments() const
{
	return this->arguments;
}

void Instance::setChildren(QList <Statement*> childs)
{
	this->children = childs;
}

void Instance::setType(Type_e t)
{
	this->type = t;
}

Instance::Type_e Instance::getType() const
{
	return this->type;
}

QList <Statement*> Instance::getChildren() const
{
	return this->children;
}

void Instance::setNamespace(QString name)
{
	this->name_space = name;
}

QString Instance::getNamespace() const
{
	return this->name_space;
}

void Instance::accept(TreeVisitor& v)
{
	v.visit(this);
}
