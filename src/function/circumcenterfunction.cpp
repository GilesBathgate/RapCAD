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

#include "circumcenterfunction.h"
#include "context.h"

CircumcenterFunction::CircumcenterFunction() : Function("circumcenter")
{
	addDescription(tr("Finds the circumcenter of the given points"));
	addParameter("a","vec3",tr("The first point."));
	addParameter("b","vec3",tr("The second point."));
	addParameter("c","vec3",tr("The third point."));
}

Value& CircumcenterFunction::evaluate(const Context& ctx) const
{
#if USE_CGAL
	auto* aVal=getParameterArgument<VectorValue>(ctx,0);
	auto* bVal=getParameterArgument<VectorValue>(ctx,1);
	auto* cVal=getParameterArgument<VectorValue>(ctx,2);
	if(aVal&&bVal&&cVal) {
		Point pa=aVal->getPoint();
		Point pb=bVal->getPoint();
		Point pc=cVal->getPoint();
		Point result=CGAL::circumcenter(pa,pb,pc);
		QList<Value*> resultVector;
		resultVector.append(&ValueFactory::createNumber(result.x()));
		resultVector.append(&ValueFactory::createNumber(result.y()));
		resultVector.append(&ValueFactory::createNumber(result.z()));
		return ValueFactory::createVector(resultVector);
	}
#endif
	return ValueFactory::createUndefined();
}


