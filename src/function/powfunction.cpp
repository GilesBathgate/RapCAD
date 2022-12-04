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

#include "powfunction.h"
#include "context.h"

PowFunction::PowFunction() : Function("pow")
{
	addDescription(tr("Returns the value raised to the power of exponent."));
	addParameter("value","num",tr("The value for which to find the power."));
	addParameter("exponent","num",tr("The exponent."));
}

Value& PowFunction::evaluate(const Context& ctx) const
{
	auto* val=getParameterArgument<Value>(ctx,0);
	auto* arg=getParameterArgument<Value>(ctx,1);
	if(val&&arg)
		return Value::evaluate(*val,Operators::Exponent,*arg);

	return ValueFactory::createUndefined();
}
