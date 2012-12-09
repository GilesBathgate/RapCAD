#include "minfunction.h"
#include "numbervalue.h"
#include "math.h"

MinFunction::MinFunction() : Function("min")
{
	addParameter("a");
	addParameter("b");
}

Value* MinFunction::evaluate(Context* ctx)
{
	NumberValue* aVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	NumberValue* bVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,1));
	if(aVal&&bVal) {
		double a=aVal->getNumber();
		double b=bVal->getNumber();

		return new NumberValue(fmin(a,b));
	}
	return new Value();
}
