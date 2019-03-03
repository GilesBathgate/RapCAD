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

#include "importmodule.h"
#include "node/importnode.h"
#include "textvalue.h"
#include "context.h"

ImportModule::ImportModule(Reporter& r) : Module(r,"import")
{
	addDeprecated(tr("The import module is depricated please use the import declaration instead."));
	addParameter("file", tr("The name of the file to import."));
}

Node* ImportModule::evaluate(const Context& ctx) const
{
	auto* fileVal = dynamic_cast<TextValue*>(getParameterArgument(ctx,0));
	if(fileVal)
		return new ImportNode(fileVal->getValueString());

	return new ImportNode(import);
}

void ImportModule::setImport(const QString& imp)
{
	import = imp;
}
