/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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

#include "maxfunction.h"
#include "context.h"

MaxFunction::MaxFunction() : Function("max")
{
	addDescription(tr("Returns the largest of the given values."));
	addParameter("values","list",tr("The values for which to find the maximum value."));
}

Value& MaxFunction::evaluate(const Context& ctx) const
{
	QList<Value*> values=ctx.getArgumentValues();

	return Value::compareAll(values,Operators::GreaterThan);
}
