/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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

#include "randfunction.h"
#include "context.h"
#include "vectorvalue.h"
#include "numbervalue.h"
#include "rmath.h"
#include <time.h>

RandFunction::RandFunction() : Function("rands")
{
	addParameter("min");
	addParameter("max");
	addParameter("count");
	addParameter("seed");
}

Value* RandFunction::evaluate(Context* ctx)
{
	decimal min=0;
	NumberValue* minVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(minVal)
		min=minVal->getNumber();
	decimal max=0;
	NumberValue* maxVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,1));
	if(maxVal)
		max=maxVal->getNumber();
	int count=1;
	NumberValue* countVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,2));
	if(countVal)
		count=countVal->toInteger();
	NumberValue* seedVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,3));
	int seed=time(0);
	if(seedVal)
		seed=seedVal->toInteger();

	QList<Value*> results;
	for(int i=0; i<count; ++i)
		results.append(new NumberValue(r_rand(seed,min,max)));

	return new VectorValue(results);
}
