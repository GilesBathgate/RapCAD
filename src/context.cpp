/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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

#include "context.h"
#include "modulescope.h"

Context::Context()
{
	parent=NULL;
	currentValue=NULL;
	currentScope=NULL;
}

Module* Context::lookupModule(QString name)
{
	if(!modules.contains(name)) {
		foreach(Declaration* d,currentScope->getDeclarations()) {
			Module* mod = dynamic_cast<Module*>(d);
			if(mod && mod->getName() == name) {
				modules.insert(name,mod);
				break;
			}
		}
		if(parent)
			return parent->lookupModule(name);
	}

	return modules.value(name);
}

Function* Context::lookupFunction(QString name)
{
	if(!functions.contains(name)) {
		foreach(Declaration* d,currentScope->getDeclarations()) {
			Function* func = dynamic_cast<Function*>(d);
			if(func && func->getName() == name) {
				functions.insert(name,func);
				break;
			}
			if(parent)
				return parent->lookupFunction(name);
		}
	}

	return functions.value(name);
}

void Context::addVariable(Value* v)
{
	variables.insert(v->getName(),v);
}

Value* Context::lookupVariable(QString name)
{
	if(variables.contains(name))
		return variables.value(name);
	else if(parent)
		return parent->lookupVariable(name);
	else
		return new Value(); //undef
}

void Context::addModule(Module* mod)
{
	modules.insert(mod->getName(),mod);
}

void Context::addFunction(Function* func)
{
	functions.insert(func->getName(),func);
}

void Context::setArguments(QVector<Value*> args, QVector<Value*> params)
{
	for(int i=0; i<params.size(); i++) {
		QString name = args.at(i)->getName();
		if(name.isEmpty() || contains(params,name))
			variables.insert(params.at(i)->getName(), args.at(i));
	}
}

bool Context::contains(QVector<Value*> params,QString name)
{
	foreach(Value* p, params)
		if(p->getName() == name)
			return true;
	return false;
}
