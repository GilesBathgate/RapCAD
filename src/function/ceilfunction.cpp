#include "ceilfunction.h"
#include "numbervalue.h"
#include "math.h"

CeilFunction::CeilFunction() : Function("ceil")
{
}

Value* CeilFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"value"));
	if(numVal) {
		double num=numVal->getNumber();

		NumberValue* result;
		result = new NumberValue(ceil(num));
		return result;
	}
	return new Value();
}
