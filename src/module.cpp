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

#include "module.h"

Module::Module()
{
	scope=NULL;
}

Module::~Module()
{
	for(int i=0; i<parameters.size(); i++)
		delete parameters.at(i);

	delete scope;
}

QString Module::getName()
{
	return this->name;
}

void Module::setName(QString name)
{
	this->name = name;
}


QVector<Parameter*> Module::getParameters()
{
	return this->parameters;
}

void Module::setParameters(QVector<Parameter*> params)
{
	this->parameters = params;
}

void Module::setScope(Scope * scp)
{
	this->scope = scp;
}

Scope* Module::getScope()
{
	return this->scope;
}

void Module::accept(Visitor& v)
{
	v.visit(this);
}

void Module::evaluate(Context*,Instance*)
{
}
