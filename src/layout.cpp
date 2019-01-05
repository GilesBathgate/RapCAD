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

#include "layout.h"

Layout::Layout(Reporter& r) :
	reporter(r),
	parent(nullptr),
	scope(nullptr)
{
}

Layout::~Layout()
{
}

void Layout::setParent(Layout* value)
{
	parent=value;
}

const Module* Layout::lookupModule(const QString& name,bool aux) const
{
	if(modules.contains(name)) {
		const Module* m=modules.value(name);
		if(m->getAuxilary()==aux)
			return m;
	} else if(parent) {
		return parent->lookupModule(name,aux);
	}

	return nullptr;
}

const Function* Layout::lookupFunction(const QString& name) const
{
	if(functions.contains(name)) {
		return functions.value(name);
	} else if(parent) {
		return parent->lookupFunction(name);
	}

	return nullptr;
}

void Layout::addModule(const Module& mod)
{
	QString name=mod.getName();
	if(modules.contains(name)) {
		reporter.reportWarning(tr("module '%1' was already defined.").arg(name));
		return;
	}

	modules.insert(name,&mod);
}

void Layout::addFunction(const Function& func)
{
	QString name=func.getName();
	if(functions.contains(name)) {
		reporter.reportWarning(tr("function '%1' was already defined.").arg(name));
		return;
	}

	functions.insert(name,&func);
}

void Layout::setScope(Scope* sc)
{
	scope=sc;
}

bool Layout::inScope(Scope* sc) const
{
	if(sc==scope)
		return true;
	else if(parent)
		return parent->inScope(sc);

	return false;
}
