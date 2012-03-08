#include "atanfunction.h"
#include "numbervalue.h"
#include "math.h"

AtanFunction::AtanFunction() : Function("atan")
{
}

Value* AtanFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"value"));
	if(numVal) {
		double num=numVal->getNumber();

		return new NumberValue(atan(num));
	}
	return new Value();
}
