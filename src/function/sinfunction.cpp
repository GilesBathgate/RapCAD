#include "sinfunction.h"
#include "numbervalue.h"
#include "math.h"

SinFunction::SinFunction() : Function("sin")
{
}

Value* SinFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"value"));
	if(numVal) {
		double num=numVal->getNumber();

		NumberValue* result;
		result = new NumberValue(sin(num));
		return result;
	}
	return new Value();
}
