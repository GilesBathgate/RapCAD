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
    }

    return modules[name];
}

void Context::addmodule(Module *mod)
{
    modules.insert(mod->getName(),mod);
}
