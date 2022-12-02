/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2022 Giles Bathgate
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

#include "logfunction.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"

LogFunction::LogFunction() : Function("log")
{
	addDescription(tr("Returns the logarithm of the number value to the given base."));
	addParameter("value","num",tr("The value for which to find the log."));
	addParameter("base","num",tr("The base."));
}

Value& LogFunction::evaluate(const Context& ctx) const
{
	auto* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	auto* baseVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,1));

	if(numVal) {
		decimal num=numVal->getNumber();

		if(baseVal) {
			decimal base=baseVal->getNumber();

			return ValueFactory::createNumber(r_log(num)/r_log(base));
		}

		return ValueFactory::createNumber(r_log10(num));
	}

	return ValueFactory::createUndefined();
}
