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

#include "chrfunction.h"
#include "context.h"
#include "numbervalue.h"
#include "textvalue.h"
#include "valuefactory.h"

ChrFunction::ChrFunction() : Function("chr")
{
	addDescription(tr("Returns characters with the given unicode number values."));
	addParameter("values","list",tr("The values to convert to characters."));
}

Value& ChrFunction::evaluate(const Context& ctx) const
{
	QString result;
	auto* codes=getParameterArgument<VectorValue>(ctx,0);
	if(codes) {
		for(const auto& codeVal: codes->getElements()) {
			auto* code=dynamic_cast<NumberValue*>(codeVal);
			if(code) {
				const char32_t unicode=code->toInteger();
				result.append(QString::fromUcs4(&unicode,1));
			}
		}
	} else {
		for(const auto& arg: ctx.getArguments()) {
			Value* argVal = arg.getValue();
			auto* code=dynamic_cast<NumberValue*>(argVal);
			if(code) {
				const char32_t unicode=code->toInteger();
				result.append(QString::fromUcs4(&unicode,1));
			}
		}
	}

	return ValueFactory::createText(result);
}
