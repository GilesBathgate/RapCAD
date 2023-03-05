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

#include "isintfunction.h"
#include "booleanvalue.h"
#include "numbervalue.h"
#include "rmath.h"
#include "valuefactory.h"

IsIntFunction::IsIntFunction() : Function("is_int")
{
	addDescription(tr("Returns true when the given value is an integer."));
	addParameter("value","int",tr("The value to test."));
}

Value& IsIntFunction::evaluate(const Context& ctx) const
{
	auto* num=getParameterArgument<NumberValue>(ctx,0);
	if(num) {
		if(r_is_int(num->getNumber()))
			return ValueFactory::createBoolean(true);
	}

	return ValueFactory::createBoolean(false);
}
