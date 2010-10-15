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
    arguments=NULL;
    children=NULL;
}

Instance::~Instance()
{
    if(arguments)
	for(int i=0; i<arguments->size(); i++)
	    delete arguments->at(i);
    delete arguments;

    if(children)
	for(int i=0; i<children->size(); i++)
	    delete children->at(i);
    delete children;
}

void Instance::setName(QString name)
{
    this->name = name;
}

QString Instance::getName()
{
    return this->name;
}

void Instance::setArguments(QVector<Argument*>* args)
{
    this->arguments = args;
}

QVector<Argument*>* Instance::getArguments()
{
    return this->arguments;
}

void Instance::setChildren(QVector <Instance*>* childs)
{
    this->children = childs;
}

QVector <Instance*>* Instance::getChildren()
{
    return this->children;
}

QString Instance::toString()
{
    QString result;
    result.append("Instance: ");
    result.append(this->name);
    result.append(" ");
    if(arguments)
	for(int i=0; i<arguments->size(); i++)
	    result.append(arguments->at(i)->toString());

    if(children)
    {
	result.append("Children: ( ");
	for(int i=0; i<children->size(); i++)
	    result.append(children->at(i)->toString());
	result.append(")");
    }

    return result;
}
