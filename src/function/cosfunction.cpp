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

#include "cosfunction.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"
#include "valuefactory.h"

CosFunction::CosFunction() : Function("cos")
{
	addDescription(tr("Returns the cosine of the number value."));
	addParameter("value","num",tr("The value for which to find the cosine."));
}

Value& CosFunction::evaluate(const Context& ctx) const
{
	auto* numVal=getParameterArgument<NumberValue>(ctx,0);
	if(numVal) {
		const decimal& num=numVal->getNumber();

		return ValueFactory::createNumber(r_cos_deg(num));
	}
	return ValueFactory::createUndefined();
}
