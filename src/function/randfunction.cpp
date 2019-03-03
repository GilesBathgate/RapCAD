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

#include "randfunction.h"
#include "context.h"
#include "vectorvalue.h"
#include "numbervalue.h"
#include "rmath.h"
#include <time.h>

RandFunction::RandFunction() : Function("rands")
{
	addDescription(tr("Returns a vector of numbers between the given min and max values."));
	addParameter("min");
	addParameter("max");
	addParameter("count",tr("The quantity of random numbers to create"));
	addParameter("seed",tr("The seed number can be optionally used to give consistent results."));
}

Value* RandFunction::evaluate(const Context& ctx) const
{
	decimal min=0;
	auto* minVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(minVal)
		min=minVal->getNumber();
	decimal max=0;
	auto* maxVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,1));
	if(maxVal)
		max=maxVal->getNumber();
	int count=1;
	auto* countVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,2));
	if(countVal)
		count=countVal->toInteger();
	auto* seedVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,3));
	int seed=time(nullptr);
	if(seedVal)
		seed=seedVal->toInteger();

	r_rand_seed(seed);

	QList<Value*> results;
	for(auto i=0; i<count; ++i)
		results.append(new NumberValue(r_rand(min,max)));

	return new VectorValue(results);
}
