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

#include "lengthfunction.h"
#include "context.h"
#include "vectorvalue.h"
#include "textvalue.h"
#include "numbervalue.h"
#include "rangevalue.h"

LengthFunction::LengthFunction() : Function("len")
{
	addDescription(tr("Returns the length of a range, list or text value."));
	addParameter("value");
}

Value* LengthFunction::evaluate(const Context& ctx) const
{
	Value* v=getParameterArgument(ctx,0);

	auto* rngVal=dynamic_cast<RangeValue*>(v);
	if(rngVal) {
		return Value::operation(rngVal,Expression::Length);
	}

	auto* vecVal=dynamic_cast<VectorValue*>(v);
	if(vecVal) {
		return new NumberValue(vecVal->getChildren().count());
	}

	auto* txtVal=dynamic_cast<TextValue*>(v);
	if(txtVal) {
		return Value::operation(txtVal,Expression::Length);
	}
	return Value::undefined();
}
