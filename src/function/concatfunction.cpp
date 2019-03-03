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

#include "concatfunction.h"
#include "context.h"
#include "vectorvalue.h"

ConcatFunction::ConcatFunction() : Function("concat")
{
	addDescription(tr("Returns a vector with the passed in vector appended."));
}

Value* ConcatFunction::evaluate(const Context& ctx) const
{
	VectorValue* val=nullptr;
	for(auto arg: ctx.getArguments()) {
		Value* argVal = arg.second;
		auto* vecVal = dynamic_cast<VectorValue*>(argVal);
		if(!vecVal)
			vecVal=argVal->toVector(1);

		if(!val) {
			val=vecVal;
		} else {
			Value* res=Value::operation(val,Expression::Concatenate,vecVal);
			val=res->toVector(1);
		}
	}

	return val;
}
