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

#include "function/componentwisefunction.h"
#include "context.h"
#include "numbervalue.h"
#include "valuefactory.h"
#include "vectorvalue.h"

ComponentWiseFunction::ComponentWiseFunction(const QString& n) :
	Function(n)
{
}

Value& ComponentWiseFunction::evaluate(const Context& ctx) const
{
	Value* val=getParameterArgument<Value>(ctx,0);
	if(val)
		return descend(*val,ctx);

	return ValueFactory::createUndefined();
}

Value& ComponentWiseFunction::descend(Value& val,const Context& ctx) const
{
	auto* numVal=dynamic_cast<NumberValue*>(&val);
	if(numVal)
		return evaluate(*numVal,ctx);

	auto* vecVal=dynamic_cast<VectorValue*>(&val);
	if(vecVal) {
		QList<Value*> result;
		for(auto* c : vecVal->getElements()) {
			if(c) {
				result.append(&descend(*c,ctx));
			} else {
				result.append(&ValueFactory::createUndefined());
			}
		}
		return ValueFactory::createVector(result);
	}

	return ValueFactory::createUndefined();
}
