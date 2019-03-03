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

#include "chrfunction.h"
#include "context.h"
#include "numbervalue.h"
#include "textvalue.h"

ChrFunction::ChrFunction() : Function("chr")
{
	addDescription(tr("Returns a characters with the given unicode number values."));
}

Value* ChrFunction::evaluate(const Context& ctx) const
{
	QString result;
	for(auto arg: ctx.getArguments()) {
		Value* argVal = arg.second;
		auto* code=dynamic_cast<NumberValue*>(argVal);
		if(code) {
			uint point=code->toInteger();
			result.append(QString::fromUcs4(&point,1));
		}
	}

	return new TextValue(result);
}
