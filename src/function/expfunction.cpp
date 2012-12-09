#include "expfunction.h"
#include "numbervalue.h"
#include "math.h"

ExpFunction::ExpFunction() : Function("exp")
{
	addParameter("value");
}

Value* ExpFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		double num=numVal->getNumber();

		return new NumberValue(exp(num));
	}
	return new Value();
}
