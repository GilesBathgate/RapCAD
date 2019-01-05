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

#include "invocation.h"

Invocation::Invocation()
{
}

Invocation::~Invocation()
{
}

void Invocation::setName(const QString& n)
{
	name = n;
}

QString Invocation::getName() const
{
	return name;
}

void Invocation::setNamespace(const QString& name)
{
	nameSpace = name;
}

QString Invocation::getNamespace() const
{
	return nameSpace;
}

void Invocation::setArguments(const QList<Argument*>& args)
{
	arguments = args;
}

QList<Argument*> Invocation::getArguments() const
{
	return arguments;
}

void Invocation::accept(TreeVisitor& v)
{
	v.visit(*this);
}
