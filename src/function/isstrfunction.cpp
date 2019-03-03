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

#include "isstrfunction.h"
#include "textvalue.h"
#include "booleanvalue.h"

IsStrFunction::IsStrFunction() : Function("is_str")
{
	addDescription(tr("Returns true when the given value is text."));
	addParameter("value");
}

Value* IsStrFunction::evaluate(const Context& ctx) const
{
	auto* text=dynamic_cast<TextValue*>(getParameterArgument(ctx,0));
	if(text)
		return new BooleanValue(true);

	return new BooleanValue(false);
}
