#include "maxfunction.h"
#include "numbervalue.h"
#include "math.h"

MaxFunction::MaxFunction() : Function("max")
{
}

Value* MaxFunction::evaluate(Context* ctx)
{
	NumberValue* aVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"a"));
	NumberValue* bVal=dynamic_cast<NumberValue*>(ctx->getArgument(1,"b"));
	if(aVal&&bVal) {
		double a=aVal->getNumber();
		double b=bVal->getNumber();

		return new NumberValue(fmax(a,b));
	}
	return new Value();
}
