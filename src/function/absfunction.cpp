#include "absfunction.h"
#include "numbervalue.h"
#include "math.h"

AbsFunction::AbsFunction() : Function("abs")
{
}

Value* AbsFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"value"));
	if(numVal) {
		double num=numVal->getNumber();

		NumberValue* result;
		result = new NumberValue(fabs(num));
		return result;
	}
	return new Value();
}
