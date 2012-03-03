#include "tanhfunction.h"
#include "numbervalue.h"
#include "math.h"

TanhFunction::TanhFunction() : Function("tanh")
{
}

Value* TanhFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"value"));
	if(numVal) {
		double num=numVal->getNumber();

		NumberValue* result;
		result = new NumberValue(tanh(num));
		return result;
	}
	return new Value();
}

