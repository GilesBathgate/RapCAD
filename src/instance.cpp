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

#include "instance.h"

Instance::Instance()
{
    type = Default;
}

Instance::~Instance()
{
    for(int i=0; i<arguments.size(); i++)
	delete arguments.at(i);

    for(int i=0; i<children.size(); i++)
	delete children.at(i);
}

void Instance::setName(QString name)
{
    this->name = name;
}

QString Instance::getName()
{
    return this->name;
}

void Instance::setArguments(QVector<Argument*> args)
{
    this->arguments = args;
}

QVector<Argument*> Instance::getArguments()
{
    return this->arguments;
}

void Instance::setChildren(QVector <Statement*> childs)
{
    this->children = childs;
}

void Instance::setType(Type_e t)
{
    this->type = t;
}

Instance::Type_e Instance::getType()
{
    return this->type;
}

QVector <Statement*> Instance::getChildren()
{
    return this->children;
}

void Instance::accept(Visitor *v)
{
    v->visit(this);
}
