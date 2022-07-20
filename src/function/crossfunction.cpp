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

#include "crossfunction.h"
#include "context.h"
#include "vectorvalue.h"

CrossFunction::CrossFunction() : Function("cross")
{
	addDescription(tr("Returns the cross product of the two vectors."));
	addParameter("v1");
	addParameter("v2");
}

Value& CrossFunction::evaluate(const Context& ctx) const
{

	auto* vec1=dynamic_cast<VectorValue*>(getParameterArgument(ctx,0));
	auto* vec2=dynamic_cast<VectorValue*>(getParameterArgument(ctx,1));
	if(vec1&&vec2)
		return Value::evaluate(*vec1,Operators::CrossProduct,*vec2);

	return ValueFactory::createUndefined();
}
