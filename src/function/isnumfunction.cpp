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

#include "isnumfunction.h"
#include "numbervalue.h"
#include "booleanvalue.h"

IsNumFunction::IsNumFunction() : Function("is_num")
{
	addDescription(tr("Returns true when the given value is a number."));
	addParameter("value");
}

Value* IsNumFunction::evaluate(const Context& ctx) const
{
	auto* num=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(num)
		return new BooleanValue(true);

	return new BooleanValue(false);
}
