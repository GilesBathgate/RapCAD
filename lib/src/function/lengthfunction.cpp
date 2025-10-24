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

#include "lengthfunction.h"
#include "context.h"
#include "rangevalue.h"
#include "textvalue.h"
#include "valuefactory.h"
#include "vectorvalue.h"

LengthFunction::LengthFunction() : Function("len")
{
	addDescription(tr("Returns the length of a range, list or text value."));
	addParameter("value","list",tr("The value to measure."));
}

Value& LengthFunction::evaluate(const Context& ctx) const
{
	auto* v=getParameterArgument<Value>(ctx,0);

	auto* rngVal=dynamic_cast<RangeValue*>(v);
	if(rngVal) {
		return Value::evaluate(*rngVal,Operators::Length);
	}

	auto* vecVal=dynamic_cast<VectorValue*>(v);
	if(vecVal) {
		return ValueFactory::createNumber(vecVal->getElements().count());
	}

	auto* txtVal=dynamic_cast<TextValue*>(v);
	if(txtVal) {
		return Value::evaluate(*txtVal,Operators::Length);
	}
	return ValueFactory::createUndefined();
}
