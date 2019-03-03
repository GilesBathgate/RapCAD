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

#include "atan2function.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"

Atan2Function::Atan2Function() : Function("atan2")
{
	addDescription(tr("Returns the arc tangent of the two number values."));
	addParameter("y");
	addParameter("x");
}

Value* Atan2Function::evaluate(const Context& ctx) const
{
	auto* yVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	auto* xVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,1));
	if(yVal&&xVal) {
		decimal y=yVal->getNumber();
		decimal x=xVal->getNumber();

		return new NumberValue(r_atan2_deg(y,x));
	}
	return Value::undefined();
}
