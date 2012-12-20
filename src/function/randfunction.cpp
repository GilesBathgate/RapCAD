/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2012 Giles Bathgate
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
#include "vectorvalue.h"
#include "numbervalue.h"
#include <stdlib.h>
#include <time.h>

RandFunction::RandFunction() : Function("rands")
{
	addParameter("min");
	addParameter("max");
	addParameter("count");
	addParameter("seed");
}

static double frand()
{
	return rand()/(double(RAND_MAX)+1);
}

static double frand(double min, double max)
{
	return (min>max)?frand()*(min-max)+max:frand()*(max-min)+min;
}

Value* RandFunction::evaluate(Context* ctx)
{
	double min=0;
	NumberValue* minVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(minVal)
		min=minVal->getNumber();
	double max=0;
	NumberValue* maxVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,1));
	if(maxVal)
		max=maxVal->getNumber();
	double count=1;
	NumberValue* countVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,2));
	if(countVal)
		count=countVal->getNumber();
	NumberValue* seedVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,3));
	if(seedVal)
		srand((unsigned int)seedVal->getNumber());
	else
		srand((unsigned int)time(0));

	QList<Value*> results;
	for(int i=0; i<count; ++i)
		results.append(new NumberValue(frand(min,max)));

	return new VectorValue(results);
}
