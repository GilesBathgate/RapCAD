/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2022 Giles Bathgate
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
#include "context.h"

Function::Function() :
	scope(nullptr)
{
}

Function::Function(const QString& n) :
	scope(nullptr),
	name(n)
{
}

Function::~Function()
{
	qDeleteAll(parameters);
	parameters.clear();
	delete scope;
}

QString Function::getName() const
{
	return name;
}

void Function::setName(const QString& n)
{
	name = n;
}

const QList<Parameter*> Function::getParameters() const
{
	return parameters;
}

void Function::setParameters(const QList<Parameter*>& params)
{
	parameters = params;
}

void Function::setScope(Scope* scp)
{
	scope = scp;
}

Scope* Function::getScope() const
{
	return scope;
}

void Function::accept(TreeVisitor& v)
{
	v.visit(*this);
}

Value& Function::evaluate(const Context&) const
{
	return ValueFactory::createUndefined();
}

void Function::addParameter(const QString& n,const QString& t,const QString& d)
{
	auto* p=new Parameter();
	p->setName(n);
	p->setType(t);
	p->addDescription(d);
	parameters.append(p);
}

Value* Function::getParameterArgument(const Context& ctx, int index) const
{
	Parameter* p = parameters.at(index);
	return ctx.getArgument(index,p->getName());
}

QString Function::getDescription() const
{
	return description;
}

void Function::addDescription(const QString& value)
{
	description = value;
}
