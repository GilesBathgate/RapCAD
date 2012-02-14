#include "minfunction.h"
#include "numbervalue.h"
#include "math.h"

MinFunction::MinFunction() : Function("min")
{
}

Value* MinFunction::evaluate(Context* ctx)
{
	NumberValue* aVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"a"));
	NumberValue* bVal=dynamic_cast<NumberValue*>(ctx->getArgument(1,"b"));
	if(aVal&&bVal) {
		double a=aVal->getNumber();
		double b=bVal->getNumber();

		NumberValue* result;
		result = new NumberValue(fmin(a,b));
		return result;
	}
	return new Value();
}
