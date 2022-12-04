/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2022 Giles Bathgate
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

#include "ismat4x4function.h"
#include "booleanvalue.h"
#include "context.h"
#include "numbervalue.h"
#include "vectorvalue.h"

IsMat4x4Function::IsMat4x4Function() : Function("is_mat4x4")
{
	addDescription(tr("Returns true when the given value is a 4x4 matrix."));
	addParameter("value","mat4x4",tr("The value to test."));
}

Value& IsMat4x4Function::evaluate(const Context& ctx) const
{
	auto* matVal=getParameterArgument<VectorValue>(ctx,0);
	if(matVal) {
		const QList<Value*> rows=matVal->getElements();
		if(rows.count()==4) {
			for(Value* c: rows) {
				auto* rowVal=dynamic_cast<VectorValue*>(c);
				if(rowVal) {
					const QList<Value*> cols=rowVal->getElements();
					if(cols.count()==4) {
						for(Value* v: cols) {
							auto* numVal=dynamic_cast<NumberValue*>(v);
							if(!numVal) {
								return ValueFactory::createBoolean(false);
							}
						}
						return ValueFactory::createBoolean(true);
					}
				}
			}
		}
	}

	return ValueFactory::createBoolean(false);
}
