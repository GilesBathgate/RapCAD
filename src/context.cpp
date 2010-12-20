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

#include "context.h"
#include "modulescope.h"

Context::Context()
{
    parent=NULL;
    currentvalue=NULL;
    currentscope=NULL;
}

Module* Context::lookupmodule(QString name)
{
    if(!modules.contains(name))
    {
	foreach(Declaration* d,currentscope->getDeclarations())
	{
	    Module* mod = dynamic_cast<Module*>(d);
	    if(mod && mod->getName() == name)
	    {
		modules.insert(name,mod);
		break;
	    }
	}
	if(parent)
	    return parent->lookupmodule(name);
    }

    return modules.value(name);
}

Function* Context::lookupfunction(QString name)
{
    if(!functions.contains(name))
    {
	foreach(Declaration* d,currentscope->getDeclarations())
	{
	    Function* func = dynamic_cast<Function*>(d);
	    if(func && func->getName() == name)
	    {
		functions.insert(name,func);
		break;
	    }
	}
    }

    return functions.value(name);
}

void Context::addmodule(Module *mod)
{
    modules.insert(mod->getName(),mod);
}

void Context::args(QVector<Value*> args, QVector<Value*> params)
{
	for (int i=0; i<params.size(); i++) {
		variables.insert(args.at(i)->getName(), args.at(i));
	}
}
