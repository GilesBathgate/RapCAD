#include "roundfunction.h"
#include "numbervalue.h"
#include "math.h"

RoundFunction::RoundFunction() : Function("round")
{
}

static double fround(double a)
{
	return a > 0 ? floor(a+0.5) : ceil(a-0.5);
}

Value* RoundFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"value"));
	if(numVal) {
		double num=numVal->getNumber();

		NumberValue* result;
		result = new NumberValue(fround(num));
		return result;
	}
	return new Value();
}
