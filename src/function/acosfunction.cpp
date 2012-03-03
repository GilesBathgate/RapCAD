#include "acosfunction.h"
#include "numbervalue.h"
#include "math.h"

AcosFunction::AcosFunction() : Function("acos")
{
}

Value* AcosFunction::evaluate(Context* ctx)
{
	NumberValue* numVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"value"));
	if(numVal) {
		double num=numVal->getNumber();

		NumberValue* result;
		result = new NumberValue(acos(num));
		return result;
	}
	return new Value();
}