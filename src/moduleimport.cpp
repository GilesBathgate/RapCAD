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

#include "moduleimport.h"

ModuleImport::ModuleImport()
{
}

ModuleImport::~ModuleImport()
{
}

void ModuleImport::setImport(const QString& imp)
{
	import = imp;
}

QString ModuleImport::getImport() const
{
	return import;
}

void ModuleImport::setName(const QString& n)
{
	name = n;
}

QString ModuleImport::getName() const
{
	return name;
}

void ModuleImport::setParameters(const QList<Parameter*>& params)
{
	parameters = params;
}

QList<Parameter*> ModuleImport::getParameters() const
{
	return parameters;
}

void ModuleImport::accept(TreeVisitor& v)
{
	v.visit(*this);
}
