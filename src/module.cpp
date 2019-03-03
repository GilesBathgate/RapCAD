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

#include "module.h"
#include "context.h"

Module::Module(Reporter& r, const QString& n) :
	auxilary(false),
	reporter(r),
	name(n),
	deprecated(false),
	scope(nullptr)
{
}

Module::~Module()
{
	for(Parameter* p: parameters)
		delete p;

	delete scope;
}

QString Module::getName() const
{
	return name;
}

void Module::setName(const QString& n)
{
	name=n;
}

QString Module::getDescription() const
{
	return description;
}

bool Module::getAuxilary() const
{
	return auxilary;
}

QList<Parameter*> Module::getParameters() const
{
	return parameters;
}

void Module::setParameters(const QList<Parameter*>& params)
{
	parameters=params;
}

void Module::setScope(Scope* scp)
{
	scope=scp;
}

Scope* Module::getScope() const
{
	return scope;
}

void Module::accept(TreeVisitor& v)
{
	v.visit(*this);
}

Node* Module::evaluate(const Context&) const
{
	return nullptr;
}

void Module::addDescription(const QString& d)
{
	description=d;
}

void Module::addDeprecated(const QString& d)
{
	deprecated=true;
	description=d;
}

void Module::addParameter(const QString& name, const QString& desc)
{
	auto* p=new Parameter();
	p->setName(name);
	p->addDescription(desc);
	parameters.append(p);
}

Value* Module::getParameterArgument(const Context& ctx, int index) const
{
	return getParameterArgument(ctx,index,index);
}

Value* Module::getParameterArgument(const Context& ctx, int index, int expectedIndex) const
{
	Parameter* p=parameters.at(index);
	return ctx.getArgument(expectedIndex,p->getName());
}

bool Module::isDeprecated() const
{
	return deprecated;
}
