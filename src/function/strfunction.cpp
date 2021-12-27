/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2021 Giles Bathgate
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

#include "strfunction.h"
#include "context.h"
#include "context.h"
#include "textvalue.h"

StrFunction::StrFunction() : Function("str")
{
	addDescription(tr("Converts the value to a text value."));
}

Value& StrFunction::evaluate(const Context& ctx) const
{
	TextValue* result=nullptr;
	for(const auto& arg: ctx.getArguments()) {
		Value* argVal=arg.getValue();
		TextValue& textArg=argVal->toText();
		if(!result) {
			result=&textArg;
		} else {
			Value& res=Value::evaluate(*result,Operators::Concatenate,textArg);
			result=&res.toText();
		}
	}

	if(result)
		return *result;

	return Value::factory.createUndefined();
}
