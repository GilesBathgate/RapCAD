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

#include "function/signfunction.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"
#include "valuefactory.h"

SignFunction::SignFunction() : ComponentWiseFunction("sign")
{
	addDescription(tr("Returns a value of -1 or 1 depending on the sign of the given value."));
	addParameter("value","num|list",tr("The value for which to find the sign."));
}

Value& SignFunction::evaluate(NumberValue& val,const Context&) const
{
	return ValueFactory::createNumber(r_sign(val.getNumber()));
}
