#include "roundfunction.h"
#include "numbervalue.h"
#include "math.h"

RoundFunction::RoundFunction() : Function("round")
{
	addParameter("value");
}

static double fround(double a)
{
	return a > 0 ? floor(a+0.5) : ceil(a-0.5);
}

Value* RoundFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		double num=numVal->getNumber();

		return new NumberValue(fround(num));
	}
	return new Value();
}
