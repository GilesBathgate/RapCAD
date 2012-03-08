#include "tanfunction.h"
#include "numbervalue.h"
#include "math.h"

TanFunction::TanFunction() : Function("tan")
{
}

Value* TanFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"value"));
	if(numVal) {
		double num=numVal->getNumber();

		return new NumberValue(tan(num));
	}
	return new Value();
}
