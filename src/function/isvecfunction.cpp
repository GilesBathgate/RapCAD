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

#include "isvecfunction.h"
#include "vectorvalue.h"
#include "booleanvalue.h"
#include "context.h"

IsVecFunction::IsVecFunction(int s) :
	Function(QString("is_vec%1").arg(s)),
	size(s)
{
	addDescription(tr("Returns true when the given value is a %1 vector.").arg(s));
	addParameter("value");
}

Value* IsVecFunction::evaluate(const Context& ctx) const
{
	auto* vec=dynamic_cast<VectorValue*>(getParameterArgument(ctx,0));
	if(!vec) {
		return new BooleanValue(false);
	} else {
		QList<Value*> vals=vec->getChildren();
		if(vals.count()!=size) {
			return new BooleanValue(false);
		} else {
			for(Value* v: vals) {
				auto* numVal=dynamic_cast<NumberValue*>(v);
				if(!numVal) {
					return new BooleanValue(false);
				}
			}
		}
	}

	return new BooleanValue(true);
}
