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
#include "ordinalfunction.h"
#include "context.h"
#include "textvalue.h"
#include "numbervalue.h"

OrdinalFunction::OrdinalFunction() : Function("ord")
{
	addDescription(tr("Convert a character to a number representing the Unicode code point."));
	addParameter("value");
}

Value& OrdinalFunction::evaluate(const Context& ctx) const
{
	auto* textVal=dynamic_cast<TextValue*>(getParameterArgument(ctx,0));
	if(textVal) {
		const QVector<uint> codepoints=textVal->getValueString().toUcs4();
		QList<Value*> results;
		for(uint unicode: codepoints)
			results.append(&Value::factory.createNumber(unicode));
		if(results.count()==1)
			return *results.at(0);
		return Value::factory.createVector(results);
	}

	return Value::factory.createUndefined();
}
