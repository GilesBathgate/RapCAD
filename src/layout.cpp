/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2013 Giles Bathgate
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

#include "layout.h"

Layout::Layout(QTextStream& s) : output(s)
{
	parent=NULL;
	scope=NULL;
}

void Layout::setParent(Layout* value)
{
	parent=value;
}

Module* Layout::lookupModule(QString name)
{
	if(modules.contains(name)) {
		return modules.value(name);
	} else if(parent) {
		return parent->lookupModule(name);
	}

	return NULL;
}

Function* Layout::lookupFunction(QString name)
{
	if(functions.contains(name)) {
		return functions.value(name);
	} else if(parent) {
		return parent->lookupFunction(name);
	}

	return NULL;
}

void Layout::addModule(Module* mod)
{
	QString name=mod->getName();
	if(modules.contains(name)) {
		output << "Warning: module '" << name << "' was already defined.\n";
		return;
	}

	modules.insert(name,mod);
}

void Layout::addFunction(Function* func)
{
	QString name=func->getName();
	if(functions.contains(name)) {
		output << "Warning: function '" << name << "' was already defined.\n";
		return;
	}

	functions.insert(name,func);
}

void Layout::setScope(Scope* sc)
{
	scope=sc;
}

bool Layout::inScope(Scope* sc)
{
	if(sc==scope)
		return true;
	else if(parent)
		return parent->inScope(sc);

	return false;
}
