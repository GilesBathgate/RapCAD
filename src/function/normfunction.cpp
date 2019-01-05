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

#include "normfunction.h"
#include "context.h"
#include "vectorvalue.h"
#include "rangevalue.h"
#include "numbervalue.h"
#include "complexvalue.h"
#include "rmath.h"

NormFunction::NormFunction() : Function("norm")
{
	addDescription(tr("Returns the strictly positive length of a given number, vector or complex value."));
	addParameter("value");
}

Value* NormFunction::evaluate(const Context& ctx) const
{
	Value* v=getParameterArgument(ctx,0);

	/* Explicitly return undefined for range which
	 * inherits from vector */
	auto* rng=dynamic_cast<RangeValue*>(v);
	if(rng)
		return Value::undefined();

	auto* vec=dynamic_cast<VectorValue*>(v);
	if(vec)
		return Value::operation(vec,Expression::Length);

	auto* num=dynamic_cast<NumberValue*>(v);
	if(num)
		return Value::operation(num,Expression::Length);

	auto* cpx=dynamic_cast<ComplexValue*>(v);
	if(cpx)
		return Value::operation(cpx,Expression::Length);

	return Value::undefined();
}
