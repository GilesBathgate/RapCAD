#include "asinfunction.h"
#include "numbervalue.h"
#include "math.h"

AsinFunction::AsinFunction() : Function("asin")
{
}

Value* AsinFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"value"));
	if(numVal) {
		double num=numVal->getNumber();

		return new NumberValue(asin(num));
	}
	return new Value();
}
