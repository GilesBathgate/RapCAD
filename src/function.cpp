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

#include "function.h"
#include "functionscope.h"
#include "context.h"

Function::Function()
{
	scope=NULL;
}

Function::Function(QString n)
{
	scope=NULL;
	name=n;
}

Function::~Function()
{
	foreach(Parameter* p, parameters)
		delete p;

	delete scope;
}

QString Function::getName() const
{
	return this->name;
}

void Function::setName(QString name)
{
	this->name = name;
}


QList<Parameter*> Function::getParameters() const
{
	return this->parameters;
}

void Function::setParameters(QList<Parameter*> params)
{
	this->parameters = params;
}

void Function::setScope(Scope* scp)
{
	this->scope = scp;
}

Scope* Function::getScope() const
{
	return this->scope;
}

void Function::accept(TreeVisitor& v)
{
	v.visit(this);
}

Value* Function::evaluate(Context*)
{
	return NULL;
}

void Function::addParameter(QString name)
{
	Parameter* p = new Parameter();
	p->setName(name);
	parameters.append(p);
}

Value* Function::getParameterArgument(Context* ctx, int index)
{
	Parameter* p = parameters.at(index);
	return ctx->getArgument(index,p->getName());
}
